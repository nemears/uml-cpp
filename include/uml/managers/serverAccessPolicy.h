#ifndef _UML_MANAGERS_SERVER_ACCESS_POLICY_H_
#define _UML_MANAGERS_SERVER_ACCESS_POLICY_H_

#include "simpleAccessPolicy.h"
#include "abstractClient.h"

namespace UML {
    class ServerAccessPolicy : virtual public SimpleAccessPolicy, virtual public AbstractClient {
        protected:
            ManagerNode& assignNode(Element* newElement) override {
                createElForServer(*newElement);
                return SimpleAccessPolicy::assignNode(newElement);
            }
            void restoreNode(ManagerNode* restoredNode) override {
                for (auto& pair : restoredNode->m_references) {
                    ManagerNode* node = pair.second.node;
                    if (node && node->m_managerElementMemory) {
                        continue;
                    }
                    // put reference back in map
                    if (loaded(pair.first)) {
                        pair.second.node = getNode(*get(pair.first));
                        if (!pair.second.node->m_references.count(restoredNode->m_managerElementMemory->getID())) {
                            pair.second.node->setReference(*restoredNode->m_managerElementMemory);
                        }
                    }
                }
                SimpleAccessPolicy::restoreNode(restoredNode);
            }
    };
}

#endif