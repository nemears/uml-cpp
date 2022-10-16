#include "uml/managers/abstractClient.h"
#ifndef WIN32
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <poll.h>
#include <unistd.h>
#include <cstring>
#include <netinet/tcp.h>
#else
#endif
#include "uml/managers/abstractManager.h"
#include <iostream>
#include "uml/managers/umlServer.h"
#include "uml/uml-stable.h"

#define UML_CLIENT_MSG_SIZE 200

using namespace UML;

void sendEmitter(socketType socket, YAML::Emitter& emitter) {
    int totalBytesSent = 0;
    int bytesSent = 0;
    int packetSize = sizeof(uint32_t) + sizeof(char) * (emitter.size() + 1);
    char* packet = (char*) malloc(packetSize);
    uint32_t sizeOfEmitter = htonl(emitter.size() + 1);
    memcpy(packet, &sizeOfEmitter, sizeof(uint32_t));
    strcpy(packet + sizeof(uint32_t), emitter.c_str());
    while ((bytesSent = send(socket, packet + totalBytesSent, packetSize - totalBytesSent, 0)) < packetSize - totalBytesSent) {
        if (bytesSent == -1) {
            throw ManagerStateException("could not send packet to server: " + std::string(strerror(errno)));
        } else {
            totalBytesSent += bytesSent;
        }
    }
    free(packet);
}

#ifdef UML_DEBUG
std::string AbstractClient::time_in_HH_MM_SS_MMM()
{
    using namespace std::chrono;

    // get current time
    auto now = system_clock::now();

    // get number of milliseconds for the current second
    // (remainder after division into seconds)
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    // convert to std::time_t in order to convert to std::tm (broken time)
    auto timer = system_clock::to_time_t(now);

    // convert to broken time
    std::tm bt = *std::localtime(&timer);

    std::ostringstream oss;

    oss << std::put_time(&bt, "%H:%M:%S"); // HH:MM:SS
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

    return oss.str();
}

#endif

void AbstractClient::init() {
    #ifndef WIN32
    struct addrinfo hints;
    struct addrinfo* myAddress;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = m_address.empty() ? AI_PASSIVE : AI_CANONNAME; // fill in my IP for me
    int status = 0;
    if ((status = getaddrinfo(m_address.empty() ? 0 : m_address.c_str(), std::to_string(m_port).c_str(), &hints, &myAddress)) != 0) {
        throw ManagerStateException("client could not get address! " + std::string(strerror(errno)));
    }

    // get socket descriptor
    m_socketD = socket(myAddress->ai_family, myAddress->ai_socktype, myAddress->ai_protocol);
    if (m_socketD == -1) {
        freeaddrinfo(myAddress);
        throw ManagerStateException("client could not get socket!");
    }
    if (connect(m_socketD, myAddress->ai_addr, myAddress->ai_addrlen) == -1) {
        freeaddrinfo(myAddress);
        throw ManagerStateException("client could not connect to server! " + std::string(strerror(errno)));
    }
    freeaddrinfo(myAddress);
    
    // disable Nagle's on client side for no latency, client tends to write write read which is bad and it is hard to bundle writes together 
    // since is controlled by user of api
    int yes = 1;
    int result = setsockopt(m_socketD, IPPROTO_TCP, TCP_NODELAY, (char*) &yes, sizeof(int));
    if (result < 0) {
        throw ManagerStateException("could not disable Nagle's algorithm on client side!");
    }
    #else
    int status = WSAStartup(MAKEWORD(2,2), &m_wsaData);
    if (status != 0) {
        throw ManagerStateException("TOSO Winsock, WSAStartup");
    }
    struct addrinfo *result = 0,
                hints;
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    status = getaddrinfo(m_address.empty() ? 0 : m_address.c_str(), std::to_string(m_port).c_str(), &hints, &result);
    if (status != 0) {
        WSACleanup();
        throw ManagerStateException("TODO winsock 1");
    }
    m_socketD = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (m_socketD == INVALID_SOCKET) {
        freeaddrinfo(result);
        WSACleanup();
        throw ManagerStateException("TODO winsock could not bind to socket");
    }
    status = connect(m_socketD, result->ai_addr, (int) result->ai_addrlen);
    freeaddrinfo(result);
    if (status == SOCKET_ERROR) {
        closesocket(m_socketD);
        m_socketD = INVALID_SOCKET;
        WSACleanup();
        throw ManagerStateException("TODO winsock could not connect to server!");
    }
    #endif
    char buff[3];
    int bytesReceived = 0;
    while ((bytesReceived += recv(m_socketD, buff + bytesReceived, 3 - bytesReceived, 0)) < 3) {
        if (bytesReceived <= 0) {
            throw ManagerStateException();
        }
    }
    if (buff[2] != '\0') {
        char buff2[4] = {buff[0], buff[1], buff[2], '\0'};
        throw ManagerStateException("Invalid connection response from server, message (might be garbage): " + std::string(buff2));
    }
    if (std::string("id").compare(buff) != 0) {
        throw ManagerStateException();
    }
    char* idMsg = new char[29];
    std::string strBuff = id.string();
    std::copy(strBuff.begin(), strBuff.end(), idMsg);
    idMsg[28] = '\0';
    int bytesSent = send(m_socketD, idMsg, 29, 0);
    if (bytesSent != 29) {
        throw ManagerStateException("did not send all bytes!");
    }
    delete[] idMsg;
    char acceptBuff[29];
    bytesReceived = 0;
    while ((bytesReceived += recv(m_socketD, acceptBuff + bytesReceived, 29 - bytesReceived, 0)) < 29) {
            // if (bytesReceived == 0) {
            //     throw ManagerStateException("did not get accept message!");
            // } else {
            //     throw ManagerStateException("Error reciving acceptance message: " + std::string(strerror(errno)));
            // }
    }
    if (id.string().compare(acceptBuff) != 0) {
        throw ManagerStateException("did not get proper accept message!");
    }
    #ifdef UML_DEBUG
    std::cout << time_in_HH_MM_SS_MMM() << ": client initialized" << std::endl;
    #endif
}

