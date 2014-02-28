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
#include <ppapi/cpp/var_dictionary.h>
#include <ppapi/cpp/var_array_buffer.h>


#include "nacl_io/nacl_io.h"

#include "json/json.h"
#include "json/reader.h"
#include "json/writer.h"
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

class LASHeader
{
public:
    uint32_t point_count;
    uint32_t vlr_count;
    uint16_t header_size;
    uint32_t data_offset;
    uint8_t point_format_id;
    uint16_t point_record_length;
    double scale[3];
    double offset[3];
    double maxs[3];
    double mins[3];

    LASHeader()
        : point_count (0)
        , vlr_count (0)
        , header_size(0)
        , data_offset(0)
        , point_format_id(0)
        , point_record_length(0)
    {
        for (int i = 0; i < 3; ++i)
        {
            scale[i] = 0.0;
            offset[i] = 0.0;
            mins[i] = 0.0;
            maxs[i] = 0.0;
        }
    }
    
    pp::Var AsVar() const
    {
        pp::VarDictionary output;
        pp::VarArray scales;
        pp::VarArray offsets;
        pp::VarArray maximums;
        pp::VarArray minimums;
        output.Set("point_count", pp::Var(int32_t(point_count)));
        output.Set("vlr_count", pp::Var(int32_t(vlr_count)));
        output.Set("header_size", pp::Var(int32_t(header_size)));
        output.Set("data_offset", pp::Var(int32_t(data_offset)));
        output.Set("point_format_id", pp::Var(int32_t(point_format_id)));
        output.Set("point_record_length", pp::Var(int32_t(point_record_length)));
        for (int i=0; i < 3; ++i)
        {
            scales.Set(i, scale[i]);
            offsets.Set(i, offset[i]);
            maximums.Set(i, maxs[i]);
            minimums.Set(i, mins[i]);
        }
        output.Set("mins", minimums);
        output.Set("maxs", maximums);
        output.Set("offsets", offsets);
        output.Set("scales", scales);
        
        return pp::Var(output);
    }
};
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
      , message_thread_(NULL)
      , bCreatedFS_(false)
      , fp_(0)
      , bDidReadHeader_(false)
      , pointIndex_(0)
      , point_(0)
      , bytes_(0)
  {



  }
  virtual ~LASzipInstance() 
  {
      std::cout << "~LASzipInstance ... " << std::endl;
      if (message_thread_)
       (void) pthread_join(message_thread_, NULL);
  }

  // Initialize the module, staring a worker thread to load the shared object.
  virtual bool Init(uint32_t argc, const char* argn[], const char* argv[]) {
      nacl_io_init_ppapi( pp_instance(), 
                          pp::Module::Get()->get_browser_interface());
      printf( "Constructor thread id '%p'\n", pthread_self());                    


      printf( "createFS thread id '%p'\n", pthread_self());          
      umount("/");
      int res = mount("", "/", "memfs", 0, "");
      // int res = mount("", "/web", "httpfs", 0, "");
      if (res)
      {
          std::cout << "unable to mount httpfs file system!" << std::endl;
          bCreatedFS_ = true; 
          return false;
      }
      std::cout << "mount res: " << res << std::endl;

      if (pthread_create( &message_thread_, 
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
      EnqueueMessage(var_message);
      std::cout << "HandleMessage... " << std::endl;
      int point_size(0);
      printf( "handler thread id '%p'\n", pthread_self());                    
 
  }
  
  
  void PostError(std::string const& message)
  {
      Json::Value value(Json::objectValue);
      value["error"] = true;
      value["message"] = message;
      Json::FastWriter writer;
      PostMessage(writer.write(value));
  }
  
    bool open(const pp::Var& message)
    {
        pp::VarDictionary dict(message);
        
        if (!dict.HasKey("target"))
        {
            std::ostringstream errors;
            errors <<  "No 'target' member given to 'open' message";
            PostError(errors.str());
            return false;
        }
        pp::Var fname = dict.Get("target");
        if (!fname.is_string()) 
        {
            std::ostringstream errors;
            errors <<  "'filename' member is not a string";
            PostError(errors.str());
            return false;
        }
        std::string filename ="/"+ fname.AsString();
        std::cout << "saving file as " << filename << std::endl;
        FILE* fp = fopen(filename.c_str(), "w");
        if (!fp)
        {
            std::ostringstream errors;
            errors <<  "Unable to open to write data: '" 
                   << filename << "' "
                   << strerror(errno);
            PostError(errors.str());
            return false;
        }
        pp::Var buffer = dict.Get("buffer");
        if (!buffer.is_array_buffer())
        {
            PostError("'buffer' is not an ArrayBuffer object!");
            return false;
        }
        pp::VarArrayBuffer buf(buffer);
        
        unsigned char* data = static_cast<unsigned char*>(buf.Map());
        
        size_t amt = fwrite(data, 1, buf.ByteLength(), fp);
        if (amt != buf.ByteLength())
        {
            PostError("We weren't able to write entire file to memory!");
            return false;
        }
        
        fflush(fp);
        fclose(fp);

        fp_ = fopen(filename.c_str(), "r");
        if (!fp_)
        {
            std::ostringstream errors;
            errors <<  "Unable to open to read memory data: '" 
                   << filename << "' "
                   << strerror(errno);
            PostError(errors.str());
            return false;
        }  
        
        return true;
    }
    
    bool readHeader(LASHeader& header)
    {
        std::ostringstream errors;        
        fseek(fp_, 32*3 + 11, SEEK_SET);
        size_t result = fread(&header.point_count, 4, 1, fp_);
        if (result != 1)
        {
            errors << "unable to read point count of size 1, got " << result;
            PostError(errors.str());
            return false;
        }
    
        fseek(fp_, 32*3, SEEK_SET);
        result = fread(&header.data_offset, 4, 1, fp_);
        if (result != 1)
        {
            errors << "unable to read data offset of size 1, got " << result;
            PostError(errors.str());
            return false;
        }

        fseek(fp_, 32*3+4, SEEK_SET);
        result = fread(&header.vlr_count, 4, 1, fp_);
        if (result != 1)
        {
            errors << "unable to vlr count of size 1, got " << result;
            PostError(errors.str());
            return false;
        }
    

        fseek(fp_, 32*3-2, SEEK_SET);
        result = fread(&header.header_size, 2, 1, fp_);
        if (result != 1)
        {
            errors << "unable to header size of size 1, got " << result;
            PostError(errors.str());
            return false;
        }

        fseek(fp_, 32*3 + 8, SEEK_SET);
        result = fread(&header.point_format_id, 1, 1, fp_);
        if (result != 1)
        {
            errors << "unable to header point_format_id of size 1, got " << result;
            PostError(errors.str());
            return false;
        }

        uint8_t compression_bit_7 = (header.point_format_id & 0x80) >> 7;
        uint8_t compression_bit_6 = (header.point_format_id & 0x40) >> 6;
        
        if (!compression_bit_7 && !compression_bit_6 )
        {
            errors << "This file is not a LASzip file. Is it uncompressed LAS? ";
            PostError(errors.str());
            return false;
        }
        if (compression_bit_7 && compression_bit_6)
        {
            errors << "This is LASzip, but it was compressed by an ancient compressor version that is not open source ";
            PostError(errors.str());
            return false;            
        }
        header.point_format_id = header.point_format_id &= 0x3f;

        fseek(fp_, 32*3 + 8+1, SEEK_SET);
        result = fread(&header.point_record_length, 2, 1, fp_);
        if (result != 1)
        {
            errors << "unable to header point_record_length of size 1, got " << result;
            PostError(errors.str());
            return false;
        }        


        size_t start = 32*3 + 35;
        fseek(fp_, start, SEEK_SET);
    

        result = fread(&header.scale, 8, 3, fp_ );
        if (result != 3)
        {       
            errors << "unable to read scale information!" << std::endl;
            PostError(errors.str());
            return false;
        }
        

        result = fread(&header.offset, 8, 3, fp_ );
        if (result != 3)
        {      
            errors << "unable to read offset information!" << std::endl;
            PostError(errors.str());
            return false;
        }
        
        result = fread(&header.maxs[0], 8, 1, fp_ );
        if (result != 1)
        {      
            errors << "unable to read minx information!" << std::endl;
            PostError(errors.str());
            return false;
        }
        result = fread(&header.mins[0], 8, 1, fp_ );
        if (result != 1)
        {      
            errors << "unable to read minx information!" << std::endl;
            PostError(errors.str());
            return false;
        }
        result = fread(&header.maxs[1], 8, 1, fp_ );
        if (result != 1)
        {      
            errors << "unable to read maxy information!" << std::endl;
            PostError(errors.str());
            return false;
        }
        result = fread(&header.mins[1], 8, 1, fp_ );
        if (result != 1)
        {      
            errors << "unable to read miny information!" << std::endl;
            PostError(errors.str());
            return false;
        }

        result = fread(&header.maxs[2], 8, 1, fp_ );
        if (result != 1)
        {      
            errors << "unable to read maxz information!" << std::endl;
            PostError(errors.str());
            return false;
        }
        result = fread(&header.mins[2], 8, 1, fp_ );
        if (result != 1)
        {      
            errors << "unable to read minz information!" << std::endl;
            PostError(errors.str());
            return false;
        }


        fseek(fp_, header_.header_size, SEEK_SET);
        std::vector<VLR*> vlrs = readVLRs(fp_, header_.vlr_count);
        VLR* zvlr = getLASzipVLR(vlrs);
    
        if (!zvlr)
        {
            errors << "No LASzip VLRs were found in this file! ";
            PostError(errors.str());
            return false;
        }
        bool stat = zip_.unpack(&(zvlr->data[0]), zvlr->size);
        if (!stat)
        {
            errors << "Unable to unpack LASzip VLR!" << std::endl;
            errors << "error msg: " << unzipper_.get_error() << std::endl;            
            PostError(errors.str());
            return false;
        }
    
        fseek(fp_, header_.data_offset, SEEK_SET);
        stat = unzipper_.open(fp_, &zip_);
        if (!stat)
        {       
            errors << "Unable to open zip file!" << std::endl;
            errors << "error msg: " << unzipper_.get_error() << std::endl;            
            PostError(errors.str());
            return false;
        }
        
        unsigned int point_offset(0);
        point_ = new unsigned char*[zip_.num_items];
        uint32_t point_size(0);
        for (unsigned i = 0; i < zip_.num_items; i++)
        {
            point_size += zip_.items[i].size;
        }

        bytes_ = new uint8_t[ point_size ];

        for (unsigned i = 0; i < zip_.num_items; i++)
        {
            point_[i] = &(bytes_[point_offset]);
            point_offset += zip_.items[i].size;
        }
                
        return true;
    }
    
    pp::Var status(bool bStatus, std::string const& message)
    {
        pp::VarDictionary dict;
        dict.Set("status", bStatus);
        dict.Set("message", message);
        return pp::Var(dict);
    }
  virtual void dosomething(const pp::Var& var_message) 
  {

      std::cout << "HandleMessage... " << std::endl;
      int point_size(0);
      printf( "handler thread id '%p'\n", pthread_self());                    
      
      pp::VarDictionary dict;
      pp::Var command_name;
      
      if (var_message.is_dictionary())
      {
          dict = pp::VarDictionary(var_message);
          if (!dict.HasKey("command"))
          {
              PostError("message JSON provided no 'command' member!");
              return;              
          }
          command_name = dict.Get("command");      
      } else
      {
          PostError("No dictionary object was provided!");
          return;          
      }

      if (boost::iequals(command_name.AsString(), "open"))
      {
          bool opened = open(var_message);
          if (!opened)
          {
              PostMessage(status(false, "Unable to open file"));
              return;
          }
          PostMessage(status(true, "File opened successfully"));
          return; // open has set any errors          
          
      }

      if (boost::iequals(command_name.AsString(), "getheader"))
      {
          if (!fp_)
          {
              PostError("No file is open!");
              return;
          }
          bDidReadHeader_ = readHeader(header_);
          if (!bDidReadHeader_)
          {
              PostError("Header read failed!");
              return;
          }
          PostMessage(header_.AsVar());
          return;

      }
        // std::ostringstream errors;

      if (boost::iequals(command_name.AsString(), "read"))
      {
          if (!fp_)
          {
              PostError("No file is open!");
              return;
          }
          if (!bDidReadHeader_)
          {
              PostError("No header has been fetched!");
              return;
          }
          uint32_t count(header_.point_count);
          if (!dict.HasKey("buffer"))
          {
              PostError("No 'buffer' ArrayBuffer object was given!");
              return;
          }
          pp::Var buffer = dict.Get("buffer");
          if (!buffer.is_array_buffer())
          {
              PostError("'buffer' is not an ArrayBuffer object!");
              return;
          }
          
          uint32_t num_left = header_.point_count - pointIndex_;
          pp::VarArrayBuffer buf(buffer);
          if (buf.ByteLength() < num_left*header_.point_record_length != 0)
          {
              std::ostringstream error;
              error << "'buffer'::ByteLength() < num_left*header_.point_record_length!"
                  << " buf.ByteLength(): " << buf.ByteLength() 
                      << " and num_left*header_.point_record_length: " << num_left*header_.point_record_length;
              PostError(error.str());
          }
          
          unsigned char* start = static_cast<unsigned char*>(buf.Map());
          unsigned char* data = start;
          
          for (int i = pointIndex_; i < header_.point_count; ++i)
          {
              // fills in bytes_
                bool ok = unzipper_.read(point_);
                if (!ok)
                {
                      std::ostringstream error;
                    error << "Unable to read point at index " << i << std::endl;
                    error << "error msg: " << unzipper_.get_error() << std::endl;            
                    PostError(error.str());
                    return;
                }
                std::copy(bytes_, bytes_ + header_.point_record_length, data);
                data += header_.point_record_length;
                pointIndex_ = i;
                
          }
          buf.Unmap();
          
          pp::VarDictionary dict;
          dict.Set("status", true);
          dict.Set("message", "Done reading data");
          dict.Set("buffer", buf);
          PostMessage(dict);
          return;
                    
        // int32_t x = *(int32_t*)&start[0];
        // int32_t y = *(int32_t*)&start[4];
        // int32_t z = *(int32_t*)&start[8];
        //     
        // double xs = applyScaling(x, header_.scale[0], header_.offset[0]);
        // double ys = applyScaling(y, header_.scale[1], header_.offset[1]);
        // double zs = applyScaling(z, header_.scale[2], header_.offset[2]);
        // 
        // std::ostringstream oss;
        // oss << "scale[0]: " << header_.scale[0] <<std::endl;
        // oss << "scale[1]: " << header_.scale[1] <<std::endl;
        // oss << "scale[2]: " << header_.scale[2] <<std::endl;
        // oss << "offset[0]: " << header_.offset[0] <<std::endl;
        // oss << "offset[1]: " << header_.offset[1] <<std::endl;
        // oss << "offset[2]: " << header_.offset[2] <<std::endl;
        // oss << "min[0]: " << header_.mins[0] <<std::endl;
        // oss << "min[1]: " << header_.mins[1] <<std::endl;
        // oss << "min[2]: " << header_.mins[2] <<std::endl;
        // oss << "max[0]: " << header_.maxs[0] <<std::endl;
        // oss << "max[1]: " << header_.maxs[1] <<std::endl;
        // oss << "max[2]: " << header_.maxs[2] <<std::endl;
        // 
        // oss << "x: " << x << std::endl; 
        // oss  << "y: " << y << std::endl; 
        // oss  << "z: " << z << std::endl; 
        // 
        // oss << "x: " << xs << std::endl; 
        // oss  << "y: " << ys << std::endl; 
        // oss  << "z: " << zs << std::endl; 
        // PostMessage(oss.str());
        // PostMessage(header_.AsVar());
          
          // PostMessage(status(true, "Done reading data"));
          // return;

      }
      
      PostError("Command not found");
      return;
        

        // unsigned char** point;
 
    
        // bool ok = unzipper_.read(point_);
        // if (!ok)
        // {     
        //     errors << "Unable to read point!" << std::endl;
        //     errors << "error msg: " << unzipper_.get_error() << std::endl;            
        //     PostError(errors.str());
        //     return;
        // }
        //     

    
        // int32_t x = *(int32_t*)&bytes_[0];
        // int32_t y = *(int32_t*)&bytes_[4];
        // int32_t z = *(int32_t*)&bytes_[8];
        //     
        // double xs = applyScaling(x, header_.scale[0], header_.offset[0]);
        // double ys = applyScaling(y, header_.scale[1], header_.offset[1]);
        // double zs = applyScaling(z, header_.scale[2], header_.offset[2]);
        // 
        // std::ostringstream oss;
        // oss << "scale[0]: " << header_.scale[0] <<std::endl;
        // oss << "scale[1]: " << header_.scale[1] <<std::endl;
        // oss << "scale[2]: " << header_.scale[2] <<std::endl;
        // oss << "offset[0]: " << header_.offset[0] <<std::endl;
        // oss << "offset[1]: " << header_.offset[1] <<std::endl;
        // oss << "offset[2]: " << header_.offset[2] <<std::endl;
        // oss << "min[0]: " << header_.mins[0] <<std::endl;
        // oss << "min[1]: " << header_.mins[1] <<std::endl;
        // oss << "min[2]: " << header_.mins[2] <<std::endl;
        // oss << "max[0]: " << header_.maxs[0] <<std::endl;
        // oss << "max[1]: " << header_.maxs[1] <<std::endl;
        // oss << "max[2]: " << header_.maxs[2] <<std::endl;
        // 
        // oss << "x: " << x << std::endl; 
        // oss  << "y: " << y << std::endl; 
        // oss  << "z: " << z << std::endl; 
        // 
        // oss << "x: " << xs << std::endl; 
        // oss  << "y: " << ys << std::endl; 
        // oss  << "z: " << zs << std::endl; 
        // PostMessage(header_.AsVar());


  }

  static void* HandleMessageThread(void* This) {
      std::cout << "HandleMessageThread..." << std::endl;
    while (1) {
      pp::Var message = DequeueMessage();
      std::cout << "DequeueMessage..." << std::endl;      
      ((LASzipInstance*)This)->dosomething(message);
    }
  }
  
  pthread_t message_thread_;
  bool bCreatedFS_;
  LASzip zip_;
  LASunzipper unzipper_;
  FILE* fp_;
  LASHeader header_;
  bool bDidReadHeader_;
  uint32_t pointIndex_;
  unsigned char** point_;
  unsigned char* bytes_;
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
