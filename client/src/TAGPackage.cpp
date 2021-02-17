#include "TAGPackage.h"

/************/
my::TAGPackage::TAGPackage(LongInt<uint8_t> len, DATA_T dataType) {
    LongInt<uint8_t> l127(std::string("127"));

    m_dataType = dataType;

    switch (dataType) {
    case DATA_T::TEXT:
        m_header.tag = 0;
        break;
    case DATA_T::FILE:
        m_header.tag = 1 << 7;
        break;
    }

    // Fill header structure
    if (len > l127) {
        // Long form
        m_leng = len;
        m_header.sLeng = (1 << 7) | len.getSizeInBytes();
        m_header.advLen = LongInt<uint8_t>(len);
        m_header.headerSize = 2 + len.getSizeInBytes();
    } else {
        // Short form
        m_leng = len;
        m_header.sLeng = len.getDataVec().at(0);
        m_header.headerSize = 2;
    }
}

/************/
my::TAGPackage::~TAGPackage() {

}

/************/
void my::TAGPackage::wrapData(const uint8_t *buf, size_t size) {
    if (m_isFirstBlock) {
        struct dataBlock dBlock;
        dBlock.data[0] = m_header.tag;
        dBlock.data[1] = m_header.sLeng;
        dBlock.size = m_header.headerSize;
        std::memcpy(dBlock.data + 2,
                    m_header.advLen.getDataVec().data(),
                    m_header.headerSize - 2);

        m_blocksQue.push(dBlock);
        m_isFirstBlock = false;
    }

    size_t totalSize = 0;
    while (totalSize < size) {
        struct dataBlock dBlock;
        if ((size - totalSize)/MAX_DATA_BLOCK_SIZE > 0) {
            std::memcpy(dBlock.data, buf, MAX_DATA_BLOCK_SIZE);
            dBlock.size = MAX_DATA_BLOCK_SIZE;
            m_blocksQue.push(dBlock);
        } else {
            std::memcpy(dBlock.data, buf, size - totalSize);
            dBlock.size = size - totalSize;
            m_blocksQue.push(dBlock);
        }

        totalSize += dBlock.size;
    }
}

/************/
bool my::TAGPackage::getData(dataBlock &dBlock) {
    if (m_blocksQue.size() == 0)
        return false;

    dBlock = m_blocksQue.front();
    m_blocksQue.pop();

    return true;
}
