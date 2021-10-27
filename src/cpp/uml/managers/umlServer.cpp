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
#include <chrono>
#include <ctime>

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
    me->log("server set up thread to listen to client " + id.string());
    struct pollfd pfds[1] = {{me->m_clients[id].socket, POLLIN}};
    while (me->m_running) {
        if (poll(pfds, 1, 1000)) { 
            char buff[1000]; // TODO: optimize send in multiple messages instead of taking so much memory each time
            int bytesReceived = recv(pfds->fd, buff, 1000, 0);
            if (bytesReceived <= 0) {
                continue;
            }
            std::unique_lock<std::mutex> mLck(me->m_msgMtx);
            me->log("server got message from client(" + id.string() + "):\n" + std::string(buff));
            me->m_msgCv.wait(mLck, [me]{ return me->m_msgV ? true : false; });
            me->m_msgV = false;
            // TODO all
            YAML::Node node = YAML::Load(buff);
            if (node["DELETE"]) {
                ID elID = ID::fromString(node["DELETE"].as<std::string>());
                {
                    std::lock_guard<std::mutex> elLck(*me->m_locks[elID]);
                    me->log("aquired lock for element " + elID.string());
                    me->m_msgV = true;
                    me->m_msgCv.notify_one();
                    me->erase(elID);
                    me->log("erased element " + elID.string());
                }
                me->m_locks.erase(elID);
                continue;
            }
            if (node["GET"]) {
                ID elID = ID::fromString(node["GET"].as<std::string>());
                std::lock_guard<std::mutex> elLck(*me->m_locks[elID]);
                me->log("aquired lock for element " + elID.string());
                me->m_msgV = true;
                me->m_msgCv.notify_one();
                std::string msg = Parsers::emitIndividual(me->get<>(elID));
                int bytesSent = send(pfds->fd, msg.c_str(), msg.size() + 1, 0);
                if (bytesSent <= 0) {
                    throw ManagerStateException();
                }
                me->log("server got element " +  elID.string() + " for client " + id.string() + ":\n" + msg);
                continue;
            }
            if (node["POST"]) {
                me->m_msgV = true;
                me->m_msgCv.notify_one();
                *me->m_stream << "server handling post request from client " + id.string() <<std::endl;
                ElementType type = Parsers::elementTypeFromString(node["POST"].as<std::string>());
                Element* ret = 0;
                ret = &me->post(type);
                std::string msg = Parsers::emit(*ret);
                int bytesSent = send(pfds->fd, msg.c_str(), msg.size() + 1, 0);
                if (bytesSent <= 0) {
                    throw ManagerStateException();
                } 
                me->log("server created new element for client" + id.string());
                continue;
            }
            if (node["PUT"]) {
                Parsers::ParserMetaData data(me);
                data.m_strategy = Parsers::ParserStrategy::INDIVIDUAL;
                ID elID = ID::fromString(node["PUT"]["id"].as<std::string>());
                std::lock_guard<std::mutex> elLck(*me->m_locks[elID]);
                me->log("aquired lock for element " + elID.string());
                me->m_msgV = true;
                me->m_msgCv.notify_one();
                Parsers::parseYAML(node["PUT"]["element"], data);
                me->log("server put element " + elID.string() + " successfully for client " + id.string());
                continue;
            }
        }
    }
}

