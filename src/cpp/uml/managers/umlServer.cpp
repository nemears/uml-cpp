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

void UmlServer::handleMessage(ID id, std::string buff) {
    ClientInfo& info = m_clients[id];
    log("server got message from client(" + id.string() + "):\n" + std::string(buff));

    // if (strcmp(buff, "KILL") == 0) {
    //     me->m_msgV = true;
    //     me->m_msgCv.notify_one();
    //     info.handlerCv.notify_one();
    //     me->shutdown();
    //     free(buff);
    //     return;
    // }
    
    std::vector<YAML::Node> messages = YAML::LoadAll(buff);
    for (YAML::Node node : messages) {
        if (node["DELETE"]) {
            ID elID = ID::fromString(node["DELETE"].as<std::string>());
            {
                std::lock_guard<std::mutex> elLck(m_locks[elID]);
                log("aquired lock for element " + elID.string());
                m_msgV = true;
                m_msgCv.notify_one();
                erase(elID);
                log("erased element " + elID.string());
            }
            m_locks.erase(elID);
        }
        if (node["GET"]) {
            ID elID;
            if (isValidID(node["GET"].as<std::string>())) {
                elID = ID::fromString(node["GET"].as<std::string>());
            } else {
                elID = m_urls.at(node["GET"].as<std::string>());
            }
            std::lock_guard<std::mutex> elLck(m_locks[elID]);
            log("aquired lock for element " + elID.string());
            m_msgV = true;
            m_msgCv.notify_one();
            std::string msg = Parsers::emitIndividual(get<>(elID));
            int bytesSent = send(info.socket, msg.c_str(), msg.size() + 1, 0);
            if (bytesSent <= 0) {
                throw ManagerStateException();
            }
            log("server got element " +  elID.string() + " for client " + id.string() + ":\n" + msg);
        }
        if (node["POST"]) {
            m_msgV = true;
            m_msgCv.notify_one();
            log("server handling post request from client " + id.string());
            try {
                ElementType type = Parsers::elementTypeFromString(node["POST"].as<std::string>());
                ID id = ID::fromString(node["id"].as<std::string>());
                Element* ret = 0;
                ret = &create(type);
                ret->setID(id);
                log("server created new element for client" + id.string());
            } catch (std::exception& e) {
                log("server could not create new element for client " + id.string() + ", exception with request: " + std::string(e.what()));
            }
        }
        if (node["PUT"]) {
            ID elID = ID::fromString(node["PUT"]["id"].as<std::string>());
            std::lock_guard<std::mutex> elLck(m_locks[elID]);
            log("aquired lock for element " + elID.string());
            m_msgV = true;
            m_msgCv.notify_one();
            if (node["PUT"]["qualifiedName"]) {
                m_urls[node["PUT"]["qualifiedName"].as<std::string>()] = elID;
            }
            Parsers::ParserMetaData data(this);
            data.m_strategy = Parsers::ParserStrategy::INDIVIDUAL;
            try {
                Element& el = Parsers::parseYAML(node["PUT"]["element"], data);
                if (el.isSubClassOf(ElementType::NAMED_ELEMENT)) {
                    m_urls[el.as<NamedElement>().getQualifiedName()] = el.getID();
                }
                log("server put element " + elID.string() + " successfully for client " + id.string());
            } catch (std::exception& e) {
                log("Error parsing PUT request: " + std::string(e.what()));
            }
        }
    }
}

