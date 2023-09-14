#pragma once
#include "uml/id.h"

#define UML_PORT 8652
#define UML_CLIENT_MSG_SIZE 200

namespace YAML {
    class Emitter;
}

namespace UML {
    class ServerPersistencePolicy {
        protected:
            std::string m_address;
            int m_port = UML_PORT;
            int m_socketD = 0;
            const ID clientID = ID::randomID();

            void sendEmitter(int socket, YAML::Emitter& emitter);
            std::string loadElementData(ID id);
            void saveElementData(std::string data, ID id);
            std::string getProjectData(std::string path);
            std::string getProjectData();
            void saveProjectData(std::string data, std::string path);
            void saveProjectData(std::string data);
            void eraseEl(ID id);
            void reindex(ID oldID, ID newID);
            ServerPersistencePolicy();
        public:
            void mount(std::string mountPath);
            virtual ~ServerPersistencePolicy();
    };
}

// namespace UML {
//     class ServerPersistencePolicy : virtual public AbstractClient {
//         protected:
//             ElementPtr aquire(ID id, AbstractManager* manager) {
//                 return getElFromServer(id, manager);
//             }
//             void write(Element& el, AbstractManager* me) {
//                 writeToServer(el);
//             }
//             void write(AbstractManager* me) {
//                 saveToServer();
//             }
//             void write(std::string key, AbstractManager* me) {
//                 saveToServer(key);
//             }
//             ElementPtr parse(AbstractManager* me) {
//                 return getElFromServer("", me);
//             }
//             ElementPtr parse(std::string path, AbstractManager* me) {
//                 // TODO switch adress
//                 return getElFromServer("", me);
//             }
//             std::string getLocation(ID id) {
//                 return "";
//             }
//             std::string getLocation() {
//                 return "";
//             }
//             void setLocation(ID id, std::string location) {
                
//             }
//             void eraseEl(Element& el) {
//                 eraseFromServer(el);
//             }
//             void reindex(ID oldID, ID newID, AbstractManager* me) {
//                 writeToServer(*me->get(newID));
//             }
//     };
// }
