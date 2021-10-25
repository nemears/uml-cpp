#include "uml/managers/umlClient.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include "uml/managers/umlServer.h"

using namespace UML;

UmlClient::UmlClient() {
    struct addrinfo hints;
    struct addrinfo* myAddress;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me
    if (!getaddrinfo(NULL, std::to_string(UML_PORT).c_str(), &hints, &myAddress)) {
        throw ManagerStateException();
    }

    // get socket descriptor
    m_socketD = socket(myAddress->ai_family, myAddress->ai_socktype, myAddress->ai_protocol);
    connect(m_socketD, myAddress->ai_addr, myAddress->ai_addrlen);
}