#ifndef _UML_MANAGERS_MANAGER_POLICY_H_
#define _UML_MANAGERS_MANAGER_POLICY_H_

#include "uml/element.h"
#include "abstractManager.h"
#include "abstractAccessPolicy.h"
#include "uml/umlPtr.h"

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
        protected:
            ElementPtr m_root;
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
                switch (type) {
                    case ElementType::MODEL : {
                        return create<Model>().ptr();
                    }
                    case ElementType::PACKAGE: {
                        return create<Package>().ptr();
                        break;
                    }
                    default: {
                        throw ManagerPolicyStateException("could not do multimethod create!");
                    }
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

            ElementPtr open(std::string path) {
                m_root = PersistencePolicy::parse(path, this);
                return m_root;
            }

            ElementPtr open() {
                m_root = PersistencePolicy::parse(this);
                return m_root;
            }

            void save() {
                PersistencePolicy::write(this);
            }

            void setRoot(Element* root) {
                m_root = root;
            }

            ElementPtr getRoot() const override {
                return m_root;
            }

            std::string getLocation(ID id) override {
                return PersistencePolicy::getLocation(id);
            }
    };

    
    class ServerPersistencePolicy {
        
    };

    // typedef Manager<ThreadSafeAccessPolicy, FilePersistencePolicy> PolicyUmlServer;
    // typedef Manager<SimpleAccessPolicy, ServerPersistencePolicy> PolicyUmlClient;
}

#endif