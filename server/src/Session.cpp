#include "Session.h"
#include "TAGPackage.h"
#include "TextColor.h"

extern std::mutex g_mtx;

unsigned int my::Session::s_genId = 0;

/************/
my::Session::Session(int sockfd, struct sockaddr_in addr) : m_sockfd(sockfd), m_addr(addr) {
    #ifdef __linux__
    char *buf = new char[INET_ADDRSTRLEN];
    #endif

    m_id = s_genId++;
    m_sockfd = sockfd;
    m_addr = addr;

    #ifdef __linux__
    m_ip = inet_ntop(AF_INET, &(addr.sin_addr), buf, INET_ADDRSTRLEN);
    #elif _WIN32
    m_ip = inet_ntoa(m_addr.sin_addr);
    #endif
    m_port = ntohs(addr.sin_port);



    m_thread = std::thread(&my::Session::init, this);

    g_mtx.lock();
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::cout.flush();
    std::cout << TC_BLUE << "\r* [" << std::put_time(std::localtime(&time), "%Y-%m-%d %X");
    std::cout << "] ";
    std::cout << TC_BLUE << "[sid " << m_id << "]" << TC_DEFAULT << " Session run in the thread 0x" << std::hex << m_thread.get_id() << std::dec << std::endl;
    std::cout << TC_BLUE << "*" << TC_DEFAULT << " User IP: " << m_ip << std::endl;
    std::cout << TC_BLUE << "*" << TC_DEFAULT << " User port: " << m_port << std::endl << std::endl;
    g_mtx.unlock();

    #ifdef __linux__
    delete buf;
    #endif
}

/************/
my::Session::~Session() {
    if (m_sockfd > 0) {
        #ifdef __linux__
        close(m_sockfd);
        #elif _WIN32
        closesocket(m_sockfd);
        #endif
        m_sockfd = 0;
    }

    for (auto it : m_packages) {
        delete it;
    }

    m_packages.clear();

    m_isStoped = true;
    if (m_thread.joinable())
        m_thread.join();
}

/************/
unsigned int my::Session::getId() const {
    return m_id;
}

/************/
bool my::Session::init() {
    while (1) {
        char buf[BLOCK_SIZE];

        #ifdef __linux__
        int nBytes = recv(m_sockfd, &buf, BLOCK_SIZE, 0);
        #elif _WIN32
        int nBytes = recv(m_sockfd, reinterpret_cast<char*>(&buf), BLOCK_SIZE, 0);
        #endif

        if (nBytes == -1) {
            std::cerr << TC_RED << "ERROR " << errno << ": Receiving data problem" << std::endl;
            #ifdef __linux__
            std::cerr << gai_strerror(errno) << TC_DEFAULT << std::endl;
            #endif
        }

        if (nBytes > 0) {
//            std::cout << nBytes << std::endl;
            if (!m_isReceive) {
                m_currentPackage = new TAGPackage();
                m_nPackages++;
                m_isReceive = true;
            }

            char *dBlock = new char[nBytes];
            memcpy(dBlock, &buf, nBytes);
            if (!m_currentPackage->unwrapData(reinterpret_cast<const uint8_t*>(dBlock), nBytes)) {
                m_packages.push_back(m_currentPackage);
                m_isReceive = false;

                g_mtx.lock();
                auto now = std::chrono::system_clock::now();
                auto time = std::chrono::system_clock::to_time_t(now);

                std::cout << TC_GREEN << "\r* [" << std::put_time(std::localtime(&time), "%Y-%m-%d %X");
                std::cout << "] ";
                std::cout << "[pid " << m_currentPackage->getId() << "]" << TC_DEFAULT << " Reseive package" << std::endl;
                std::cout << TC_GREEN << "*" << TC_DEFAULT << " Total size: ";
                m_currentPackage->printSize();
                std::cout << "bytes" << std::endl;
                std::cout << TC_GREEN << "*" << TC_DEFAULT << " From: " << m_ip << ":" << m_port << std::endl;
                std::cout << TC_GREEN << "*" << TC_DEFAULT << " Header: ";
                m_currentPackage->printHeader();
                std::cout << std::endl;
                if (m_currentPackage->getDataType() == DATA_T::TEXT) {
                    std::cout << TC_GREEN << "*" << TC_DEFAULT << " Type: Text" << std::endl;
                    std::cout << TC_GREEN << "*" << TC_DEFAULT << " Content: ";
                    dataBlock db;
                    while (m_currentPackage->getData(db)) {
                        for (int i = 0; i < db.size; i++) {
                           std::cout << db.data[i];
                        }
                    }
                    std::cout << std::endl;
                } else {
                    std::cout << TC_GREEN << "*" << TC_DEFAULT << " Type: File " << std::endl;
                }
                std::cout << std::endl;
                g_mtx.unlock();
            }
        } else if (nBytes == 0) {
            // Client disconnect from server
            g_mtx.lock();
            auto now = std::chrono::system_clock::now();
            auto time = std::chrono::system_clock::to_time_t(now);

            std::cout << TC_RED << "\r* [" << std::put_time(std::localtime(&time), "%Y-%m-%d %X");
            std::cout << "] ";
            std::cout << "[sid " << m_id << "]" << TC_DEFAULT << " Session complete" << std::endl;
            std::cout << TC_RED << "*" << TC_DEFAULT << " User IP: " << m_ip << std::endl;
            std::cout << TC_RED << "*" << TC_DEFAULT << " User port: " << m_port << std::endl << std::endl;
            g_mtx.unlock();

            if (m_sockfd > 0) {
                #ifdef __linux__
                close(m_sockfd);
                #elif _WIN32
                closesocket(m_sockfd);
                #endif
                m_sockfd = 0;
            }

            m_isStoped = true;
        }

        if (m_isStoped)
            break;
    }
    return true;
}
