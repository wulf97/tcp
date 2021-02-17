#ifndef TAGPACKAGE_H
#define TAGPACKAGE_H

#include <iostream>
#include <string>
#include <cstring>
#include <cinttypes>
#include <queue>
#include <mutex>

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
    TAGPackage();
    virtual ~TAGPackage();

    bool unwrapData(const uint8_t *buf, size_t size);
    bool getData(dataBlock &dBlock);
    uint32_t getId() const;
    DATA_T getDataType() const;
    void printSize() const;
    void printHeader() const;
private:
    LongInt<uint8_t> m_leng;
    LongInt<uint8_t> m_receiveSize;
    std::queue<dataBlock> m_blocksQue;
    header m_header;
    DATA_T m_dataType;
    bool m_isFirstBlock {true};
    uint32_t m_id;
    static uint32_t s_gid;
};
}

#endif // TAGPACKAGE_H
