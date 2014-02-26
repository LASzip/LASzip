/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "hello.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/mount.h>
#include <pthread.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <boost/algorithm/string.hpp>

#include "ppapi/c/pp_errors.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/ppb.h"
#include "ppapi/c/ppb_instance.h"
#include "ppapi/c/ppb_messaging.h"
#include "ppapi/c/ppb_var.h"
#include "ppapi/c/ppp.h"
#include "ppapi/c/ppp_instance.h"
#include "ppapi/c/ppp_messaging.h"
#include "nacl_io/nacl_io.h"

#include "queue.h"

#include <laszip/laszip.hpp>    
#include <laszip/lasunzipper.hpp>
#include <laszip/laszipper.hpp>


template<class T>
static inline void read_array_field(uint8_t*& src, T* dest, std::size_t count)
{
    memcpy((uint8_t*)dest, (uint8_t*)(T*)src, sizeof(T)*count);
    src += sizeof(T) * count;
    return;
}

template <typename T>
static inline size_t read_n(T& dest, FILE* src, size_t const& num)
{
    return  fread(dest, 1, num, src);
}

template<class T>
static inline T read_field(uint8_t*& src)
{
    T tmp = *(T*)(void*)src;
    src += sizeof(T);
    return tmp;
}




class VLR
{
public:
    uint16_t reserved;
    std::string userId;
    uint16_t recordId;
    uint16_t size;
    std::string description;
    uint8_t* data;
    enum
    {
        eHeaderSize = 54,
        eUserIdSize = 16,
        eDescriptionSize = 32
    };

    
    VLR()
        : reserved(0)
        , userId("")
        , recordId(0)
        , size(0)
        , description("")
        , data(0)
    {}
    ~VLR()
    {
        delete data;
    }
    void read(FILE* fp);
};

void VLR::read(FILE* fp)
{
    // assumes the stream is already positioned to the beginning

    {
        uint8_t* buf1 = new uint8_t[eHeaderSize];
        size_t numRead = read_n(buf1, fp, eHeaderSize);
        if (numRead != eHeaderSize)
        {
            std::ostringstream oss;
            oss << " read size was invalid, not " << eHeaderSize << std::endl;
        }
        uint8_t* p1 = buf1;

        reserved = read_field<uint16_t>(p1);
        
        std::cout << "reserved: " << reserved << std::endl;
        
        uint8_t userId_data[eUserIdSize];
        read_array_field(p1, userId_data, eUserIdSize);
        userId = std::string((const char*)&userId_data);
        // userId = VariableLengthRecord::bytes2string(userId_data, VariableLengthRecord::eUserIdSize);

        recordId = read_field<uint16_t>(p1);
        size = read_field<uint16_t>(p1);
        
        std::cout << "recordId: " << recordId << std::endl;
        std::cout << "size: " << size << std::endl;
        
        uint8_t description_data[eDescriptionSize];
        read_array_field(p1, description_data, eDescriptionSize);
        
        description = std::string(  (const char*)&description_data);
        // description = VariableLengthRecord::bytes2string(description_data, VariableLengthRecord::eDescriptionSize);

        delete[] buf1;
    }

    data = new uint8_t[size];
    {
        read_n(data, fp, size);
    }
    
    std::cout << "read VLR data" << std::endl;
    return;
}


std::vector<VLR*> readVLRs(FILE* fp, int count)
{
    std::vector<VLR*> output;
    
    for (int i = 0; i < count; ++i)
    {
        std::cout << "Reading vlr #:" << i << std::endl;
        VLR* vlr = new VLR;
        vlr->read(fp);
        output.push_back(vlr);
    }
    return output;
}

VLR* getLASzipVLR(std::vector<VLR*> const& vlrs)
{
    std::string userId("laszip encoded");
    uint16_t recordId(22204);
    
    for(size_t i = 0; i < vlrs.size(); ++i)
    {
        VLR* vlr = vlrs[i];
        std::string const& uid = vlr->userId;
        uint16_t rid = vlr->recordId;
        
        std::cout << "VLR recordId: " << rid << std::endl;
        std::cout << "VLR userid: '" << uid <<"'"<< std::endl;
        std::cout << "uid size" << uid.size() << std::endl;
        
        std::cout << "uid equal: " << boost::iequals(uid, userId) << std::endl;
        std::cout << "rid equal: " << (rid == recordId) << std::endl;
        
        if (boost::iequals(uid, userId) && rid == recordId)
            return vlr;
    }
    return 0;
}


static PP_Instance g_instance = 0;
static PPB_GetInterface get_browser_interface = NULL;
static PPB_Messaging* ppb_messaging_interface = NULL;
static PPB_Var* ppb_var_interface = NULL;



/** A handle to the thread the handles messages. */
static pthread_t g_handle_message_thread;