AbstractClient::AbstractClient() : id(ID::randomID()) {
    m_port = UML_PORT;
    init();
}

AbstractClient::AbstractClient(std::string adress) : id(ID::randomID()) {
    bool adressAndPort = false;
    if (adress.find(':') != std::string::npos) {
        for (char& c : adress.substr(adress.find_last_of(':'))) {
            adressAndPort = std::isdigit(c);
            if (adressAndPort) {
                break;
            }
        }
    } 
    if (adressAndPort) {
        m_address = adress.substr(0, adress.find_last_of(':'));
        m_port = atoi(adress.substr(adress.find_last_of(':') + 1).c_str());
    } else {
        m_address = adress;
        m_port = UML_PORT;
    }
    init();
}

AbstractClient::~AbstractClient() {
    #ifndef WIN32
    close(m_socketD);
    #ifdef UML_DEBUG
    std::cout << time_in_HH_MM_SS_MMM() << ": client destroyed" << std::endl;
    #endif
    #else
    int result = shutdown(m_socketD, SD_SEND);
    if (result == SOCKET_ERROR) {
        closesocket(m_socketD);
        WSACleanup();
    }
    #endif
}

void AbstractClient::createElForServer(Element& el) {
    YAML::Emitter emitter;
    emitter << YAML::DoubleQuoted  << YAML::Flow << YAML::BeginMap << 
        YAML::Key << "POST" << YAML::Value << Element::elementTypeToString(el.getElementType()) << 
        YAML::Key << "id" << YAML::Value << el.getID().string() <<
    YAML::EndMap;
    sendEmitter(m_socketD, emitter);
    #ifdef UML_DEBUG
    std::cout << time_in_HH_MM_SS_MMM() << ": client created element of id " << el.getID().string() << std::endl;
    #endif
}

ElementPtr AbstractClient::getElFromServer(ID id, AbstractManager* me) {
    YAML::Emitter emitter;
    emitter << YAML::DoubleQuoted  << YAML::Flow << YAML::BeginMap << 
        YAML::Key << "GET" << YAML::Value << id.string() << 
    YAML::EndMap;
    sendEmitter(m_socketD, emitter);
    #ifdef UML_DEBUG
    std::cout << time_in_HH_MM_SS_MMM() << ": client sent get reqiest for el of id " << id.string() << std::endl;
    #endif
    char* buff = (char*)malloc(UML_CLIENT_MSG_SIZE);
    int bytesReceived = recv(m_socketD, buff, UML_CLIENT_MSG_SIZE, 0);
    if (bytesReceived <= 0) {
        throw ManagerStateException();
    }
    int i = 0;
    while (bytesReceived >= UML_CLIENT_MSG_SIZE - 1) {
        if (buff[i * UML_CLIENT_MSG_SIZE + UML_CLIENT_MSG_SIZE - 1] != '\0') {
            buff = (char*)realloc(buff, 2 * UML_CLIENT_MSG_SIZE + i * UML_CLIENT_MSG_SIZE);
            bytesReceived = recv(m_socketD, &buff[UML_CLIENT_MSG_SIZE + i * UML_CLIENT_MSG_SIZE], UML_CLIENT_MSG_SIZE, 0);
        } else {
            break;
        }
        i++;
    }
    Parsers::ParserMetaData data;
    data.m_manager = me;
    if (std::string("ERROR").compare(buff) == 0) {
        free(buff);
        throw ManagerStateException("ERROR from server!");
    }
    data.m_strategy = Parsers::ParserStrategy::INDIVIDUAL;
    Element& ret = *Parsers::parseString(buff, data);
    free(buff);
    // for (auto& pair : ret.m_node->m_references) {
    //     if (!pair.second.node && exists(pair.first)) {
    //         pair.second.node = &m_graph[pair.first];
    //     }
    //     if (!pair.second.node || !pair.second.node->m_managerElementMemory) {
    //         // element has been released, possibly there are no pointers
    //         continue;
    //     }
    //     if (pair.second.node->m_references.count(id)) {
    //         pair.second.node->restoreReference(&ret);
    //     } else {
    //         pair.second.node->setReference(ret);
    //     }
    //     ret.m_node->restoreReference(pair.second.node->m_managerElementMemory);
    // }
    // SimpleAccessPolicy::restoreNode(ret.m_node);
    #ifdef UML_DEBUG
    std::cout << time_in_HH_MM_SS_MMM() << ": client got el of id " << id.string() << std::endl;
    #endif
    return ElementPtr(&ret);
}

