#ifndef TAGPACKAGE_H
#define TAGPACKAGE_H

#include <iostream>
#include <string>
#include <cstring>
#include <cinttypes>
#include <queue>

#include "LongInt.h"

#define MAX_DATA_BLOCK_SIZE 256

namespace my
{

// Data type
enum DATA_T{
    TEXT,
    FILE
};

struct header {
    uint8_t tag;
    uint8_t sLeng;
    LongInt<uint8_t> advLen;
    size_t headerSize;
};

struct dataBlock {
    uint8_t data[MAX_DATA_BLOCK_SIZE];
    uint16_t size;
};

class TAGPackage {
public:
    TAGPackage(LongInt<uint8_t> len, DATA_T dataType = DATA_T::TEXT);
    virtual ~TAGPackage();

    void wrapData(const uint8_t *buf, size_t size);
    bool getData(dataBlock &dBlock);
private:
    LongInt<uint8_t> m_leng;
    std::queue<dataBlock> m_blocksQue;
    header m_header;
    DATA_T m_dataType;
    bool m_isFirstBlock {true};
};
}

#endif // TAGPACKAGE_H
