#include "uml/uml-stable.h"
#include "uml/managers/umlClient.h"
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
#include "uml/managers/umlServer.h"

#ifdef UML_DEBUG
#include <iostream>
#endif

#include "yaml-cpp/yaml.h"
#include "uml/parsers/parser.h"

#define UML_CLIENT_MSG_SIZE 200

using namespace UML;

#ifndef WIN32
typedef int socketType;
#else
typedef SOCKET socketType;
#endif

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

Element* UmlClient::get(Element* me, ID theID) {
    if (!theID.isNull()) {
        if (me->m_node) {
            if (me->m_node->m_references.count(theID)) {
                if (!me->m_node->m_references[theID].node || !me->m_node->m_references[theID].node->m_managerElementMemory) {
                    // check map to allow quick restore of reference
                    std::unordered_map<ID, ManagerNode>::iterator it = m_graph.find(theID);
                    if (it != m_graph.end() && it->second.m_managerElementMemory) {
                        me->m_node->m_references[theID].node = &(it->second);
                        if (it->second.m_references.count(me->getID())) {
                            it->second.restoreReference(me);
                        } else {
                            it->second.setReference(me->getID());
                        }
                    } else {
                        ElementPtr aquired = get(theID);
                        me->m_node->m_references[theID].node = aquired->m_node;
                    }
                }
                return me->m_node->m_references[theID].node->m_managerElementMemory;
            } else {
                throw ManagerStateException("could not find reference " + theID.string());
            }
        } else {
            get(theID);
            return  me->m_node->m_references[theID].node->m_managerElementMemory;
        }
    }
    return 0;
}

ElementPtr UmlClient::get(ID id) {
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
    data.m_manager = this;
    if (std::string("ERROR").compare(buff) == 0) {
        free(buff);
        throw ManagerStateException("ERROR from server!");
    }
    data.m_strategy = Parsers::ParserStrategy::INDIVIDUAL;
    Element& ret = *Parsers::parseString(buff, data);
    free(buff);
    for (auto& pair : ret.m_node->m_references) {
        if (!pair.second.node && exists(pair.first)) {
            pair.second.node = &m_graph[pair.first];
        }
        if (!pair.second.node || !pair.second.node->m_managerElementMemory) {
            // element has been released, possibly there are no pointers
            continue;
        }
        if (pair.second.node->m_references.count(id)) {
            pair.second.node->restoreReference(&ret);
        } else {
            pair.second.node->setReference(ret);
        }
        ret.m_node->restoreReference(pair.second.node->m_managerElementMemory);
    }
    // SimpleAccessPolicy::restoreNode(ret.m_node);
    #ifdef UML_DEBUG
    std::cout << time_in_HH_MM_SS_MMM() << ": client got el of id " << id.string() << std::endl;
    #endif
    return ElementPtr(&ret);
}

ElementPtr UmlClient::get(std::string qualifiedName) {
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
    data.m_manager = this;
    data.m_strategy = Parsers::ParserStrategy::INDIVIDUAL;
    Element& ret = *Parsers::parseString(buff, data);
    free(buff);
    #ifdef UML_DEBUG
    std::cout << time_in_HH_MM_SS_MMM() << ": client got el of name " << qualifiedName << std::endl;
    #endif
    return ElementPtr(&ret);
}

Element& UmlClient::post(ElementType eType) {
    Element* ret = Manager<SimpleAccessPolicy, ServerPersistencePolicy>::create(eType);
    YAML::Emitter emitter;
    emitter << YAML::DoubleQuoted  << YAML::Flow << YAML::BeginMap << 
        YAML::Key << "POST" << YAML::Value << Element::elementTypeToString(eType) << 
        YAML::Key << "id" << YAML::Value << ret->getID().string() <<
    YAML::EndMap;
    sendEmitter(m_socketD, emitter);
    #ifdef UML_DEBUG
    std::cout << time_in_HH_MM_SS_MMM() << ": client created element of id " << ret->getID().string() << std::endl;
    #endif
    return *ret;
}

void UmlClient::put(Element& el) {
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

void UmlClient::putAll() {
    for (auto& node : m_graph) {
        put(*node.second.m_managerElementMemory);
    }
}

void UmlClient::erase(Element& el) {
    YAML::Emitter emitter;
    emitter << YAML::DoubleQuoted  << YAML::Flow << YAML::BeginMap << 
        YAML::Key << "DELETE" << YAML::Value << el.getID().string() << 
    YAML::EndMap;
    Manager<SimpleAccessPolicy, ServerPersistencePolicy>::erase(el);
    sendEmitter(m_socketD, emitter);
    #ifdef UML_DEBUG
    std::cout << time_in_HH_MM_SS_MMM() << ": client erased el" << std::endl;
    #endif
}

void UmlClient::release(Element& el) {
    put(el);
    Manager<SimpleAccessPolicy, ServerPersistencePolicy>::release(el);
}

void UmlClient::setRoot(Element* el) {
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
        m_root = el;
    } else {
        m_root = ElementPtr();
        erase(*getRoot());
    }
    #ifdef UML_DEBUG
    std::cout << time_in_HH_MM_SS_MMM() << ": client set root" << std::endl;
    #endif
}

void UmlClient::shutdownServer() {
    const char* msg = "KILL";
    int bytesSent;
    while ((bytesSent = send(m_socketD, msg, 5, 0)) <= 0) {
        send(m_socketD, msg, 5, 0);
    }
}

void UmlClient::save() {
    YAML::Emitter emitter;
    emitter << YAML::DoubleQuoted  << YAML::Flow << YAML::BeginMap;
        emitter << YAML::Key << "SAVE" << YAML::Value << ".";
    emitter << YAML::EndMap;
    sendEmitter(m_socketD, emitter);
}

void UmlClient::save(std::string path) {
    YAML::Emitter emitter;
    emitter << YAML::DoubleQuoted  << YAML::Flow << YAML::BeginMap;
        emitter << YAML::Key << "SAVE" << YAML::Value << path;
    emitter << YAML::EndMap;
    sendEmitter(m_socketD, emitter);
}