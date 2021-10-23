#include "uml/managers/distributedManager.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

using namespace UML;



DistributedManager::DistributedManager() {
    struct addrinfo hints;
    struct addrinfo* info;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    if (!getaddrinfo(0, PORT_NUMBER, &hints, &info)) {
        throw ManagerStateException();
    }

    m_serverSocketDescriptor = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
    if (m_serverSocketDescriptor < 0) {
        throw ManagerStateException();
    }

    if (!bind(m_serverSocketDescriptor, info->ai_addr, info->ai_addrlen)) {
        throw ManagerStateException();
    }
    if (!listen(m_serverSocketDescriptor, 20)) {
        throw ManagerStateException();
    }
    
    // TODO


    freeaddrinfo(info);
}

DistributedManager::~DistributedManager() {
    // TODO
}

void DistributedManager::loadFromMount(std::string path) {
    m_path = path;
    // TODO
}