ElementPtr AbstractClient::getElFromServer(std::string qualifiedName, AbstractManager* me) {
    YAML::Emitter emitter;
    emitter << YAML::DoubleQuoted  << YAML::Flow << YAML::BeginMap << 
        YAML::Key << "GET" << YAML::Value << qualifiedName << 
    YAML::EndMap;
    sendEmitter(m_socketD, emitter);
    char* buff = (char*)malloc(UML_CLIENT_MSG_SIZE);
    int bytesReceived = recv(m_socketD, buff, UML_CLIENT_MSG_SIZE, 0);
    if (bytesReceived <= 0) {
        throw ManagerStateException();
    }
    int i = 0;
    while (bytesReceived >= UML_CLIENT_MSG_SIZE - 1) {
        if (buff[i * UML_CLIENT_MSG_SIZE + UML_CLIENT_MSG_SIZE - 1] != '\0') {
            buff = (char*)realloc(buff, 2 * UML_CLIENT_MSG_SIZE + i * UML_CLIENT_MSG_SIZE);
            bytesReceived = recv(m_socketD, &buff[UML_CLIENT_MSG_SIZE + i * UML_CLIENT_MSG_SIZE], UML_CLIENT_MSG_SIZE, 0);
        } else {
            break;
        }
        i++;
    }
    Parsers::ParserMetaData data;
    data.m_manager = me;
    data.m_strategy = Parsers::ParserStrategy::INDIVIDUAL;
    Element& ret = *Parsers::parseString(buff, data);
    free(buff);
    #ifdef UML_DEBUG
    std::cout << time_in_HH_MM_SS_MMM() << ": client got el of name " << qualifiedName << std::endl;
    #endif
    return ElementPtr(&ret);
}

void AbstractClient::writeToServer(Element& el) {
    YAML::Emitter emitter;
    emitter << YAML::DoubleQuoted  << YAML::Flow << YAML::BeginMap << 
        YAML::Key << "PUT" << YAML::Value << YAML::BeginMap 
        << YAML::Key << "id" << YAML::Value << el.getID().string();
        if (el.isSubClassOf(ElementType::NAMED_ELEMENT)) {
            if (!el.as<NamedElement>().getQualifiedName().empty()) {
                emitter << YAML::Key << "qualifiedName" << YAML::Value << el.as<NamedElement>().getQualifiedName();
            }
        }
        emitter << YAML::Key << "element" << YAML::Value ;
        Parsers::emitIndividual(el, emitter);
    emitter << YAML::EndMap << YAML::EndMap;
    sendEmitter(m_socketD, emitter);
    #ifdef UML_DEBUG
    std::cout << time_in_HH_MM_SS_MMM() << ": client put el of id " << el.getID().string() << std::endl;
    #endif
}

void AbstractClient::eraseFromServer(Element& el) {
    YAML::Emitter emitter;
    emitter << YAML::DoubleQuoted  << YAML::Flow << YAML::BeginMap << 
        YAML::Key << "DELETE" << YAML::Value << el.getID().string() << 
    YAML::EndMap;
    sendEmitter(m_socketD, emitter);
    #ifdef UML_DEBUG
    std::cout << time_in_HH_MM_SS_MMM() << ": client erased el" << std::endl;
    #endif
}

void AbstractClient::saveToServer() {
    YAML::Emitter emitter;
    emitter << YAML::DoubleQuoted  << YAML::Flow << YAML::BeginMap;
        emitter << YAML::Key << "SAVE" << YAML::Value << ".";
    emitter << YAML::EndMap;
    sendEmitter(m_socketD, emitter);
}

void AbstractClient::saveToServer(std::string path) {
    YAML::Emitter emitter;
    emitter << YAML::DoubleQuoted  << YAML::Flow << YAML::BeginMap;
        emitter << YAML::Key << "SAVE" << YAML::Value << path;
    emitter << YAML::EndMap;
    sendEmitter(m_socketD, emitter);
}

void AbstractClient::setRootForServer(Element* el) {
    if (el) {
        YAML::Emitter emitter;
        emitter << YAML::DoubleQuoted  << YAML::Flow << YAML::BeginMap << 
            YAML::Key << "PUT" << YAML::Value << YAML::BeginMap 
            << YAML::Key << "id" << YAML::Value << el->getID().string();
            if (el->isSubClassOf(ElementType::NAMED_ELEMENT)) {
                emitter << YAML::Key << "qualifiedName" << YAML::Value << "";
            }
            emitter << YAML::Key << "element" << YAML::Value ;
            Parsers::emitIndividual(*el, emitter);
        emitter << YAML::EndMap << YAML::EndMap;
        sendEmitter(m_socketD, emitter);
    } else {
        // TODO
    }
}