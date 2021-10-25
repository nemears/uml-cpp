#include "uml/managers/umlServer.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <poll.h>

using namespace UML;

void UmlServer::acceptNewClients(UmlServer* me) {
    struct pollfd pfds[1] = {{me->m_socketD, POLLIN}};
    while (me->m_running) {
        int newSocketD = -1;
        struct addrinfo* clientAddress;
        socklen_t addr_size = sizeof clientAddress;
        if(poll(pfds, 1, 1000)) {
            newSocketD = accept(me->m_socketD, (struct sockaddr *)&clientAddress, &addr_size);
            if (newSocketD == -1) {
                if (me->m_running) {
                    throw ManagerStateException();
                } else {
                    continue;
                }
            } else {
                *me->m_stream << "connected to client!" << std::endl;
            }
        } else {
            continue; // timeout
        }
        
        // request ID
        const char* idMsg = "id";
        int len = strlen(idMsg);
        int bytesSent = send(newSocketD, idMsg, len, 0);
        *me->m_stream << "server sent request for id from new client" << std::endl;
        char buff[29];
        int bytesReceived = recv(newSocketD, buff, 29, 0);
        if (bytesReceived <= 0) {
            throw ManagerStateException();
        }
        *me->m_stream << "server received id from new client: " << buff << std::endl;
        me->m_clients[ID::fromString(buff)] = newSocketD;
    }
}

UmlServer::UmlServer() {
    m_running = true;
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
    m_running = false;
    close(m_socketD);
    for (auto client : m_clients) {
        close(client.second);
    }
    m_acceptThread->join();
}

int UmlServer::numClients() {
    return m_clients.size();
}