struct PP_Var CStrToVar(const char* str) {
  if (ppb_var_interface != NULL) {
    return ppb_var_interface->VarFromUtf8(str, strlen(str));
  }
  return PP_MakeUndefined();
}

uint32_t VarToCStr(struct PP_Var var, char* buffer, uint32_t length) {
  if (ppb_var_interface != NULL) {
    uint32_t var_length;
    const char* str = ppb_var_interface->VarToUtf8(var, &var_length);
    /* str is NOT NULL-terminated. Copy using memcpy. */
    uint32_t min_length = std::min(var_length, length - 1);
    memcpy(buffer, str, min_length);
    buffer[min_length] = 0;

    return min_length;
  }

  return 0;
}

double applyScaling(const int32_t& v, const double& scale, const double& offset)
{
    return static_cast<double>(v) * scale + offset;
}
/**
 * Handle as message from JavaScript on the worker thread.
 *
 * @param[in] message The message to parse and handle.
 */

static void PostError(std::string const& message)
{
    ppb_messaging_interface->PostMessage(g_instance, CStrToVar(message.c_str()));
}

static void HandleMessage(char* message) {

  int point_size(0);
  
  LASzip zip;
LASunzipper* unzipper = new LASunzipper();
      std::string filename("/http/test.laz");
      // std::ifstream f(filename.c_str(), std::ios::in|std::ios::binary);

    std::ostringstream errors;
        FILE* fp = fopen(filename.c_str(), "r");
        if (!fp)
        {
            errors <<  "Unable to open file: '" << filename << "'" << std::endl;
            PostError(errors.str());
        }
        
        uint32_t point_count(0);
        fseek(fp, 32*3 + 11, SEEK_SET);
        size_t result = fread(&point_count, 4, 1, fp);
        if (result != 1)
        {
            errors << "unable to read point count of size 1, got " << result;
            PostError(errors.str());
        }
        
                
        uint32_t data_offset(0);
        fseek(fp, 32*3, SEEK_SET);
        result = fread(&data_offset, 4, 1, fp);
        if (result != 1)
        {
            errors << "unable to read data offset of size 1, got " << result;
            PostError(errors.str());
        }
        
        uint32_t vlr_count(0);
        fseek(fp, 32*3+4, SEEK_SET);
        result = fread(&vlr_count, 4, 1, fp);
        if (result != 1)
        {
            errors << "unable to vlr count of size 1, got " << result;
            PostError(errors.str());
        }
        
        
        uint16_t header_size(0);
        fseek(fp, 32*3-2, SEEK_SET);
        result = fread(&header_size, 2, 1, fp);
        if (result != 1)
        {
            errors << "unable to header size of size 1, got " << result;
            PostError(errors.str());
        }
        

        fseek(fp, header_size, SEEK_SET);
        std::vector<VLR*> vlrs = readVLRs(fp, vlr_count);
        VLR* zvlr = getLASzipVLR(vlrs);
        
        if (!zvlr)
        {
            errors << "No LASzip VLRs were found in this file! " << result;
            PostError(errors.str());
        }
        
        fseek(fp, data_offset, SEEK_SET);

        
        bool stat = zip.unpack(&(zvlr->data[0]), zvlr->size);
        if (!stat)
        {
            errors << "Unable to unpack LASzip VLR!" << std::endl;
            errors << "error msg: " << unzipper->get_error() << std::endl;            
            PostError(errors.str());
        }
        
        stat = unzipper->open(fp, &zip);
        if (!stat)
        {       
            errors << "Unable to open zip file!" << std::endl;
            errors << "error msg: " << unzipper->get_error() << std::endl;            
            PostError(errors.str());
        }

    unsigned char** point;
    unsigned int point_offset(0);
    point = new unsigned char*[zip.num_items];

    std::ostringstream oss;
    for (unsigned i = 0; i < zip.num_items; i++)
    {
        point_size += zip.items[i].size;
    }

    unsigned char* bytes = new uint8_t[ point_size ];

    for (unsigned i = 0; i < zip.num_items; i++)
    {
        point[i] = &(bytes[point_offset]);
        point_offset += zip.items[i].size;
    }
    
    bool ok = unzipper->read(point);
    if (!ok)
    {     
        errors << "Unable to read point!" << std::endl;
        errors << "error msg: " << unzipper->get_error() << std::endl;            
        PostError(errors.str());
    }
    
    double offset[3] = {0.0};
    double scale[3] = {0.0};
    
    size_t start = 32*3 + 35;
    fseek(fp, start, SEEK_SET);
    

    result = fread(&scale, 8, 3, fp );
    if (result != 3)
    {       
        errors << "unable to read scale information!" << std::endl;
        PostError(errors.str());
    }

    result = fread(&offset, 8, 3, fp );
    if (result != 3)
    {      
        errors << "unable to read offset information!" << std::endl;
        PostError(errors.str());
    }
    
    int32_t x = *(int32_t*)&bytes[0];
    int32_t y = *(int32_t*)&bytes[4];
    int32_t z = *(int32_t*)&bytes[8];
    
    double xs = applyScaling(x, scale[0], offset[0]);
    double ys = applyScaling(y, scale[1], offset[1]);
    double zs = applyScaling(z, scale[2], offset[2]);


    oss << "scale[0]: " << scale[0] <<std::endl;
    oss << "scale[1]: " << scale[1] <<std::endl;
    oss << "scale[2]: " << scale[2] <<std::endl;
    oss << "offset[0]: " << offset[0] <<std::endl;
    oss << "offset[1]: " << offset[1] <<std::endl;
    oss << "offset[2]: " << offset[2] <<std::endl;

    oss << "x: " << x << std::endl; 
    oss  << "y: " << y << std::endl; 
    oss  << "z: " << z << std::endl; 

    oss << "x: " << xs << std::endl; 
    oss  << "y: " << ys << std::endl; 
    oss  << "z: " << zs << std::endl; 

    /* Function returned an output string. Send it to JavaScript. */
    ppb_messaging_interface->PostMessage(g_instance, CStrToVar(oss.str().c_str()));

}

