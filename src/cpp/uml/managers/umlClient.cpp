#include "uml/managers/umlClient.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include "uml/managers/umlServer.h"
#include <iostream>
#include <unistd.h>
#include "yaml-cpp/yaml.h"
#include "uml/parsers/parser.h"

#define UML_CLIENT_MSG_SIZE 200

using namespace UML;

void UmlClient::init() {
    struct addrinfo hints;
    struct addrinfo* myAddress;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me
    int status = 0;
    if ((status = getaddrinfo(m_address.empty() ? 0 : m_address.c_str(), std::to_string(m_port).c_str(), &hints, &myAddress)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        std::cerr << stderr << std::endl;
        throw ManagerStateException("client could not get address!");
    }

    // get socket descriptor
    m_socketD = socket(myAddress->ai_family, myAddress->ai_socktype, myAddress->ai_protocol);
    if (m_socketD == -1) {
        throw ManagerStateException("client could not get socket!");
    }
    if (connect(m_socketD, myAddress->ai_addr, myAddress->ai_addrlen) == -1) {
        throw ManagerStateException("client could not connect to server!");
    }
    freeaddrinfo(myAddress);

    char buff[3];
    int bytesReceived = recv(m_socketD, buff, sizeof buff, 0);
    if (bytesReceived <= 0) {
        throw ManagerStateException();
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
        std::cout << "did not send all bytes!" << std::endl;
        throw ManagerStateException("did not send all bytes!");
    }
    delete[] idMsg;
    char acceptBuff[29];
    if ((bytesReceived = recv(m_socketD, acceptBuff, 29, 0)) <= 0) {
        throw ManagerStateException("did not get accept message!");
    }
    if (id.string().compare(acceptBuff) != 0) {
        throw ManagerStateException("did not get proper accept message!");
    }
}

UmlClient::UmlClient() : id(ID::randomID()) {
    m_port = UML_PORT;
    init();
}

UmlClient::UmlClient(std::string adress) : id(ID::randomID()) {
    bool adressAndPort = false;
    if (adress.find(':')) {
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

UmlClient::~UmlClient() {
    close(m_socketD);
}

Element& UmlClient::get(ID id) {
    YAML::Emitter emitter;
    emitter << YAML::BeginMap << YAML::Key << "GET" << YAML::Value << id.string() << YAML::EndMap;
    int bytesSent;
    while((bytesSent = send(m_socketD, emitter.c_str(), emitter.size() + 1, 0)) <= 0) {
        send(m_socketD, emitter.c_str(), emitter.size() + 1, 0);
    }
    std::cout << "client sent get request to server" << std::endl;
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
    Parsers::ParserMetaData data(this);
    data.m_strategy = Parsers::ParserStrategy::INDIVIDUAL;
    Element& ret = Parsers::parseString(buff, data);
    free(buff);
    return ret;
}

Element& UmlClient::get(std::string qualifiedName) {
    YAML::Emitter emitter;
    emitter << YAML::BeginMap << YAML::Key << "GET" << YAML::Value << qualifiedName << YAML::EndMap;
    int bytesSent;
    while((bytesSent = send(m_socketD, emitter.c_str(), emitter.size() + 1, 0)) <= 0) {
        send(m_socketD, emitter.c_str(), emitter.size() + 1, 0);
    }
    std::cout << "client sent get request to server" << std::endl;
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
    Parsers::ParserMetaData data(this);
    data.m_strategy = Parsers::ParserStrategy::INDIVIDUAL;
    Element& ret = Parsers::parseString(buff, data);
    free(buff);
    return ret;
}

Element& UmlClient::post(ElementType eType) {
    YAML::Emitter emitter;
    emitter << YAML::BeginMap << YAML::Key << "POST" << YAML::Value << Element::elementTypeToString(eType) << YAML::EndMap;
    int bytesSent;
    while ((bytesSent = send(m_socketD, emitter.c_str(), emitter.size() + 1, 0)) <= 0) {
        send(m_socketD, emitter.c_str(), emitter.size() + 1, 0);
    }
    std::cout << "client sent post request to server" << std::endl;
    char buff[100]; // get better sized buffer?
    int bytesReceived = recv(m_socketD, buff, 100, 0);
    std::cout << "client received posted elemnt" << std::endl;
    if (bytesReceived <= 0) {
        throw ManagerStateException();
    }
    Parsers::ParserMetaData data(this);
    data.m_strategy = Parsers::ParserStrategy::INDIVIDUAL;
    return Parsers::parseString(buff, data);
}

void UmlClient::put(Element& el) {
    YAML::Emitter emitter;
    emitter << YAML::BeginMap << YAML::Key << "PUT" << YAML::Value << YAML::BeginMap 
            << YAML::Key << "id" << YAML::Value << el.getID().string();
    if (el.isSubClassOf(ElementType::NAMED_ELEMENT)) {
        if (!el.as<NamedElement>().getQualifiedName().empty()) {
            emitter << YAML::Key << "qualifiedName" << YAML::Value << el.as<NamedElement>().getQualifiedName();
        }
    }
    emitter << YAML::Key << "element" << YAML::Value ;
    Parsers::emitIndividual(el, emitter);
    emitter << YAML::EndMap << YAML::EndMap;
    int bytesSent;
    while ((bytesSent = send(m_socketD, emitter.c_str(), emitter.size() + 1, 0)) <= 0) {
        send(m_socketD, emitter.c_str(), emitter.size() + 1, 0);
    }
    int i = 0;
    while (bytesSent < emitter.size() - (UML_SERVER_MSG_SIZE * i) - 1) {
        bytesSent = send(m_socketD, &emitter.c_str()[1000], emitter.size() - (UML_SERVER_MSG_SIZE - 1), 0);
    }
    std::cout << "client sent put to server" << std::endl;;
    int response, bytesReceived;
    bytesReceived = recv(m_socketD, &response, sizeof(int), 0);
}

void UmlClient::putAll() {
    for (auto& node : m_graph) {
        put(*node.second.m_managerElementMemory);
    }
}

void UmlClient::erase(Element& el) {
    YAML::Emitter emitter;
    emitter << YAML::BeginMap << YAML::Key << "DELETE" << YAML::Value << el.getID().string() << YAML::EndMap;
    UmlManager::erase(el);
    int bytesSent;
    while ((bytesSent = send(m_socketD, emitter.c_str(), emitter.size() + 1, 0)) <= 0) {
        send(m_socketD, emitter.c_str(), emitter.size() + 1, 0);
    }
}

Element* UmlClient::aquire(ID id) {
    return &get(id);
}

void UmlClient::release(Element& el) {
    put(el);
    releaseNode(el);
}

void UmlClient::release(ID id) {
    release(*m_graph[id].m_managerElementMemory);
}

void UmlClient::shutdownServer() {
    const char* msg = "KILL";
    int bytesSent;
    while ((bytesSent = send(m_socketD, msg, 5, 0)) <= 0) {
        send(m_socketD, msg, 5, 0);
    }
}