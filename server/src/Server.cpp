#include "Server.h"
#include "Session.h"
#include "TextColor.h"

std::mutex g_mtx;

/************/
my::Server::Server(int argc, char **argv) {
    std::cout << TC_GREEN << "MyServer greeting you!!!" << TC_DEFAULT << std::endl;
//    std::cout << "Comands:" << std::endl;
//    std::cout << "  > stop" << std::endl;
//    std::cout << "  > start" << std::endl;
//    std::cout << "  > stat" << std::endl;
//    std::cout << std::endl;

    // Fill address structure by zeros
    memset(&m_addr, 0, sizeof(m_addr));

    if (argc == 3) {
        if (argv != nullptr) {
            m_ip = argv[1];
            m_port = std::stoi(argv[2]);
        }
    } else {
        std::cerr << TC_RED << "Unexpected args for MyServer" << std::endl;
        std::cerr << "For default will use 127.0.0.1:5000" << TC_DEFAULT << std::endl << std::endl;
        m_ip = "127.0.0.1";
        m_port = 5000;
    }

    #ifdef __linux__
    if (inet_pton(AF_INET, m_ip.c_str(), &(m_addr.sin_addr)) < 1) {
    #elif _WIN32
    if (inet_addr(m_ip.c_str()) == INADDR_NONE) {
    #endif
        std::cerr << TC_RED << "ERROR " << errno << ": Incorrect IP address" << TC_DEFAULT << std::endl;
        exit(errno);
    }

    char buf[100];
    if (gethostname(buf, 100) == 0) {
        m_hostName = std::string(buf, std::string(buf, 100).find('\0'));
    }

    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(m_port);

}

/************/
my::Server::~Server() {
    // Clear sessions
    for (auto &it : m_session) {
        if (it != nullptr)
            delete it;
    }

    // Close socket descriptor
    if (m_sockfd > 0) {
        #ifdef __linux__
        close(m_sockfd);
        #elif _WIN32
        closesocket(m_sockfd);
        #endif

        m_sockfd = 0;
    }

    m_isStoped = true;
    if (m_thread.joinable())
        m_thread.join();
}

/************/
bool my::Server::start() {
    // Create socket
    m_sockfd = socket(m_addr.sin_family, SOCK_STREAM, 0);
    #ifdef __linux__
    if (m_sockfd == -1) {
    #elif _WIN32
    if (m_sockfd == INVALID_SOCKET) {
    #endif
        std::cerr << TC_RED << "ERROR " << errno << ": Can't create socket" << std::endl;
        #ifdef __linux__
        std::cerr << gai_strerror(errno) << TC_DEFAULT << std::endl;
        #endif

        exit(errno);
    }

    // Binding socket and port
    #ifdef __linux__
    if (bind(m_sockfd, reinterpret_cast<sockaddr*>(&m_addr), sizeof(m_addr)) == -1) {
    #elif _WIN32
    if (bind(m_sockfd, reinterpret_cast<sockaddr*>(&m_addr), sizeof(m_addr)) == SOCKET_ERROR) {
    #endif
        std::cerr << TC_RED << "ERROR " << errno << ": Can't bind socket to address" << std::endl;
        #ifdef __linux__
        std::cerr << gai_strerror(errno) << TC_DEFAULT << std::endl;

        close(m_sockfd);
        #elif _WIN32
        closesocket(m_sockfd);
        #endif

        exit(errno);
    }

    // Listen port
    if (listen(m_sockfd, 5) == -1) {
        std::cerr << TC_RED << "ERROR " << errno << ": Can't listen port" << std::endl;
        #ifdef __linux__
        std::cerr << gai_strerror(errno) << TC_DEFAULT << std::endl;
        close(m_sockfd);
        #elif _WIN32
        closesocket(m_sockfd);
        #endif
        exit(errno);
    }

    m_thread = std::thread(&my::Server::init, this);

    g_mtx.lock();
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::cout << TC_BLUE << "* [" << std::put_time(std::localtime(&time), "%Y-%m-%d %X");
    std::cout << "] " << TC_DEFAULT;
    std::cout << "Server run in the thread 0x" << std::hex << m_thread.get_id() << std::dec << std::endl;
    std::cout << TC_BLUE << "*" << TC_DEFAULT << " Host IP: " << m_ip << std::endl;
    std::cout << TC_BLUE << "*" << TC_DEFAULT << " Host port: " << m_port << std::endl;
    std::cout << TC_BLUE << "*" << TC_DEFAULT << " Host name: " << m_hostName << std::endl << std::endl;
    g_mtx.unlock();

	return true;
}

/************/
bool my::Server::signal(const std::string comand) {
    return true;
}

/************/
void my::Server::init() {
    while (1) {
        int clientSockdf;
        socklen_t clientAddrLen;
        struct sockaddr_in clientAddr;

        // Fill address structure by zeros
        memset(&clientAddr, 0, sizeof(clientAddr));
        clientAddrLen = sizeof(clientAddr);
        // Accept new connection
        clientSockdf = accept(m_sockfd, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientAddrLen);
        if (clientSockdf == -1) {
            std::cerr << TC_RED << "ERROR " << errno << ": Can't accept connection" << std::endl;
            #ifdef __linux__
            std::cerr << gai_strerror(errno) << TC_DEFAULT << std::endl;
            #endif
            break;
        }

        m_session.push_back(new Session(clientSockdf, clientAddr));

        if (m_isStoped)
            break;
	}
}