/**
 * A worker thread that handles messages from JavaScript.
 * @param[in] user_data Unused.
 * @return unused.
 */
void* HandleMessageThread(void* user_data) {
  while (1) {
    char* message = DequeueMessage();
    HandleMessage(message);
    free(message);
  }
}

static PP_Bool Instance_DidCreate(PP_Instance instance,
                                  uint32_t argc,
                                  const char* argn[],
                                  const char* argv[]) {
  g_instance = instance;
  nacl_io_init_ppapi(instance, get_browser_interface);

  // By default, nacl_io mounts / to pass through to the original NaCl
  // filesystem (which doesn't do much). Let's remount it to a memfs
  // filesystem.
  umount("/");
  mount("", "/", "memfs", 0, "");

  mount("",                                       /* source */
        "/persistent",                            /* target */
        "html5fs",                                /* filesystemtype */
        0,                                        /* mountflags */
        "type=PERSISTENT,expected_size=1048576"); /* data */

  mount("",       /* source. Use relative URL */
        "/http",  /* target */
        "httpfs", /* filesystemtype */
        0,        /* mountflags */
        "");      /* data */

  pthread_create(&g_handle_message_thread, NULL, &HandleMessageThread, NULL);
  InitializeMessageQueue();

  return PP_TRUE;
}

static void Instance_DidDestroy(PP_Instance instance) {}

static void Instance_DidChangeView(PP_Instance instance,
                                   PP_Resource view_resource) {}

static void Instance_DidChangeFocus(PP_Instance instance, PP_Bool has_focus) {}

static PP_Bool Instance_HandleDocumentLoad(PP_Instance instance,
                                           PP_Resource url_loader) {
  /* NaCl modules do not need to handle the document load function. */
  return PP_FALSE;
}

static void Messaging_HandleMessage(PP_Instance instance,
                                    struct PP_Var message) {
  char buffer[1024];
  VarToCStr(message, &buffer[0], 1024);
  if (!EnqueueMessage(strdup(buffer))) {
    struct PP_Var var;
    // var = PrintfToVar(
    //     "Warning: dropped message \"%s\" because the queue was full.", message);
    // ppb_messaging_interface->PostMessage(g_instance, var);
  }
}

PP_EXPORT int32_t PPP_InitializeModule(PP_Module a_module_id,
                                       PPB_GetInterface get_browser) {
  get_browser_interface = get_browser;
  ppb_messaging_interface =
      (PPB_Messaging*)(get_browser(PPB_MESSAGING_INTERFACE));
  ppb_var_interface = (PPB_Var*)(get_browser(PPB_VAR_INTERFACE));
  return PP_OK;
}

PP_EXPORT const void* PPP_GetInterface(const char* interface_name) {
  if (strcmp(interface_name, PPP_INSTANCE_INTERFACE) == 0) {
    static PPP_Instance instance_interface = {
      &Instance_DidCreate,
      &Instance_DidDestroy,
      &Instance_DidChangeView,
      &Instance_DidChangeFocus,
      &Instance_HandleDocumentLoad,
    };
    return &instance_interface;
  } else if (strcmp(interface_name, PPP_MESSAGING_INTERFACE) == 0) {
    static PPP_Messaging messaging_interface = {
      &Messaging_HandleMessage,
    };
    return &messaging_interface;
  }
  return NULL;
}

PP_EXPORT void PPP_ShutdownModule() {}
