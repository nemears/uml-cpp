#ifndef _UML_MANAGERS_MANAGER_POLICY_H_
#define _UML_MANAGERS_MANAGER_POLICY_H_
 
// #include "uml/umlManager.h"
#include "uml/element.h"
#include "abstractManager.h"

namespace UML {
    
    class AbstractAccessPolicy {
        protected:
            template <class T = Element>
            T* createPtr() {
                T* ptr = new T;
                ptr->m_accessPolicy = this;
                return ptr;
            }

            void setNode(ManagerNode* node) {
                node->m_managerElementMemory->m_node = node;
            }

            void restore(Element& el) {
                el.m_node->m_managerElementMemory = &el;
                for (auto& pair : el.m_node->m_references) {
                    ManagerNode* node = pair.second.node;
                    if (!node || !node->m_managerElementMemory) {
                        // element has been released, possibly there are no pointers
                        continue;
                    }
                    node->m_managerElementMemory->restoreReference(&el);
                    el.restoreReference(node->m_managerElementMemory);
                }
                for (auto& ptr : el.m_node->m_ptrs) {
                    ptr->reindex(el.getID(), &el);
                }
                el.restoreReferences();
            }

            ManagerNode* releaseNode(Element& el) {
                ManagerNode* node = el.m_node;
                ID id = el.getID();
                for (auto& e : node->m_references) {
                    if (!e.second.node) {
                        // el has been released there are no pointers
                        continue;
                    }
                    if (e.second.node->m_managerElementMemory) {
                        e.second.node->m_managerElementMemory->referencingReleased(id);
                    }
                }
                for (auto& ptr : node->m_ptrs) {
                    ptr->releasePtr();
                }
                node->m_managerElementMemory = 0;
                delete &el;
                return node;
            }
    };

    class AbstractPersistencePolicy {

    };

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
                UmlPtr<T> ret = AccessPolicy::template create<T>();
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

            void release(Element& el) {
                PersistencePolicy::write(el, this);
                // el.m_node->m_mountedFlag = true;
                AccessPolicy::releaseNode(el);
            }
    };

    class SimpleAccessPolicy : public AbstractAccessPolicy {
        private:
            std::unordered_map<ID, ManagerNode> m_graph;
        protected:
            template <class T = Element>
            UmlPtr<T> create() {
                T* ptr = createPtr<T>();
                m_graph[ptr->getID()] = {ptr};
                setNode(&m_graph[ptr->getID()]);
                UmlPtr<T> ret(ptr);
                return ret;
            }
            ElementPtr get(ID id) {
                std::unordered_map<ID, ManagerNode>::const_iterator result = m_graph.find(id);
                if (result != m_graph.end() && (*result).second.m_managerElementMemory) {
                    return ElementPtr(result->second.m_managerElementMemory);
                }
                return ElementPtr(0);
            }

            void restoreNode(Element& el) {
                restore(el);
            }

            void releaseNode(Element& el) {
                ID elID = el.getID();
                ManagerNode* node = AbstractAccessPolicy::releaseNode(el);
                if (node->m_ptrs.empty()) {
                    m_graph.erase(elID);
                }
            }
    };

    struct ThreadSafeManagerNode : public ManagerNode {
        std::mutex m_mtx;
    };

    class ThreadSafeAccessPolicy : public AbstractAccessPolicy {
        private:
            std::unordered_set<ID> m_elements;
            std::unordered_map<ID, ThreadSafeManagerNode> m_graph;
            std::mutex m_accessMtx;
        protected:
            template <class T = Element>
            UmlPtr<T> create() {
                std::lock_guard<std::mutex> accessLck(m_accessMtx);
                T* ptr = createPtr<T>();
                m_elements.insert(ptr->getID());
                m_graph[ptr->getID()] = {ptr};
                setNode(&m_graph[ptr->getID()]);
                UmlPtr<T> ret(ptr);
                return ret;
            }
    };

    class FilePersistencePolicy : public AbstractPersistencePolicy {
        private:
            std::filesystem::path m_mountBase;
        protected:
            ElementPtr aquire(ID id, AbstractManager* manager) {
                if (!std::filesystem::exists(m_mountBase / "mount" / (id.string() + ".yml"))) {
                    throw ID_doesNotExistException(id);
                }
                Parsers::ParserMetaData data;
                data.m_manager2 = manager;
                data.m_path = m_mountBase / "mount" / (id.string() + ".yml");
                data.m_strategy = Parsers::ParserStrategy::INDIVIDUAL;
                ElementPtr ret = Parsers::parse(data);
                return ret;
            }

            void write(Element& el, AbstractManager* me) {
                if (m_mountBase.empty()) {
                    throw ManagerPolicyStateException("manager not mounted");
                }
                Parsers::EmitterMetaData data = { m_mountBase / std::filesystem::path("mount"),
                                                    Parsers::EmitterStrategy::INDIVIDUAL,
                                                    el.getID().string() + ".yml",
                                                    0,
                                                    me };
                Parsers::emitToFile(el, data, data.m_path.string(), data.m_fileName);
            }
        public:
            void mount(std::filesystem::path mountPath) {
                m_mountBase = mountPath;
                std::filesystem::create_directories(m_mountBase / std::filesystem::path("mount"));
                // for (auto& pair : m_graph) {
                //     mountEl(*pair.second.m_managerElementMemory);
                // }
            }
    };

    class ServerPersistencePolicy {
        
    };

    typedef Manager<ThreadSafeAccessPolicy, FilePersistencePolicy> PolicyUmlServer;
    typedef Manager<SimpleAccessPolicy, ServerPersistencePolicy> PolicyUmlClient;
    typedef Manager<SimpleAccessPolicy, FilePersistencePolicy> BasicManager;
}

#endif