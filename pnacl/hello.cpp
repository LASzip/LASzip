// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/// @file hello_tutorial.cc
/// This example demonstrates loading, running and scripting a very simple NaCl
/// module.  To load the NaCl module, the browser first looks for the
/// CreateModule() factory method (at the end of this file).  It calls
/// CreateModule() once to load the module code.  After the code is loaded,
/// CreateModule() is not called again.
///
/// Once the code is loaded, the browser calls the CreateInstance()
/// method on the object returned by CreateModule().  It calls CreateInstance()
/// each time it encounters an <embed> tag that references your NaCl module.
///
/// The browser can talk to your NaCl module via the postMessage() Javascript
/// function.  When you call postMessage() on your NaCl module from the browser,
/// this becomes a call to the HandleMessage() method of your pp::Instance
/// subclass.  You can send messages back to the browser by calling the
/// PostMessage() method on your pp::Instance.  Note that these two methods
/// (postMessage() in Javascript and PostMessage() in C++) are asynchronous.
/// This means they return immediately - there is no waiting for the message
/// to be handled.  This has implications in your program design, particularly
/// when mutating property values that are exposed to both the browser and the
/// NaCl module.

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

#include <ppapi/cpp/instance.h>
#include <ppapi/cpp/module.h>
#include <ppapi/cpp/var.h>

#include "nacl_io/nacl_io.h"

#include "json/json.h"
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

double applyScaling(const int32_t& v, const double& scale, const double& offset)
{
    return static_cast<double>(v) * scale + offset;
}

/// The Instance class.  One of these exists for each instance of your NaCl
/// module on the web page.  The browser will ask the Module object to create
/// a new Instance for each occurrence of the <embed> tag that has these
/// attributes:
///     src="hello_tutorial.nmf"
///     type="application/x-pnacl"
/// To communicate with the browser, you must override HandleMessage() to
/// receive messages from the browser, and use PostMessage() to send messages
/// back to the browser.  Note that this interface is asynchronous.
class LASzipInstance : public pp::Instance {
 public:
  /// The constructor creates the plugin-side instance.
  /// @param[in] instance the handle to the browser-side plugin instance.
  explicit LASzipInstance(PP_Instance instance) 
      : pp::Instance(instance)
      , m_message_thread(NULL)
      , bCreatedFS(false)
  {



  }
  virtual ~LASzipInstance() 
  {
      std::cout << "~LASzipInstance ... " << std::endl;
      if (m_message_thread)
       (void) pthread_join(m_message_thread, NULL);
  }

  // Initialize the module, staring a worker thread to load the shared object.
  virtual bool Init(uint32_t argc, const char* argn[], const char* argv[]) {
      nacl_io_init_ppapi( pp_instance(), 
                          pp::Module::Get()->get_browser_interface());
      printf( "Constructor thread id '%p'\n", pthread_self());                    


      printf( "createFS thread id '%p'\n", pthread_self());          
      umount("/");
      mount("", "/", "memfs", 0, "");
      
      mount("",                                       /* source */
            "/persistent",                            /* target */
            "html5fs",                                /* filesystemtype */
            0,                                        /* mountflags */
            "type=PERSISTENT,expected_size=1048576"); /* data */

      int res = mount("", "/web", "httpfs", 0, "");
      if (res)
      {
          std::cout << "unable to mount httpfs file system!" << std::endl;
          return false;
      }
      std::cout << "mount res: " << res << std::endl;
      bCreatedFS = true; 
      if (pthread_create( &m_message_thread, 
                      NULL, 
                      &LASzipInstance::HandleMessageThread, 
                      this))
      {
          std::cout << "thread creation failed!" << std::endl;
          return false;
      }
      InitializeMessageQueue();
      return true;
      
  }
  /// Handler for messages coming in from the browser via postMessage().  The
  /// @a var_message can contain be any pp:Var type; for example int, string
  /// Array or Dictinary. Please see the pp:Var documentation for more details.
  /// @param[in] var_message The message posted by the browser.
  virtual void HandleMessage(const pp::Var& var_message) 
  {
      std::string filename = var_message.AsString();

      EnqueueMessage(var_message);
      std::cout << "HandleMessage... " << std::endl;
      int point_size(0);
      printf( "handler thread id '%p'\n", pthread_self());                    
     


  }

