#include "uml/managers/protocol/umlClient.h"
#include "uml/uml-stable.h"
#include "yaml-cpp/yaml.h"
#include <sys/socket.h>
#include <sys/types.h>

using namespace std;

namespace UML {

ElementPtr UmlClient::get(std::string qualifiedName) {
    // TODO check if one is in memory?
    
    // request
    YAML::Emitter emitter;
    emitter << YAML::DoubleQuoted  << YAML::Flow << YAML::BeginMap << 
        YAML::Key << "GET" << YAML::Value << qualifiedName << 
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
    
    string data(buff);
    free(buff);

    // parse
    ElementPtr ret = OpenUmlJsonSerializationPolicy::parseIndividual(data, *this);
    
    if (ret) {
        restoreNode(ret);
    }

    return ret;
}

ElementPtr UmlClient::get(ID id) {
    return Manager<OpenUmlJsonSerializationPolicy, ServerPersistencePolicy>::get(id);
}

void UmlClient::setRoot(Element* root) {
    Manager<OpenUmlJsonSerializationPolicy, ServerPersistencePolicy>::setRoot(root);
    if (!root) {
        throw new ManagerStateException("TODO set root to null on server");
    }
    YAML::Emitter emitter;
    emitter << YAML::DoubleQuoted << YAML::Flow << YAML::BeginMap << 
        YAML::Key << "PUT" << YAML::Value << YAML::BeginMap << YAML::Key << 
        "id" << YAML::Value << root->getID().string() << YAML::Key << "qualifiedName" << YAML::Value << "" << 
        YAML::Key << "element" << YAML::Value << YAML::Load(emitIndividual(*root, *this)) << YAML::EndMap << YAML::EndMap;
    sendEmitter(m_socketD, emitter);
}

}