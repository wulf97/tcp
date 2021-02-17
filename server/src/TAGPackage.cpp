#include "TAGPackage.h"

extern std::mutex g_mtx;

uint32_t my::TAGPackage::s_gid = 0;

/************/
my::TAGPackage::TAGPackage() {
    m_id = s_gid;
    s_gid++;
}

/************/
my::TAGPackage::~TAGPackage() {

}

/************/
bool my::TAGPackage::unwrapData(const uint8_t *buf, size_t size) {
    if (size > 0) {
        struct dataBlock dBlock;
        if (m_isFirstBlock) {
            m_receiveSize = LongInt<uint8_t>(std::string("0"));

            if (buf[0] & (1 << 7))
                m_dataType = DATA_T::FILE;
            else
                m_dataType = DATA_T::TEXT;

            if (buf[1] & (1 << 7)) {
                // Long form
                m_header.sLeng = buf[1];
                m_header.headerSize = (buf[1] & ~(1 << 7)) + 2;

                std::vector<uint8_t> v;
                for (int i = 0; i < m_header.headerSize - 2; i++) {
                    v.push_back(reinterpret_cast<uint8_t>(buf[i + 2]));
                }

                LongInt<uint8_t> hSize(std::to_string(m_header.headerSize));
                m_header.advLen = LongInt<uint8_t>(v);
                m_leng = m_header.advLen;
                m_leng += hSize;
            } else {
                // Short form
                m_header.sLeng = buf[1];
                m_header.headerSize = 2;
                m_leng = LongInt<uint8_t>(std::to_string(m_header.sLeng + m_header.headerSize));
            }

            m_isFirstBlock = false;
            dBlock.size = size - m_header.headerSize;
            std::memcpy(dBlock.data, buf + m_header.headerSize, size - m_header.headerSize);
        } else {
            dBlock.size = size;
            std::memcpy(dBlock.data, buf, size);
        }

        m_blocksQue.push(dBlock);

        LongInt<uint8_t> blockSize(std::to_string(size));
        m_receiveSize += blockSize;

        if (m_receiveSize != m_leng) {
            return true;
        } else {
            m_isFirstBlock = true;
            return false;
        }

    }


    return false;
}

/************/
bool my::TAGPackage::getData(dataBlock &dBlock) {
    if (m_blocksQue.size() == 0)
        return false;

    dBlock = m_blocksQue.front();
    m_blocksQue.pop();

    return true;
}

/************/
uint32_t my::TAGPackage::getId() const {
    return m_id;
}

/************/
void my::TAGPackage::printSize() const {
    m_leng.printSrc();
}

/************/
my::DATA_T my::TAGPackage::getDataType() const {
    return m_dataType;
}

/************/
void my::TAGPackage::printHeader() const {
    std::string str;

    str.resize(m_header.headerSize);
    str[0] = m_header.tag;
    str[1] = m_header.sLeng;
    if (m_header.headerSize > 2)
        std::memcpy(&str.front() + 2, m_header.advLen.getDataVec().data(), str.length() - 2);

    uint32_t num {0};
    for (auto it : str) {
        std::memcpy(&num, &it, sizeof(it));
        std::cout << "0x" << std::hex << num << std::dec << " ";
    }
}

