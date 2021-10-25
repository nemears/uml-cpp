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
#include "uml/uml-stable.h"

using namespace UML;

Element& UmlServer::post(ElementType eType) {
    switch (eType) {
        case ElementType::ABSTRACTION : {
            return static_cast<Element&>(create<Abstraction>());
        }
        case ElementType::ARTIFACT : {
            return static_cast<Element&>(create<Association>());
        }
        case ElementType::CLASS : {
            return static_cast<Element&>(create<Class>());
        }
        case ElementType::COMMENT : {
            return static_cast<Element&>(create<Comment>());
        }
        case ElementType::DATA_TYPE : {
            return static_cast<Element&>(create<DataType>());
        }
        case ElementType::DEPENDENCY : {
            return static_cast<Element&>(create<Dependency>());
        }
        case ElementType::DEPLOYMENT : {
            return static_cast<Element&>(create<Deployment>());
        }
        case ElementType::ENUMERATION : {
            return static_cast<Element&>(create<Enumeration>());
        }
        case ElementType::ENUMERATION_LITERAL : {
            return static_cast<Element&>(create<EnumerationLiteral>());
        }
        case ElementType::EXPRESSION : {
            return static_cast<Element&>(create<Expression>());
        }
        case ElementType::EXTENSION : {
            return static_cast<Element&>(create<Extension>());
        }
        case ElementType::EXTENSION_END : {
            return static_cast<Element&>(create<ExtensionEnd>());
        }
        case ElementType::GENERALIZATION : {
            return static_cast<Element&>(create<Generalization>());
        }
        case ElementType::INSTANCE_SPECIFICATION : {
            return static_cast<Element&>(create<InstanceSpecification>());
        }
        case ElementType::INSTANCE_VALUE : {
            return static_cast<Element&>(create<InstanceValue>());
        }
        case ElementType::LITERAL_BOOL : {
            return static_cast<Element&>(create<LiteralBool>());
        }
        case ElementType::LITERAL_INT : {
            return static_cast<Element&>(create<LiteralInt>());
        }
        case ElementType::LITERAL_NULL : {
            return static_cast<Element&>(create<LiteralNull>());
        }
        case ElementType::LITERAL_REAL : {
            return static_cast<Element&>(create<LiteralReal>());
        }
        case ElementType::LITERAL_STRING : {
            return static_cast<Element&>(create<LiteralString>());
        }
        case ElementType::LITERAL_UNLIMITED_NATURAL : {
            return static_cast<Element&>(create<LiteralUnlimitedNatural>());
        }
        case ElementType::MANIFESTATION : {
            return static_cast<Element&>(create<Manifestation>());
        }
        case ElementType::MODEL : {
            return static_cast<Element&>(create<Model>());
        }
        case ElementType::OPAQUE_BEHAVIOR : {
            return static_cast<Element&>(create<OpaqueBehavior>());
        }
        case ElementType::OPERATION : {
            return static_cast<Element&>(create<Operation>());
        }
        case ElementType::PACKAGE : {
            return static_cast<Element&>(create<Package>());
        }
        case ElementType::PACKAGE_MERGE : {
            return static_cast<Element&>(create<PackageMerge>());
        }
        case ElementType::PARAMETER : {
            return static_cast<Element&>(create<Parameter>());
        }
        case ElementType::PRIMITIVE_TYPE : {
            return static_cast<Element&>(create<PrimitiveType>());
        }
        case ElementType::PROFILE : {
            return static_cast<Element&>(create<Profile>());
        }
        case ElementType::PROFILE_APPLICATION : {
            return static_cast<Element&>(create<ProfileApplication>());
        }
        case ElementType::PROPERTY : {
            return static_cast<Element&>(create<Property>());
        }
        case ElementType::REALIZATION : {
            return static_cast<Element&>(create<Realization>());
        }
        case ElementType::SLOT : {
            return static_cast<Element&>(create<Slot>());
        }
        case ElementType::STEREOTYPE : {
            return static_cast<Element&>(create<Stereotype>());
        }
        case ElementType::TEMPLATE_BINDING : {
            return static_cast<Element&>(create<TemplateBinding>());
        }
        case ElementType::TEMPLATE_PARAMETER : {
            return static_cast<Element&>(create<TemplateParameter>());
        }
        case ElementType::TEMPLATE_PARAMETER_SUBSTITUTION : {
            return static_cast<Element&>(create<TemplateParameterSubstitution>());
        }
        case ElementType::TEMPLATE_SIGNATURE : {
            return static_cast<Element&>(create<TemplateSignature>());
        }
        case ElementType::USAGE : {
            return static_cast<Element&>(create<Usage>());
        }
        default : {
            throw ManagerStateException();
        }
    }
}

void UmlServer::receiveFromClient(UmlServer* me, ID id) {
    struct pollfd pfds[1] = {{me->m_clients[id].socket, POLLIN}};
    while (me->m_running) {
        if (poll(pfds, 1, 1000)) {
            char buff[1000]; // optimize send in multiple messages instead of taking so much memory each time
            int bytesReceived = recv(pfds->fd, buff, 1000, 0);
            if (bytesReceived <= 0) {
                continue;
            }
            *me->m_stream << "server got message from client(" + id.string() + "):" << std::endl << buff << std::endl;
            // TODO all
            YAML::Node node = YAML::Load(buff);
            if (node["GET"]) {
                ID elID = ID::fromString(node["GET"].as<std::string>());
                std::string msg = Parsers::emitIndividual(me->get<>(elID));
                int bytesSent = send(pfds->fd, msg.c_str(), msg.size() + 1, 0);
                if (bytesSent <= 0) {
                    throw ManagerStateException();
                }
                *me->m_stream << "server got element " +  elID.string() + " for client " + id.string() << std::endl << "msg: " << std::endl << msg << std::endl;
                continue;
            }
            if (node["POST"]) {
                ElementType type = Parsers::elementTypeFromString(node["POST"].as<std::string>());
                Element* ret = 0;
                ret = &me->post(type);
                std::string msg = Parsers::emit(*ret);
                *me->m_stream << msg << std::endl;
                int bytesSent = send(pfds->fd, msg.c_str(), msg.size() + 1, 0);
                if (bytesSent <= 0) {
                    throw ManagerStateException();
                } 
                *me->m_stream << "server created new element for client" + id.string() << std::endl;
                continue;
            }
            if (node["PUT"]) {
                Parsers::ParserMetaData data(me);
                data.m_strategy = Parsers::ParserStrategy::INDIVIDUAL;
                *me->m_stream << "yaml being put into server:\n" + node["PUT"].as<std::string>() << std::endl;
                Parsers::parseString(node["PUT"].as<std::string>(), data);
                continue;
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