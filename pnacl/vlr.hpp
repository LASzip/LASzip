// (c) 2014, Howard Butler, howard@hobu.co 
// This code is LGPL2.1 with static linking exception.

#ifndef NACL_VLR_MODULE
#define NACL_VLR_MODULE

#include <stdint.h>
#include <string>
#include <sstream>
#include <stdio.h>

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
        delete[] data;
    }
    inline void read(FILE* fp)
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

            uint8_t userId_data[eUserIdSize];
            read_array_field(p1, userId_data, eUserIdSize);
            userId = std::string((const char*)&userId_data);

            recordId = read_field<uint16_t>(p1);
            size = read_field<uint16_t>(p1);
        
            uint8_t description_data[eDescriptionSize];
            read_array_field(p1, description_data, eDescriptionSize);
            description = std::string(  (const char*)&description_data);

            delete[] buf1;
        }

        data = new uint8_t[size];
        {
            read_n(data, fp, size);
        }

        return;
    }    
};


#endif
