#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <chrono>
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
#include <wsipv6ok.h>

#endif

namespace my
{

class Session;

class Server {
public:
    Server(int argc = 0, char **argv = nullptr);
	virtual ~Server();

    // Begin server process
	bool start();
    bool signal(const std::string comand);
private:
    // Init main server thread
    void init();
private:
    // Main server thread in wich listen new connections
	std::thread m_thread;
    // Check if server was stoped
	bool m_isStoped {false};
    // List of active sessions
    std::vector<Session*> m_session;
    // Socket descriptor
    int m_sockfd {0};
    // Host address structure
    struct sockaddr_in m_addr;
    // Host IP address
    std::string m_ip;
    // Host name
    std::string m_hostName;
    // Number of port wich listen server
    int m_port;
};
}

#endif // SERVER_H
