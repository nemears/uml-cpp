#ifndef _UML_MANAGERS_MANAGER_POLICY_H_
#define _UML_MANAGERS_MANAGER_POLICY_H_

#include "uml/element.h"
#include "abstractManager.h"
#include "abstractAccessPolicy.h"
#include "uml/umlPtr.h"

namespace UML {

    template <typename AccessPolicy, typename PersistencePolicy>
    class Manager : public AbstractManager , public AccessPolicy, public PersistencePolicy {
        protected:
            ElementPtr m_root;
        public:
            template <class T = Element>
            UmlPtr<T> create() {
                T* newElement = new T;
                ManagerNode& node = AccessPolicy::assignNode(newElement);
                newElement->m_node = &node;
                UmlPtr<T> ret(newElement);
                ret->m_manager2 = this;
                ret.m_manager2 = this;
                return ret;
            };

            Element* create(ElementType type) override {
                switch (type) {
                    case ElementType::CLASS : {
                        return create<Class>().ptr();
                    }
                    case ElementType::EXTENSION : {
                        return create<Extension>().ptr();
                    }
                    case ElementType::EXTENSION_END : {
                        return create<ExtensionEnd>().ptr();
                    }
                    case ElementType::INSTANCE_SPECIFICATION : {
                        return create<InstanceSpecification>().ptr();
                    }
                    case ElementType::MODEL : {
                        return create<Model>().ptr();
                    }
                    case ElementType::PACKAGE: {
                        return create<Package>().ptr();
                    }
                    case ElementType::PROFILE : {
                        return create<Profile>().ptr();
                    }
                    case ElementType::PROFILE_APPLICATION: {
                        return create<ProfileApplication>().ptr();
                    }
                    case ElementType::STEREOTYPE : {
                        return create<Stereotype>().ptr();
                    }
                    default: {
                        throw ManagerPolicyStateException("could not do multimethod create for element type: " + Element::elementTypeToString(type));
                    }
                }
                return 0;
            }

            ElementPtr get(ID id) override {
                ElementPtr ret = AccessPolicy::get(id);
                if (ret) {
                    return ret;
                }

                // not loaded or does not exist
                ret = PersistencePolicy::aquire(id, this);
                
                if (!ret) {
                    // Manager has no reference to an element of this id
                    // throw ManagerPolicyStateException("unknown id!");
                } else {
                    AccessPolicy::restoreNode(ret.m_node);
                }

                return ret;
            };

            bool loaded(ID id) override {
                return AccessPolicy::loaded(id);
            }

            void release(Element& el) override {
                PersistencePolicy::write(el, this);
                // el.m_node->m_mountedFlag = true;
                AccessPolicy::releaseNode(el.m_node);
            }

            void erase(Element& el) override {
                AccessPolicy::eraseNode(el.m_node, this);
            };

            void reindex(ID oldID, ID newID) override {
                AccessPolicy::reindex(oldID, newID);
                // TODO persistence policy reindex?
            }

            void removeNode(ID id) override {
                AccessPolicy::removeNode(id);
            }

            ElementPtr open(std::string path) override {
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

            void setLocation(ID id, std::string location) override {
                PersistencePolicy::setLocation(id, location);
            }
    };

    
    class ServerPersistencePolicy {
        
    };

    // typedef Manager<ThreadSafeAccessPolicy, FilePersistencePolicy> PolicyUmlServer;
    // typedef Manager<SimpleAccessPolicy, ServerPersistencePolicy> PolicyUmlClient;
}

#endif