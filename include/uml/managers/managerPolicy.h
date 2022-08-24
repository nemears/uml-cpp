#ifndef _UML_MANAGERS_MANAGER_POLICY_H_
#define _UML_MANAGERS_MANAGER_POLICY_H_

#include "uml/element.h"
#include "abstractManager.h"
#include "abstractAccessPolicy.h"
#include "uml/umlPtr.h"

namespace UML {

    /**
     * This is the policy class for a manager of UML elements, it requires two policies to be defined
     * in order to use it:
     *  AccessPolicy: The policy handling accessing the managers current memory of uml elements
     *  PersistencePolicy: The policy handling the persistent storage of uml elements as well as the
     *      swap space. // TODO think about seperating persistence and swap policies
     * 
     * These policies have methods that need to be implemented:
     *      AccessPolicy:
     *          - ManagerNode& assignNode(Element* newElement) : assigns the element a ManagerNode* object or sub object
     *          - ElementPtr get(ID id) : get an element from memory
     *          - bool loaded(ID id) : the element with the specified id is loaded in memory
     *          - void restoreNode(ManagerNode* restoredNode) : restore a node from memory
     *          - void eraseNode(ManagerNode* node, AbstractManager* me) : erase a node from existence
     *          - void releaseNode(ManagerNode* node) : release a node from memory
     *          - void reindex(ID oldID, ID newID) : reindex a node within the node graph
     *          - void removeNode(ID id) : remove a node with the specified id from the graph
     *      PersistencePolicy:
     *          - ElementPtr aquire(ID id, AbstractManager* manager) : aquire an element with the specified id from disk
     *          - void write(Element& el, AbstractManager* me) : write an element to disk
     *          - void write(AbstractManager* me) : write everything to disk whole
     *          - void write(std::string key, AbstractManager* me) : write to disk under key
     *          - ElementPtr parse(AbstractManager* me) : read from disk
     *          - ElementPtr parse(std::string path, AbstractManager* me) : read from specified signifier
     *          - std::string getLocation(ID id) : get ID of signifier
     *          - void setLocation(ID id, std::string location) : set a signifier ID pair
     **/
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

            void save(std::string location) override {
                PersistencePolicy::write(location, this);
            }

            void save() override {
                PersistencePolicy::write(this);
            }

            void setRoot(Element* root) override {
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
}

#endif