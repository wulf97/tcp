#include "Client.h"
#include "TextColor.h"

my::Client::Client() {
    // Fill server address structure by zeros
    memset(&m_serverAddr, 0, sizeof(m_serverAddr));
}

my::Client::~Client() {
    // Close socket descriptor
    if (m_sockfd > 0) {
        #ifdef __linux__
        close(m_sockfd);
        #elif _WIN32
        closesocket(m_sockfd);
        #endif

        m_sockfd = 0;
    }
}

bool my::Client::connectToServer(int argc, char **argv) {
    if (argc == 3) {
        if (argv != nullptr) {
            m_ip = argv[1];
            m_port = std::stoi(argv[2]);
        }
    } else {
        std::cout << TC_RED << "\rIncorrect args" << TC_DEFAULT << std::endl;
        return false;
    }

    if (inet_pton(AF_INET, m_ip.c_str(), &(m_serverAddr.sin_addr)) < 1) {
        std::cerr << TC_RED << "ERROR " << errno << ": Incorrect IP address" << TC_DEFAULT << std::endl;
        exit(errno);
    }

    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_port = htons(m_port);

    m_sockfd = socket(m_serverAddr.sin_family, SOCK_STREAM, 0);
    if (m_sockfd == -1) {
        std::cerr << TC_RED << "ERROR " << errno << ": Can't create socket" << std::endl;
        std::cerr << gai_strerror(errno) << TC_DEFAULT << std::endl;

        exit(errno);
    }

    if (connect(m_sockfd, reinterpret_cast<sockaddr*>(&m_serverAddr), sizeof(m_serverAddr)) == -1) {
        std::cerr << TC_RED << "ERROR " << errno << ": Can't connect to server" << std::endl;
        std::cerr << gai_strerror(errno) << TC_DEFAULT << std::endl;

        #ifdef __linux__
        close(m_sockfd);
        #elif _WIN32
        closesocket(m_sockfd);
        #endif

        exit(errno);
    }

    return true;
}

void my::Client::sendToServer(my::TAGPackage &pack, enum DATA_T type) {
    dataBlock dBlock;

    while (pack.getData(dBlock)) {
        send(m_sockfd, &dBlock.data, dBlock.size, 0);
    }
}