  virtual void dosomething(const pp::Var& var_message) 
  {

      std::cout << "HandleMessage... " << std::endl;
      int point_size(0);
      printf( "handler thread id '%p'\n", pthread_self());                    
  
      LASzip zip;
    LASunzipper* unzipper = new LASunzipper();
        std::string filename = var_message.AsString();
          // std::string filename("test.laz");
          // std::ifstream f(filename.c_str(), std::ios::in|std::ios::binary);

        std::ostringstream errors;
            FILE* fp = fopen(filename.c_str(), "r");
            if (!fp)
            {
                errors <<  "Unable to open file: '" << filename << "' " <<  strerror(errno) <<std::endl;
                PostMessage(errors.str());
                return;
            }
        
            uint32_t point_count(0);
            fseek(fp, 32*3 + 11, SEEK_SET);
            size_t result = fread(&point_count, 4, 1, fp);
            if (result != 1)
            {
                errors << "unable to read point count of size 1, got " << result;
                PostMessage(errors.str());
                return;
            }
        
                
            uint32_t data_offset(0);
            fseek(fp, 32*3, SEEK_SET);
            result = fread(&data_offset, 4, 1, fp);
            if (result != 1)
            {
                errors << "unable to read data offset of size 1, got " << result;
                PostMessage(errors.str());
                return;
            }
        
            uint32_t vlr_count(0);
            fseek(fp, 32*3+4, SEEK_SET);
            result = fread(&vlr_count, 4, 1, fp);
            if (result != 1)
            {
                errors << "unable to vlr count of size 1, got " << result;
                PostMessage(errors.str());
                return;                
            }
        
        
            uint16_t header_size(0);
            fseek(fp, 32*3-2, SEEK_SET);
            result = fread(&header_size, 2, 1, fp);
            if (result != 1)
            {
                errors << "unable to header size of size 1, got " << result;
                PostMessage(errors.str());
                return;
            }
        

            fseek(fp, header_size, SEEK_SET);
            std::vector<VLR*> vlrs = readVLRs(fp, vlr_count);
            VLR* zvlr = getLASzipVLR(vlrs);
        
            if (!zvlr)
            {
                errors << "No LASzip VLRs were found in this file! " << result;
                PostMessage(errors.str());
                return;
            }
        
            fseek(fp, data_offset, SEEK_SET);

        
            bool stat = zip.unpack(&(zvlr->data[0]), zvlr->size);
            if (!stat)
            {
                errors << "Unable to unpack LASzip VLR!" << std::endl;
                errors << "error msg: " << unzipper->get_error() << std::endl;            
                PostMessage(errors.str());
                return;
            }
        
            stat = unzipper->open(fp, &zip);
            if (!stat)
            {       
                errors << "Unable to open zip file!" << std::endl;
                errors << "error msg: " << unzipper->get_error() << std::endl;            
                PostMessage(errors.str());
                return;
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
            PostMessage(errors.str());
            return;
        }
    
        double offset[3] = {0.0};
        double scale[3] = {0.0};
    
        size_t start = 32*3 + 35;
        fseek(fp, start, SEEK_SET);
    

        result = fread(&scale, 8, 3, fp );
        if (result != 3)
        {       
            errors << "unable to read scale information!" << std::endl;
            PostMessage(errors.str());
            return;
        }

        result = fread(&offset, 8, 3, fp );
        if (result != 3)
        {      
            errors << "unable to read offset information!" << std::endl;
            PostMessage(errors.str());
            return;
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
        PostMessage(oss.str());


  }

  static void* HandleMessageThread(void* This) {
      std::cout << "HandleMessageThread..." << std::endl;
    while (1) {
      pp::Var message = DequeueMessage();
      std::cout << "DequeueMessage..." << std::endl;      
      ((LASzipInstance*)This)->dosomething(message);
    }
  }
  
  pthread_t m_message_thread;
  bool bCreatedFS;
};

/// The Module class.  The browser calls the CreateInstance() method to create
/// an instance of your NaCl module on the web page.  The browser creates a new
/// instance for each <embed> tag with type="application/x-pnacl".
class LASzipModule : public pp::Module {
 public:
  LASzipModule() : pp::Module() {}
  virtual ~LASzipModule() {}

  /// Create and return a LASzipInstance object.
  /// @param[in] instance The browser-side instance.
  /// @return the plugin-side instance.
  virtual pp::Instance* CreateInstance(PP_Instance instance) {
    return new LASzipInstance(instance);
  }
};

namespace pp {
/// Factory function called by the browser when the module is first loaded.
/// The browser keeps a singleton of this module.  It calls the
/// CreateInstance() method on the object you return to make instances.  There
/// is one instance per <embed> tag on the page.  This is the main binding
/// point for your NaCl module with the browser.
Module* CreateModule() {
  return new LASzipModule();
}
}  // namespace pp
