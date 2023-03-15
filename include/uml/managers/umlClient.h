#ifndef _UML_UML_CLIENT_H_
#define _UML_UML_CLIENT_H_

#include "manager.h"
#include "serverAccessPolicy.h"
#include "serverPersistencePolicy.h"

namespace UML {
    class UmlClient : virtual public Manager<ServerAccessPolicy, ServerPersistencePolicy> {
        public:
            virtual ~UmlClient() {}
            ElementPtr get(std::string qualifiedName) {
                return getElFromServer(qualifiedName, this);
            }
            ElementPtr get(ID id) {
                return Manager<ServerAccessPolicy, ServerPersistencePolicy>::get(id);
            }
            void setRoot(Element* root) override {
                setRootForServer(root);
                Manager<ServerAccessPolicy, ServerPersistencePolicy>::setRoot(root);
            }
        protected:
            Element* get(Element* me, ID theID) override {
                Element* el = AbstractManager::get(me, theID);
                if (!el) {
                    return el;
                }
                if (!getNode(*el)->m_references.count(me->getID())) {
                    getNode(*el)->setReference(*me);
                }
                return el;
            }
    };
}

#endif