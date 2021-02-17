#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>
#include <cstring>

#ifdef __linux__

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#elif _WIN32

#include <WinSock2.h>

#endif

#include "TAGPackage.h"

namespace my
{

class Client {
public:
    Client();
    virtual ~Client();

    bool connectToServer(int argc, char **argv);
    void sendToServer(my::TAGPackage &pack, enum DATA_T type);
private:
    int m_sockfd {0};
    std::string m_ip;
    int m_port;
    // Server address structure
    struct sockaddr_in m_serverAddr;
};
}

#endif // CLIENT_H