void UmlServer::receiveFromClient(UmlServer* me, ID id) {

    /**
     * TODO: lossless receive, client can rapidfire messages
     **/

    me->log("server set up thread to listen to client " + id.string());
    ClientInfo& info = me->m_clients[id];
    while (me->m_running) {
        int numEvents;
        struct pollfd pfds[1] = {{info.socket, POLLIN}};
        if ((numEvents = poll(pfds, 1, 1000))) {
            // data to read
            // first bytes are size of message
            uint32_t size;
            ssize_t bytesRead = recv(info.socket, &size, sizeof(uint32_t), 0);
            size = ntohl(size);
            size_t sizeSize = sizeof(uint32_t);
            if (bytesRead == 0) {
                // client shutdown
                me->log("error receiving message, lost connection to client");
                return;
            }
            if (bytesRead < 0) {
                // error
                me->log("error receiving message, " + std::string(strerror(errno)));
                continue;
            }

            if (size == 0) {
                me->log("error determining size of message");
                continue;
            }

            // get rest of message
            char* messageBuffer = (char*) malloc(2 * size);
            bytesRead = recv(info.socket, messageBuffer, 2 * size, 0);

            // store message data
            info.threadQueue.push_back(messageBuffer);
            std::lock_guard<std::mutex> handlerLck(info.handlerMtx);

            // check for additional data
            while (bytesRead > size) { // TODO broken
                me->log("received additional data...");
                uint32_t secondSize;
                memcpy(&secondSize, &messageBuffer[size], sizeof(uint32_t));
                secondSize = ntohl(secondSize);
                if (secondSize > bytesRead - size - sizeSize) {
                    me->log("need to receive more");
                    char* tempBuffer = (char*) malloc(bytesRead - size - sizeSize);
                    memcpy(tempBuffer, messageBuffer + size + sizeSize, bytesRead - size - sizeSize);
                    free(messageBuffer);
                    messageBuffer = (char*) malloc(2 * secondSize);
                    memcpy(messageBuffer, tempBuffer, bytesRead - size - sizeSize);
                    free(tempBuffer);
                    bytesRead = size - sizeSize + recv(info.socket, 
                                                       messageBuffer + bytesRead - size - sizeSize, 
                                                       (secondSize * 2) - (bytesRead - size - sizeSize), 
                                                       0);
                } else {
                    messageBuffer = messageBuffer + size + sizeSize;
                    bytesRead = 0;
                }

                // store message data
                info.threadQueue.push_back(messageBuffer);
                size = secondSize;
            }

            // start processing messages
            info.handlerCv.notify_one();
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
            me->log("got id from client: " + std::string(buff));
            ClientInfo& info = me->m_clients[ID::fromString(buff)];
            info.socket = newSocketD;
            info.thread = new std::thread(receiveFromClient, me, ID::fromString(buff));;
            info.handler = new std::thread(clientSubThreadHandler, me, ID::fromString(buff));;
            if (!send(newSocketD, buff, 29, 0)) { // send ID back to say that the server has a thread ready for the client's messages
                throw ManagerStateException("Was not able to send response back to client!");
            }
            me->log("sent id back to client: " + std::string(buff));
        }
        me->m_running = false;
    }
    me->m_runCv.notify_all();
}

void UmlServer::clientSubThreadHandler(UmlServer* me, ID id) {
    ClientInfo& info = me->m_clients[id];
    while (me->m_running) {
        std::unique_lock<std::mutex> lck(info.handlerMtx);
        info.handlerCv.wait(lck, [&info] { return !info.threadQueue.empty(); });
        for (std::string buff : info.threadQueue) {
            if (!buff.empty()) {
                me->handleMessage(id, buff);
            }
        }
        info.threadQueue.clear();
    }
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

UmlServer::UmlServer(int port) : UmlServer() {
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
    // std::unique_lock<std::mutex> mLck(m_msgMtx);
    // m_msgCv.wait(mLck, [this]{ return !m_msgV; });

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
    for (auto& client : m_clients) {
        client.second.thread->join();
        close(client.second.socket);
        delete client.second.thread;
        client.second.threadQueue.push_back("");
        client.second.handlerCv.notify_all();
        client.second.handler->join();
        delete client.second.handler;
    }
    delete m_acceptThread;

    m_shutdownV = true;
    m_shutdownCv.notify_all();
    log("server succesfully shut down");
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
    log("wiating until server idle");
    std::unique_lock<std::mutex> pLck(m_msgMtx);
    m_msgCv.wait(pLck, [this] { return !m_msgV; });
    return 1;
}