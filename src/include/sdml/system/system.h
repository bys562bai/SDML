#pragma once
#include <stdint.h>

namespace sdml{
    inline bool isBigEndian() {
        union{
            int16_t data;
            struct 
            {
                int8_t lowbyte;
                int8_t highbyte;
            };
        } checkData;
        checkData.lowbyte = -1;
        checkData.highbyte = 0;
        return checkData.data < 0;
    }
}