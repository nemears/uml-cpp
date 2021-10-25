#include "uml/managers/umlServer.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <thread>
#include <iostream>

using namespace UML;

void UmlServer::acceptNewClients(UmlServer* me) {
    int newSocketD = 0;
    struct addrinfo* clientAddress;
    socklen_t addr_size = sizeof clientAddress;
    while (!(newSocketD = accept(me->m_socketD, (struct sockaddr *)&clientAddress, &addr_size))) {
        me->m_clients[ID::nullID()] = newSocketD;
        std::cout << "connected to client";
    }
}

UmlServer::UmlServer() {
    struct addrinfo hints;
    struct addrinfo* m_address;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me
    if (!getaddrinfo(NULL, std::to_string(UML_PORT).c_str(), &hints, &m_address)) {
        std::cerr << stderr << std::endl;
        throw ManagerStateException();
    }

    // get socket descriptor
    m_socketD = socket(m_address->ai_family, m_address->ai_socktype, m_address->ai_protocol);
    bind(m_socketD, m_address->ai_addr, m_address->ai_addrlen);
    listen(m_socketD, 10);
    std::thread acceptThread(acceptNewClients, this);

}