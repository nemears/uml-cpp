#ifndef _UML_MANAGERS_SERVER_PERSISTENCE_POLICY_H_
#define _UML_MANAGERS_SERVER_PERSISTENCE_POLICY_H_

#include "abstractClient.h"
#include "uml/types/element.h"
#include "uml/managers/abstractManager.h"

namespace UML {
    class ServerPersistencePolicy : virtual public AbstractClient {
        protected:
            ElementPtr aquire(ID id, AbstractManager* manager) {
                return getElFromServer(id, manager);
            }
            void write(Element& el, AbstractManager* me) {
                writeToServer(el);
            }
            void write(AbstractManager* me) {
                saveToServer();
            }
            void write(std::string key, AbstractManager* me) {
                saveToServer(key);
            }
            ElementPtr parse(AbstractManager* me) {
                return getElFromServer("", me);
            }
            ElementPtr parse(std::string path, AbstractManager* me) {
                // TODO switch adress
                return getElFromServer("", me);
            }
            std::string getLocation(ID id) {
                return "";
            }
            std::string getLocation() {
                return "";
            }
            void setLocation(ID id, std::string location) {
                
            }
            void eraseEl(Element& el) {
                eraseFromServer(el);
            }
            void reindex(ID oldID, ID newID, AbstractManager* me) {
                writeToServer(*me->get(newID));
            }
    };
}

#endif