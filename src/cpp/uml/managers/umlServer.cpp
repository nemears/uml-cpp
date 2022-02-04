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

void UmlServer::handleMessage(UmlServer* me, ID id, char* buff) {
    ClientInfo& info = me->m_clients[id];
    if (!me->m_msgV) {
        me->m_msgV = true;
    }
    // wait for handler / client thread to tell us to start
    std::unique_lock<std::mutex> clientLock(info.messageMtx);
    me->m_msgCv.wait(clientLock, [&info] { return info.messageV; });
    info.handlerV = false;
    info.messageV = false;

    // handle message TODO cleanup
    me->log("server got message from client(" + id.string() + "):\n" + std::string(buff));

    if (strcmp(buff, "KILL") == 0) {
        me->m_msgV = true;
        me->m_msgCv.notify_one();
        info.handlerCv.notify_one();
        me->shutdown();
        free(buff);
        return;
    }
    
    std::vector<YAML::Node> messages = YAML::LoadAll(buff);
    for (YAML::Node node : messages) {
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
            int bytesSent = send(info.socket, msg.c_str(), msg.size() + 1, 0);
            if (bytesSent <= 0) {
                throw ManagerStateException();
            }
            me->log("server got element " +  elID.string() + " for client " + id.string() + ":\n" + msg);
        }
        if (node["POST"]) {
            me->m_msgV = true;
            me->m_msgCv.notify_one();
            me->log("server handling post request from client " + id.string());
            try {
                ElementType type = Parsers::elementTypeFromString(node["POST"].as<std::string>());
                ID id = ID::fromString(node["id"].as<std::string>());
                Element* ret = 0;
                ret = &me->create(type);
                ret->setID(id);
                // std::string msg = Parsers::emit(*ret);
                // int bytesSent = send(pfds->fd, msg.c_str(), msg.size() + 1, 0);
                // if (bytesSent <= 0) {
                //     free(buff);
                //     throw ManagerStateException();
                // } 
                me->log("server created new element for client" + id.string());
            } catch (std::exception& e) {
                // std::string msg = "ERROR: " + std::string(e.what());
                // int bytesSent = send(pfds->fd, msg.c_str(), msg.size() + 1 , 0);
                // if (bytesSent <= 0) {
                //     free(buff);
                //     throw ManagerStateException();
                // }
                // TODO revive above and implement thread to handle errors for client
                me->log("server could not create new element for client " + id.string() + ", exception with request: " + std::string(e.what()));
            }
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
            
            // int msg = 1, bytesSent = 1;
            // while((bytesSent = send(pfds->fd, &msg, sizeof(int), 0)) <= 0) {
            //     bytesSent = send(pfds->fd, &msg, sizeof(int), 0);
            // }
            // me->log("told client we got element put");
        }
    }
    free(buff);
    info.handlerV = true;
    info.handlerCv.notify_one();
}

void UmlServer::receiveFromClient(UmlServer* me, ID id) {
    me->log("server set up thread to listen to client " + id.string());
    struct pollfd pfds[1] = {{me->m_clients[id].socket, POLLIN}};
    while (me->m_running) {
        int numEvents;
        if ((numEvents = poll(pfds, 1, 1000)) > 0) { 
            /**
             * TODO: Move everything in this conditional to a message handler thread so that poll wont miss any
             *       messages from the client. We need another thread to properly join it later without blocking this thread
             **/
            if (numEvents == 0) {
                continue;
            }
            ClientInfo& info = me->m_clients[id];
            char* buff = (char*)malloc(UML_SERVER_MSG_SIZE);
            int bytesReceived = recv(info.socket, buff, UML_SERVER_MSG_SIZE, 0);
            if (bytesReceived <= 0) {
                free(buff);
                // info.handlerCv.notify_one();
                continue;//return;
            }
            int i = 0;
            while (bytesReceived >= UML_SERVER_MSG_SIZE - 1) {
                if (buff[i*UML_SERVER_MSG_SIZE + UML_SERVER_MSG_SIZE - 1] != '\0') {
                    me->log("did not receive all of the message, waiting for follow up data");
                    // replace escape character if there is one
                    char c = buff[i*UML_SERVER_MSG_SIZE + 1];
                    size_t ci = 1;
                    while (c != '\0' && ci < UML_SERVER_MSG_SIZE) {
                        c = buff[i*UML_SERVER_MSG_SIZE + ci];
                        ci++;
                    }
                    if (c == '\0') {
                        *(buff + (i*UML_SERVER_MSG_SIZE) + ci-1) = '\n';
                    }
                    // get rest of data
                    buff = (char*)realloc(buff, 2 * UML_SERVER_MSG_SIZE + i * UML_SERVER_MSG_SIZE);
                    bytesReceived = recv(info.socket, buff + UML_SERVER_MSG_SIZE + (i * UML_SERVER_MSG_SIZE), UML_SERVER_MSG_SIZE, 0);
                } else {
                    break;
                }
                i++;
            }
            me->m_clients[id].threadQueue.push_back(new std::thread(handleMessage, me, id, buff));
            if (me->m_clients[id].threadQueue.size() == 1) {
                // if thread queue is empty start it up
                me->m_clients[id].messageV = true;
                me->m_clients[id].messageCv.notify_one();
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
        info.handlerCv.wait(lck, [&info] { return info.handlerV; });
        if (!info.threadQueue.empty()) {
            std::lock_guard<std::mutex> mLck(me->m_msgMtx);
            std::thread* doneThread = info.threadQueue.front();
            info.threadQueue.pop_front();
            doneThread->join();
            delete doneThread;
            info.messageV = true;
            info.messageCv.notify_one();
            if (info.threadQueue.empty()) {
                // check other clients to make sure no processing is being done and notify server if idle
                bool isIdle = true;
                for (auto& pair : me->m_clients) {
                    if (!pair.second.threadQueue.empty()) {
                        isIdle = false;
                        break;
                    }
                }
                if (isIdle) {
                    me->m_msgV = false;
                    me->m_msgCv.notify_all();
                }
            }
        }
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
    std::unique_lock<std::mutex> mLck(m_msgMtx);
    m_msgCv.wait(mLck, [this]{ return !m_msgV; });

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
        client.second.handlerV = true;
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