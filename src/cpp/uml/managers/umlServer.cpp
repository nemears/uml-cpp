#include "uml/managers/umlServer.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <thread>
#include <iostream>
#include <unistd.h>

using namespace UML;

void UmlServer::acceptNewClients(UmlServer* me) {
    int newSocketD = -1;
    struct addrinfo* clientAddress;
    socklen_t addr_size = sizeof clientAddress;
    newSocketD = accept(me->m_socketD, (struct sockaddr *)&clientAddress, &addr_size);
    if (newSocketD == -1) {
        throw ManagerStateException();
    } else {
        std::cout << "connected to client!" << std::endl;
    }
    
    // request ID
    const char* idMsg = "id";
    int len = strlen(idMsg);
    int bytesSent = send(newSocketD, idMsg, len, 0);
    std::cout << "server sent request for id from new client" << std::endl;
    char buff[29];
    int bytesReceived = recv(newSocketD, buff, 29, 0);
    if (bytesReceived <= 0) {
        throw ManagerStateException();
    }
    std::cout << "server received id from new client: " << buff << std::endl;
    me->m_clients[ID::fromString(buff)] = newSocketD;
}

UmlServer::UmlServer() {
    int status;
    struct addrinfo hints;
    struct addrinfo* m_address;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me
    if ((status = getaddrinfo(NULL, std::to_string(UML_PORT).c_str(), &hints, &m_address)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        std::cerr << stderr << std::endl;
        throw ManagerStateException();
    }

    // get socket descriptor
    m_socketD = socket(m_address->ai_family, m_address->ai_socktype, m_address->ai_protocol);
    bind(m_socketD, m_address->ai_addr, m_address->ai_addrlen);
    listen(m_socketD, 10);
    m_acceptThread = new std::thread(acceptNewClients, this);
    freeaddrinfo(m_address);
}

UmlServer::~UmlServer() {
    m_acceptThread->join();
    close(m_socketD);
}