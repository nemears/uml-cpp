#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <cstring>
#include <netinet/tcp.h>
#include <unistd.h>
#include "uml/managers/protocol/serverPersistencePolicy.h"
#include "uml/managers/abstractManager.h"
#include "uml/managers/serialization/open_uml/openUmlSerializationPolicy.h"
#include "yaml-cpp/yaml.h"

using namespace std;

namespace UML {

void ServerPersistencePolicy::sendEmitter(int socket, YAML::Emitter& emitter) {
    int totalBytesSent = 0;
    int bytesSent = 0;
    int packetSize = sizeof(uint32_t) + sizeof(char) * (emitter.size() + 1);
    char* packet = (char*) malloc(packetSize);
    uint32_t sizeOfEmitter = htonl(emitter.size() + 1);
    memcpy(packet, &sizeOfEmitter, sizeof(uint32_t));
    strcpy(packet + sizeof(uint32_t), emitter.c_str());
    while ((bytesSent = send(socket, packet + totalBytesSent, packetSize - totalBytesSent, 0)) < packetSize - totalBytesSent) {
        if (bytesSent == -1) {
            throw UML::ManagerStateException("could not send packet to server: " + string(strerror(errno)));
        } else {
            totalBytesSent += bytesSent;
        }
    }
    free(packet);
}

std::string ServerPersistencePolicy::loadElementData(ID id) {
    // request
    YAML::Emitter emitter;
    emitter << YAML::DoubleQuoted  << YAML::Flow << YAML::BeginMap << 
        YAML::Key << "GET" << YAML::Value << id.string() << 
    YAML::EndMap;
    sendEmitter(m_socketD, emitter);

    // receive
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
    
    string ret(buff);
    free(buff);
    return ret;
}

void ServerPersistencePolicy::saveElementData(std::string data, ID id) {
    YAML::Emitter emitter;
    emitter << YAML::DoubleQuoted << YAML::Flow << YAML::BeginMap << 
        YAML::Key << "PUT" << YAML::Value << YAML::BeginMap << YAML::Key << 
        "id" << YAML::Value << id.string() << YAML::Key << "element" << YAML::Value << 
        YAML::Load(data) << YAML::EndMap << YAML::EndMap;
    sendEmitter(m_socketD, emitter);
    // TODO restore qualified name and not loading twice
}

std::string ServerPersistencePolicy::getProjectData(std::string path) {
    // TODO connect to new server and get head
    throw ManagerStateException("Do not try to open with a client, get individual elements");
}

std::string ServerPersistencePolicy::getProjectData() {
    // get head and construct ????
    throw ManagerStateException("Do not try to open with a client, get individual elements");
}

void ServerPersistencePolicy::saveProjectData(std::string data, std::string path) {
    // TODO this one is weird, maybe we connect to a different server ?
    YAML::Emitter emitter;
    emitter << YAML::DoubleQuoted << YAML::Flow << YAML::BeginMap << YAML::Key << "save" << YAML::Value << "." << YAML::EndMap;
    sendEmitter(m_socketD, emitter);
}

void ServerPersistencePolicy::saveProjectData(std::string data) {
    YAML::Emitter emitter;
    emitter << YAML::DoubleQuoted << YAML::Flow << YAML::BeginMap << YAML::Key << "save" << YAML::Value << "." << YAML::EndMap;
    sendEmitter(m_socketD, emitter);
}

void ServerPersistencePolicy::eraseEl(ID id) {
    YAML::Emitter emitter;
    emitter << YAML::DoubleQuoted  << YAML::Flow << YAML::BeginMap << 
        YAML::Key << "DELETE" << YAML::Value << id.string() << 
    YAML::EndMap;
    sendEmitter(m_socketD, emitter);
}

void ServerPersistencePolicy::reindex(ID oldID, ID newID) {
    // TODO
}

ServerPersistencePolicy::ServerPersistencePolicy() {
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
    std::string strBuff = clientID.string();
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
    if (clientID.string().compare(acceptBuff) != 0) {
        throw ManagerStateException("did not get proper accept message!");
    }
}

void mount(string mountPath) {
    // TODO connect to another server
}

ServerPersistencePolicy::~ServerPersistencePolicy() {
    close(m_socketD);
}

}