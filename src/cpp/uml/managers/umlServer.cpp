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
#include <errno.h>
#include <string.h>

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
    if (!send(me->m_clients[id].socket, id.string().c_str(), 29, 0)) { // send ID back to say that the server has a thread ready for the client's messages
        throw ManagerStateException("Was not able to send response back to client!");
    }
    while (me->m_running) {
        int numEvents;
        if ((numEvents = poll(pfds, 1, 1000)) > 0) { 
            me->m_processingAV = true;
            bool unlock = false;
            unlock = me->m_processingMtx.try_lock(); // may be better way than directly doing this?
            me->m_processingAV = false;
            for (int msgCount = 0; msgCount < numEvents; msgCount++) {
                char* buff = (char*)malloc(UML_SERVER_MSG_SIZE);
                int bytesReceived = recv(pfds->fd, buff, UML_SERVER_MSG_SIZE, 0);
                if (bytesReceived <= 0) {
                    free(buff);
                    continue;
                }
                int i = 0;
                while (bytesReceived >= UML_SERVER_MSG_SIZE - 1) {
                    if (buff[i*UML_SERVER_MSG_SIZE + UML_SERVER_MSG_SIZE - 1] != '\0') {
                        me->log("did not receive all of the message, waiting for follow up data");
                        buff = (char*)realloc(buff, 2 * UML_SERVER_MSG_SIZE + i * UML_SERVER_MSG_SIZE);
                        bytesReceived = recv(pfds->fd, &buff[UML_SERVER_MSG_SIZE + i * UML_SERVER_MSG_SIZE], UML_SERVER_MSG_SIZE, 0);
                    } else {
                        break;
                    }
                    i++;
                }
                me->log("server got message from client(" + id.string() + "):\n" + std::string(buff));
                std::unique_lock<std::mutex> mLck(me->m_msgMtx);
                me->m_msgCv.wait(mLck, [me]{ return me->m_msgV ? true : false; });
                me->m_msgV = false;
                // TODO all

                if (strcmp(buff, "KILL") == 0) {
                    me->m_msgV = true;
                    me->m_msgCv.notify_one();
                    me->shutdown();
                    free(buff);
                    continue;
                }

                YAML::Node node = YAML::Load(buff);
                if (node["DELETE"]) {
                    ID elID = ID::fromString(node["DELETE"].as<std::string>());
                    {
                        std::lock_guard<std::mutex> elLck(me->m_locks[elID]);
                        me->log("aquired lock for element " + elID.string());
                        me->m_msgV = true;
                        me->m_msgCv.notify_one();
                        me->erase(elID);
                        me->log("erased element " + elID.string());
                    }
                    me->m_locks.erase(elID);
                }
                if (node["GET"]) {
                    ID elID;
                    if (isValidID(node["GET"].as<std::string>())) {
                        elID = ID::fromString(node["GET"].as<std::string>());
                    } else {
                        elID = me->m_urls.at(node["GET"].as<std::string>());
                    }
                    std::lock_guard<std::mutex> elLck(me->m_locks[elID]);
                    me->log("aquired lock for element " + elID.string());
                    me->m_msgV = true;
                    me->m_msgCv.notify_one();
                    std::string msg = Parsers::emitIndividual(me->get<>(elID));
                    int bytesSent = send(pfds->fd, msg.c_str(), msg.size() + 1, 0);
                    if (bytesSent <= 0) {
                        throw ManagerStateException();
                    }
                    me->log("server got element " +  elID.string() + " for client " + id.string() + ":\n" + msg);
                }
                if (node["POST"]) {
                    me->m_msgV = true;
                    me->m_msgCv.notify_one();
                    me->log("server handling post request from client " + id.string());
                    ElementType type = Parsers::elementTypeFromString(node["POST"].as<std::string>());
                    Element* ret = 0;
                    ret = &me->post(type);
                    std::string msg = Parsers::emit(*ret);
                    int bytesSent = send(pfds->fd, msg.c_str(), msg.size() + 1, 0);
                    if (bytesSent <= 0) {
                        free(buff);
                        throw ManagerStateException();
                    } 
                    me->log("server created new element for client" + id.string());
                }
                if (node["PUT"]) {
                    ID elID = ID::fromString(node["PUT"]["id"].as<std::string>());
                    std::lock_guard<std::mutex> elLck(me->m_locks[elID]);
                    me->log("aquired lock for element " + elID.string());
                    me->m_msgV = true;
                    me->m_msgCv.notify_one();
                    if (node["PUT"]["qualifiedName"]) {
                        me->m_urls[node["PUT"]["qualifiedName"].as<std::string>()] = elID;
                    }
                    Parsers::ParserMetaData data(me);
                    data.m_strategy = Parsers::ParserStrategy::INDIVIDUAL;
                    try {
                        Element& el = Parsers::parseYAML(node["PUT"]["element"], data);
                        if (el.isSubClassOf(ElementType::NAMED_ELEMENT)) {
                            me->m_urls[el.as<NamedElement>().getQualifiedName()] = el.getID();
                        }
                        me->log("server put element " + elID.string() + " successfully for client " + id.string());
                    } catch (std::exception& e) {
                        me->log("Error parsing PUT request: " + std::string(e.what()));
                    }
                    
                    int msg = 1, bytesSent = 1;
                    while((bytesSent = send(pfds->fd, &msg, sizeof(int), 0)) <= 0) {
                        bytesSent = send(pfds->fd, &msg, sizeof(int), 0);
                    }
                    me->log("told client we got element put");
                }
                free(buff);
            }
            me->m_processingCv.notify_all();
            if (unlock) {
                me->m_processingMtx.unlock();
            }
        } else if (numEvents == -1) {
            me->log("Server experienced error from poll for client " + id.string());
            throw ManagerStateException("Server experienced error from poll!");
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
            if (!poll(pfds, 1, 1000)) {
                continue;
            }
            if (!me->m_running) {
                break;
            }
            std::lock_guard<std::mutex> aLck(me->m_acceptMtx);
            me->log("server aquired acceptance lock");
            newSocketD = accept(me->m_socketD, (struct sockaddr *)&clientAddress, &addr_size);
            if (newSocketD == -1) {
                if (me->m_running) {
                    me->log("bad socket accepted, error: " + std::string(strerror(errno)));
                    throw ManagerStateException("bad socket accepted");
                } else {
                    continue;
                }
            }
            
            // request ID
            const char* idMsg = "id";
            int bytesSent = send(newSocketD, idMsg, 3, 0);
            if (bytesSent != 3) {
                throw ManagerStateException("could not send id request to new client!");
            }
            char buff[29];
            int bytesReceived = recv(newSocketD, buff, 29, 0);
            if (bytesReceived <= 0) {
                throw ManagerStateException("Did not get proper id from client");
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
    m_locks[el->getID()];
}

void UmlServer::log(std::string msg) {
    std::lock_guard<std::mutex> lck(m_logMtx);
    auto now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
    std::string nowStr = std::ctime(&nowTime);
    std::cout << "[" + nowStr.substr(0, nowStr.size() - 1) + "]:" + msg << std::endl;
}

UmlServer::UmlServer(int port) {
    m_port = port;
}

UmlServer::UmlServer() {
    int status;
    struct addrinfo hints;
    struct addrinfo* m_address;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me
    if ((status = getaddrinfo(NULL, std::to_string(m_port).c_str(), &hints, &m_address)) != 0) {
        throw ManagerStateException("Server could not get address info! error: " + std::string(strerror(errno)));
    }

    // get socket descriptor
    if ((m_socketD = socket(m_address->ai_family, m_address->ai_socktype, m_address->ai_protocol)) == -1) {
        throw ManagerStateException("Server could not get socket from addressinfo, error: " + std::string(strerror(errno)));
    }

    int enable = 1;
    if (setsockopt(m_socketD, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        throw ManagerStateException("Server could not set socket options, error: " + std::string(strerror(errno)));
    }

    if ((status = bind(m_socketD, m_address->ai_addr, m_address->ai_addrlen)) == -1) {
        throw ManagerStateException("Server could not bind to socket, error: " + std::string(strerror(errno)));
    }
    if ((status = listen(m_socketD, 10)) == -1) {
        throw ManagerStateException("Server could not listen to socker, error:" + std::string(strerror(errno)));
    }
    m_running = true;
    m_acceptThread = new std::thread(acceptNewClients, this);
    log("server set up thread to accept new clients");
    freeaddrinfo(m_address);
}

UmlServer::~UmlServer() {
    if (m_running) {
        shutdown();
    }
}

int UmlServer::numClients() {
    std::lock_guard<std::mutex> lck(m_acceptMtx);
    return m_clients.size();
}

bool UmlServer::loaded(ID id) {
    std::unique_lock<std::mutex> mLck(m_msgMtx);
    m_msgCv.wait(mLck, [this]{ return m_msgV ? true : false; });
    if (m_locks.count(id)) {
        std::lock_guard<std::mutex> lck(m_locks.at(id));
    }
    return UmlManager::loaded(id);
}

void UmlServer::reset() {
    log("server resetting");
    clear();
}

void UmlServer::shutdown() {
    log("server shutting down");

    // wait for processing message traffic to end
    std::unique_lock<std::mutex> mLck(m_msgMtx);
    m_msgCv.wait(mLck, [this]{ return m_msgV ? true : false; });

    // connect to self to stop acceptNewClientsLoop
    bool fail = false;
    struct addrinfo hints;
    struct addrinfo* myAddress;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me
    int status;
    if ((status = getaddrinfo(NULL, std::to_string(m_port).c_str(), &hints, &myAddress)) != 0) {
        // TODO warn on improper shutdown
        log("server could not get address info!");
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        std::cerr << stderr << std::endl;
        fail = true;
    }
    int tempSocket;
    if (!fail) {
        tempSocket = socket(myAddress->ai_family, myAddress->ai_socktype, myAddress->ai_protocol);
        if (tempSocket == -1) {
            /** TODO: failure message **/
            log("server could not get socket descriptor!");
            fail = true;
        }
    }
    if (!fail) {
        if (connect(tempSocket, myAddress->ai_addr, myAddress->ai_addrlen) == -1) {
            /** TODO: failure message **/
            log("server could not connect to self!");
            fail = true;
        }
    }
    
    if (!fail) {
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
        m_acceptThread->join();
    }

    // close everything
    close(m_socketD);
    for (auto client : m_clients) {
        client.second.thread->join();
        close(client.second.socket);
        delete client.second.thread;
    }
    delete m_acceptThread;

    m_shutdownV = true;
    m_shutdownCv.notify_all();
}

using namespace std::chrono_literals;

int UmlServer::waitTillShutDown(int ms) {
    std::unique_lock<std::mutex> sLck(m_shutdownMtx);
    m_shutdownCv.wait_for(sLck, ms * 1ms, [this] { return m_shutdownV; });
    return 1;
}

int UmlServer::waitTillShutDown() {
    std::unique_lock<std::mutex> sLck(m_shutdownMtx);
    m_shutdownCv.wait(sLck, [this] { return m_shutdownV; });
    return 1;
}

int UmlServer::waitForProcessing() {
    std::unique_lock<std::mutex> pLck(m_processingMtx);
    m_processingCv.wait(pLck, [this] { return !m_processingAV; });
    m_processingCv.notify_all();
    return 1;
}