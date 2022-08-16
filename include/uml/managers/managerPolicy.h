#ifndef _UML_MANAGERS_MANAGER_POLICY_H_
#define _UML_MANAGERS_MANAGER_POLICY_H_

#include "uml/element.h"
#include "abstractManager.h"
#include "abstractAccessPolicy.h"

namespace UML {

    class ManagerPolicyStateException : public std::exception {
        std::string m_msg;
        const char* what() const throw() override {
            return m_msg.c_str();
        };
        public:
            ManagerPolicyStateException(){};
            ManagerPolicyStateException(std::string msg) : m_msg("UmlManager bad state! " + msg) {};
    };

    template <typename AccessPolicy, typename PersistencePolicy>
    class Manager : public AbstractManager , public AccessPolicy, public PersistencePolicy {
        public:
            template <class T = Element>
            UmlPtr<T> create() {
                T* newElement = new T;
                AccessPolicy::assignNode(newElement);
                UmlPtr<T> ret(newElement);
                ret->m_manager2 = this;
                ret.m_manager2 = this;
                return ret;
            };

            Element* create(ElementType type) override {
                switch (type)
                {
                case ElementType::PACKAGE:
                    return create<Package>().ptr();
                    break;
                
                default:
                    break;
                }
                return 0;
            }

            ElementPtr get(ID id) override {
                ElementPtr ret = AccessPolicy::get(id);
                if (!ret) {
                    // not loaded or does not exist
                    ret = PersistencePolicy::aquire(id, this);
                }
                
                if (!ret) {
                    // Manager has no reference to an element of this id
                    // throw ManagerPolicyStateException("unknown id!");
                } else {
                    AccessPolicy::restoreNode(*ret);
                }

                return ret;
            };

            void release(Element& el) override {
                PersistencePolicy::write(el, this);
                // el.m_node->m_mountedFlag = true;
                AccessPolicy::releaseNode(el);
            }

            void reindex(ID oldID, ID newID) override {
                AccessPolicy::reindex(oldID, newID);
                // TODO persistence policy reindex?
            }

            void removeNode(ID id) override {
                AccessPolicy::removeNode(id);
            }

            void open(std::string path) override {
                ElementPtr root = PersistencePolicy::parse(path, this);
                AccessPolicy::setRoot(root);
            }
    };

    
    class ServerPersistencePolicy {
        
    };

    // typedef Manager<ThreadSafeAccessPolicy, FilePersistencePolicy> PolicyUmlServer;
    // typedef Manager<SimpleAccessPolicy, ServerPersistencePolicy> PolicyUmlClient;
}

#endif