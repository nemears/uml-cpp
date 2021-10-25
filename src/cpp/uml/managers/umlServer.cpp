#include "uml/managers/umlServer.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <poll.h>
#include <yaml-cpp/yaml.h>
#include "uml/parsers/parser.h"
#include "uml/class.h"

using namespace UML;

void UmlServer::receiveFromClient(UmlServer* me, ID id) {
    struct pollfd pfds[1] = {{me->m_clients[id].socket, POLLIN}};
    while (me->m_running) {
        if (poll(pfds, 1, 1000)) {
            char buff[100];
            int bytesReceived = recv(pfds->fd, buff, 100, 0);
            if (bytesReceived <= 0) {
                continue;
            }
            *me->m_stream << "server got message from client(" + id.string() + "):" << std::endl << buff << std::endl;
            // TODO all
            YAML::Node node = YAML::Load(buff);
            if (node["POST"]) {
                *me->m_stream << "message is post request!" << std::endl;
                ElementType type = Parsers::elementTypeFromString(node["POST"].as<std::string>());
                Element* ret = 0;
                switch (type) {
                    case ElementType::CLASS : {
                        ret = &static_cast<Element&>(me->create<Class>());
                    }
                }
                std::string msg = Parsers::emit(*ret);
                *me->m_stream << msg << std::endl;
                int bytesSent = send(pfds->fd, msg.c_str(), msg.size() + 1, 0);
                if (bytesSent <= 0) {
                    throw ManagerStateException();
                } 
                *me->m_stream << "server created new element" << std::endl;
            }
        }
    }
}

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
        std::thread* clientThread = new std::thread(receiveFromClient, me, ID::fromString(buff));
        me->m_clients[ID::fromString(buff)] = {newSocketD, clientThread};
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
        close(client.second.socket);
        client.second.thread->join();
        delete client.second.thread;
    }
    m_acceptThread->join();
    delete m_acceptThread;
}

int UmlServer::numClients() {
    return m_clients.size();
}