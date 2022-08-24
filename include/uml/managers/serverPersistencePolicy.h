#ifndef _UML_MANAGERS_SERVER_PERSISTENCE_POLICY_H_
#define _UML_MANAGERS_SERVER_PERSISTENCE_POLICY_H_

#include "uml/element.h"
#include "abstractManager.h"

namespace UML {
    class ServerPersistencePolicy {
        protected:
            ElementPtr aquire(ID id, AbstractManager* manager) {
                return ElementPtr();
            }
            void write(Element& el, AbstractManager* me) {

            }
            void write(AbstractManager* me) {

            }
            void write(std::string key, AbstractManager* me) {
                
            }
            ElementPtr parse(AbstractManager* me) {

            }
            ElementPtr parse(std::string path, AbstractManager* me) {
                return ElementPtr();
            }
            std::string getLocation(ID id) {
                return "";
            }
            void setLocation(ID id, std::string location) {
                
            }
    };
}

#endif