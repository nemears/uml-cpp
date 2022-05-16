#include "uml/managers/umlServer.h"
#ifndef WIN32
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <iostream>
#include <unistd.h>
#include <poll.h>
#else
#include <ws2tcpip.h>
#include <stdio.h>
#endif
#include <thread>
#include <yaml-cpp/yaml.h>
#include "uml/parsers/parser.h"
#include "uml/uml-stable.h"
#include <chrono>
#include <ctime>
#include <errno.h>
#include <string.h>

#ifdef WIN32
typedef size_t ssize_t;
#endif

using namespace UML;

void UmlServer::handleMessage(ID id, std::string buff) {
    ClientInfo& info = m_clients[id];
    log("server got message from client(" + id.string() + "):\n" + std::string(buff));

    if (buff == "KILL") {
        m_msgV = true;
        m_msgCv.notify_one();
        info.handlerCv.notify_one();
        shutdownServer();
        return;
    }
    
    YAML::Node node;
    try {
        node = YAML::Load(buff);
    } catch (std::exception& e) {
        log(e.what());
        const char* msg = "ERROR";
        int bytesSent = send(info.socket, msg, 6, 0);
        return;
    }
    
    if (!node.IsMap()) {
        log("ERROR receiving message from client, invalid format!\nMessage:\n" + buff);
        const char* msg = "ERROR";
        int bytesSent = send(info.socket, msg, 6, 0);
        return;
    }
    if (node["DELETE"]) {
        ID elID = ID::fromString(node["DELETE"].as<std::string>());
        try {
            {
                std::lock_guard<std::mutex> elLck(m_locks[elID]);
                log("aquired lock for element " + elID.string());
                m_msgV = true;
                m_msgCv.notify_one();
                erase(elID);
            }
            m_locks.erase(elID);
            log("erased element " + elID.string());
            std::lock_guard<std::mutex> garbageLck(m_garbageMtx);
            m_releaseQueue.remove(elID);
            m_numEls--;
        } catch (std::exception& e) {
            log("exception encountered when trying to delete element: " + std::string(e.what()));
        }
    } else if (node["GET"]) {
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
        try {
            Element& el = get(elID);
            Parsers::EmitterMetaData data = Parsers::getData(el);
            data.m_isJSON = true;
            std::string msg = Parsers::emitString(data, el);
            int bytesSent = send(info.socket, msg.c_str(), msg.size() + 1, 0);
            if (bytesSent <= 0) {
                throw ManagerStateException();
            }
            log("server got element " +  elID.string() + " for client " + id.string() + ":\n" + msg);
        } catch (std::exception e) {
            log(e.what());
            const char* msg = "ERROR";
            int bytesSent = send(info.socket, msg, 6, 0);
        }
    } else if (node["POST"]) {
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
            std::lock_guard<std::mutex> garbageLck(m_garbageMtx);
            m_releaseQueue.push_front(id);
            m_garbageCv.notify_one();
        } catch (std::exception& e) {
            log("server could not create new element for client " + id.string() + ", exception with request: " + std::string(e.what()));
        }
    } else if (node["PUT"]) {
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
            Element& el = *Parsers::parseYAML(node["PUT"]["element"], data);
            if (el.isSubClassOf(ElementType::NAMED_ELEMENT)) {
                m_urls[el.as<NamedElement>().getQualifiedName()] = el.getID();
            }
            log("server put element " + elID.string() + " successfully for client " + id.string());
        } catch (std::exception& e) {
            log("Error parsing PUT request: " + std::string(e.what()));
        }
    } else {
        log("ERROR receiving message from client, invalid format!\nMessage:\n" + buff);
        const char* msg = "ERROR";
        int bytesSent = send(info.socket, msg, 6, 0);
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
        #ifndef WIN32
        if ((numEvents = poll(pfds, 1, 1000))) {
        #else
        if ((numEvents = WSAPoll(pfds, 1, 1000))) {
        #endif
            // data to read
            // first bytes are size of message
            uint32_t size;
            ssize_t bytesRead = recv(info.socket, (char*)&size, sizeof(uint32_t), 0);
            size = ntohl(size);
            size_t sizeSize = sizeof(uint32_t);
            if (bytesRead == 0) {
                // client shutdown
                me->log("error receiving message, lost connection to client");
                std::lock_guard<std::mutex> zombieLck(me->m_zombieMtx);
                me->m_zombies.push_back(id);
                me->m_zombieCv.notify_one();
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

            if (messageBuffer == 0) {
                me->log("error receiving message, could not allocate memory for message of size " + std::to_string(size));
                continue;
            }

            bytesRead = recv(info.socket, messageBuffer, 2 * size, 0);

            // store message data
            info.threadQueue.push_back(messageBuffer);
            std::lock_guard<std::mutex> handlerLck(info.handlerMtx);

            // check for additional data
            while (bytesRead > size) { // TODO test
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
            socketType newSocketD = 
            #ifndef WIN32
            0;
            #else
            INVALID_SOCKET;
            #endif
            struct addrinfo* clientAddress;
            socklen_t addr_size = sizeof clientAddress;
            #ifndef WIN32
            if (!poll(pfds, 1, 1000)) {
            #else
            if (!WSAPoll(pfds, 1,1000)) {
            #endif
                continue;
            }
            if (!me->m_running) {
                break;
            }
            std::lock_guard<std::mutex> aLck(me->m_acceptMtx);
            me->log("server aquired acceptance lock");
            #ifndef WIN32
            newSocketD = accept(me->m_socketD, (struct sockaddr *)&clientAddress, &addr_size);
            if (newSocketD == -1) {
                if (me->m_running) {
                    me->log("bad socket accepted, error: " + std::string(strerror(errno)));
                    throw ManagerStateException("bad socket accepted");
                } else {
                    continue;
                }
            }
            #else
            newSocketD = accept(me->m_socketD, 0, 0);
            if (newSocketD == INVALID_SOCKET) {
                if (me->m_running) {
                    closesocket(newSocketD);
                    WSACleanup();
                    me->log("bad socket accepted, error: " + std::string(strerror(errno)));
                    throw ManagerStateException("bad socket accepted");
                } else {
                    closesocket(newSocketD);
                    WSACleanup();
                    continue;
                }
            }
            #endif
            
            // request ID
            const char* idMsg = "id";
            int bytesSent = send(newSocketD, idMsg, 3, 0);
            if (bytesSent != 3) {
                me->log("wcould not send id request to new client! error:" + std::string(strerror(errno)));
                #ifdef WIN32
                closesocket(newSocketD);
                WSACleanup();
                #endif
                throw ManagerStateException("could not send id request to new client!");
            }
            char buff[29];
            int bytesReceived = recv(newSocketD, buff, 29, 0);
            if (bytesReceived <= 0) {
                #ifdef WIN32
                closesocket(newSocketD);
                WSACleanup();
                #endif
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
                #ifdef WIN32
                closesocket(newSocketD);
                WSACleanup();
                #endif
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
    while (me->m_running && info.thread) {
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

void UmlServer::garbageCollector(UmlServer* me) {
    while(me->m_running) {
        std::unique_lock<std::mutex> garbageLck(me->m_garbageMtx);
        me->m_garbageCv.wait(garbageLck, [me] { return me->m_releaseQueue.size() != me->m_numEls; });
        if (me->m_numEls == me->m_maxEls) {
            ID releasedID = me->m_releaseQueue.back();
            me->release(releasedID);
            me->m_releaseQueue.pop_back();
            me->m_locks.erase(releasedID);
        } else {
            me->m_numEls++;
        }
    }
}

void UmlServer::closeClientConnections(ClientInfo& client) {
    client.thread->join();
    #ifndef WIN32
    close(client.socket);
    #else
    int result = shutdown(client.socket, SD_SEND);
    if (result == SOCKET_ERROR) {
        closesocket(client.socket);
        WSACleanup();
    }
    #endif
    delete client.thread;
    client.thread = 0;
    client.threadQueue.push_back("");
    client.handlerCv.notify_all();
    client.handler->join();
    delete client.handler;
}

void UmlServer::zombieKiller(UmlServer* me) {
    while(me->m_running) {
        std::unique_lock<std::mutex> zombieLck(me->m_zombieMtx);
        me->m_zombieCv.wait(zombieLck, [me] { return !me->m_zombies.empty(); });
        for (const ID id : me->m_zombies) {
            ClientInfo& client = me->m_clients[id];
            me->closeClientConnections(client);
            me->m_clients.erase(id);
        }
        me->m_zombies.clear();
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

UmlServer::UmlServer(int port) {
    m_port = port;
    int status;
    #ifndef WIN32
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
    freeaddrinfo(m_address);
    #else
    status = WSAStartup(MAKEWORD(2,2), &m_wsaData);
    if (status != 0) {
        throw ManagerStateException("TOSO Winsock, WSAStartup");
    }
    struct addrinfo *result = 0, *ptr = 0, hints;
    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    status = getaddrinfo(0, std::to_string(port).c_str(), &hints, &result);
    if (status != 0) {
        WSACleanup();
        throw ManagerStateException("TODO winsock getaddrinfo");
    }
    m_socketD = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (m_socketD == INVALID_SOCKET) {
        freeaddrinfo(result);
        WSACleanup();
        throw ManagerStateException("TOSO winsock socket");
    }
    status = bind(m_socketD, result->ai_addr, (int) result->ai_addrlen);
    freeaddrinfo(result);
    if (status == SOCKET_ERROR) {
        closesocket(m_socketD);
        WSACleanup();
        throw ManagerStateException("TODO winsock bind");
    }
    status = listen(m_socketD, SOMAXCONN);
    if (status == SOCKET_ERROR) {
        closesocket(m_socketD);
        WSACleanup();
        throw ManagerStateException("TODO winsock listen");
    }
    #endif

    m_running = true;
    m_acceptThread = new std::thread(acceptNewClients, this);
    m_garbageCollectionThread = new std::thread(garbageCollector, this);
    m_zombieKillerThread = new std::thread(zombieKiller, this);
    log("server set up thread to accept new clients");
}

UmlServer::UmlServer() : UmlServer(UML_PORT) {
    
}

UmlServer::~UmlServer() {
    if (m_running) {
        shutdownServer();
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

void UmlServer::shutdownServer() {
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
    socketType tempSocket;
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
        #ifndef WIN32
        close(tempSocket);
        #else
        int result = shutdown(tempSocket, SD_SEND);
        if (result == SOCKET_ERROR) {
            closesocket(tempSocket);
            WSACleanup();
        }
        #endif
        delete[] idMsg;

        // wait for thread to stop
        std::unique_lock<std::mutex> rLck(m_runMtx);
        m_runCv.wait(rLck, [this]{ return !m_running; });
        m_acceptThread->join();
    }

    // close everything
    #ifndef WIN32
    close(m_socketD);
    #else
    int result = shutdown(m_socketD, SD_SEND);
    if (result == SOCKET_ERROR) {
        closesocket(m_socketD);
        WSACleanup();
    }
    #endif
    for (auto& client : m_clients) {
        closeClientConnections(client.second);
    }
    delete m_acceptThread;

    m_releaseQueue.clear();
    m_numEls = -1;
    m_garbageCv.notify_one();
    m_garbageCollectionThread->join();
    delete m_garbageCollectionThread;

    m_zombieCv.notify_one();
    m_zombieKillerThread->join();
    delete m_zombieKillerThread;

    m_shutdownV = true;
    m_shutdownCv.notify_all();
    log("server succesfully shut down");
}

void UmlServer::setMaxEls(int maxEls) {
    std::lock_guard<std::mutex> garbageLck(m_garbageMtx);
    m_maxEls = maxEls;
}

int UmlServer::getMaxEls() {
    return m_maxEls;
}

int UmlServer::getNumElsInMemory() {
    return m_numEls;
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