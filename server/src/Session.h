#ifndef SESSION_H
#define SESSION_H

#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <chrono>
#include <cstring>
#include <vector>


#ifdef __linux__

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#elif _WIN32

#include <WinSock2.h>
#include <winsock.h>

#endif

#define BLOCK_SIZE 256


namespace my
{

class TAGPackage;

class Session {
public:
    Session(int sockfd, struct sockaddr_in addr);
	virtual ~Session();

    unsigned int getId() const;
private:
	bool init();
private:
    static unsigned int s_genId;
private:
	// Thread in wich runing init() function 
	std::thread m_thread;
    // Flag of working session
	bool m_isStoped {false};
    // Is data receiving or waite
    bool m_isReceive {false};
	// Socket file descriptor
    int m_sockfd {0};
	// User address structure
	struct sockaddr_in m_addr;
    // User IP address
    std::string m_ip;
    // Number of user port
    int m_port;
    // Session id
    unsigned int m_id;
    // Current receiving package
    TAGPackage *m_currentPackage = nullptr;
    std::vector<TAGPackage*> m_packages;
    // Number of received packages
    unsigned int m_nPackages {0};
    // Total received number of bytes
    unsigned int m_totalSize {0};
};
}

#endif // SESSION_H