void UmlServer::acceptNewClients(UmlServer* me) {
    struct pollfd pfds[1] = {{me->m_socketD, POLLIN}};
    {
        std::lock_guard<std::mutex> rLck(me->m_runMtx);
        while (me->m_running) {
            int newSocketD = -1;
            struct addrinfo* clientAddress;
            socklen_t addr_size = sizeof clientAddress;
            if (!poll(pfds, 1, 1000)) { // TODO: change to ppoll
                continue;
            }
            if (!me->m_running) {
                break;
            }
            std::lock_guard<std::mutex> aLck(me->m_acceptMtx);
            newSocketD = accept(me->m_socketD, (struct sockaddr *)&clientAddress, &addr_size);
            if (newSocketD == -1) {
                if (me->m_running) {
                    me->log("bad socket accepted");
                    throw ManagerStateException();
                } else {
                    continue;
                }
            }
            
            // request ID
            const char* idMsg = "id";
            int len = strlen(idMsg);
            int bytesSent = send(newSocketD, idMsg, len, 0);
            char buff[29];
            int bytesReceived = recv(newSocketD, buff, 29, 0);
            if (bytesReceived <= 0) {
                throw ManagerStateException();
            }
            if (ID::fromString(buff) == me->m_shutdownID) {
                break;
            }
            std::thread* clientThread = new std::thread(receiveFromClient, me, ID::fromString(buff));
            me->m_clients[ID::fromString(buff)] = {newSocketD, clientThread};
        }
        me->m_running = false;
    }
    me->m_runCv.notify_all();
}

void UmlServer::createNode(Element* el) {
    UmlManager::createNode(el);
    m_locks[el->getID()] = new std::mutex;
}

void UmlServer::log(std::string msg) {
    std::lock_guard<std::mutex> lck(m_logMtx);
    auto now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
    std::string nowStr = std::ctime(&nowTime);
    std::cout << "[" + nowStr.substr(0, nowStr.size() - 1) + "]:" + msg << std::endl;
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
    m_running = true;
    m_acceptThread = new std::thread(acceptNewClients, this);
    freeaddrinfo(m_address);
}

UmlServer::~UmlServer() {
    std::unique_lock<std::mutex> mLck(m_msgMtx);
    m_msgCv.wait(mLck, [this]{ return m_msgV ? true : false; });
    log("#####DESTRUCTOR#####");

    // connect to self to stop acceptNewClientsLoop
    bool fail = false;
    struct addrinfo hints;
    struct addrinfo* myAddress;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me
    int status;
    if ((status = getaddrinfo(NULL, std::to_string(UML_PORT).c_str(), &hints, &myAddress)) != 0) {
        // TODO warn on improper shutdown
        *m_stream << "server could not get address info!" << std::endl;
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        std::cerr << stderr << std::endl;
        fail = true;
    }
    int tempSocket = socket(myAddress->ai_family, myAddress->ai_socktype, myAddress->ai_protocol);
    if (tempSocket == -1) {
        /** TODO: failure message **/
        *m_stream << "server could not get socket descriptor!" << std::endl;
        fail = true;
    }
    if (connect(tempSocket, myAddress->ai_addr, myAddress->ai_addrlen) == -1) {
        /** TODO: failure message **/
        *m_stream << "server could not connect to self!" << std::endl;
        fail = true;
    }
    
    // send terminate message
    char* idMsg = new char[29];
    std::string strBuff = m_shutdownID.string();
    std::copy(strBuff.begin(), strBuff.end(), idMsg);
    idMsg[28] = '\0';
    send(tempSocket, idMsg, 29, 0);
    freeaddrinfo(myAddress);
    close(tempSocket);
    delete[] idMsg;

    // wait for thread to stop
    std::unique_lock<std::mutex> rLck(m_runMtx);
    m_runCv.wait(rLck, [this]{ return !m_running; });

    // close everything
    close(m_socketD);
    for (auto client : m_clients) {
        close(client.second.socket);
        client.second.thread->join();
        delete client.second.thread;
    }
    for (auto lock : m_locks) {
        delete lock.second;
    }
    m_acceptThread->join();
    delete m_acceptThread;

   log("#####END_DESTRUCTOR#####");
}

int UmlServer::numClients() {
    std::lock_guard<std::mutex> lck(m_acceptMtx);
    return m_clients.size();
}

bool UmlServer::loaded(ID id) {
    std::unique_lock<std::mutex> mLck(m_msgMtx);
    m_msgCv.wait(mLck, [this]{ return m_msgV ? true : false; });
    if (m_locks.count(id)) {
        std::lock_guard<std::mutex> lck(*m_locks.at(id));
    }
    return UmlManager::loaded(id);
}