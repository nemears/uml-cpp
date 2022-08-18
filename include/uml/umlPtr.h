#ifndef _UML_UML_PTR_H_
#define _UML_UML_PTR_H_

#include "element.h"
#include "managers/abstractManager.h"

namespace UML {

    struct NullPtrException : public std::exception {
        const char* what() const  throw() override {
            return "tried to access null UmlPtr";
        };
    };

    template <class T, class U> class Singleton;
    template <class T> class UmlPtr;

    class AbstractUmlPtr {

        template <class T, class U> friend class Singleton;
        friend class UmlManager;
        template <class AccessPolicy, class PersistencePolicy> friend class Manager;
        friend class AbstractAccessPolicy;
        template <class T> friend class UmlPtr;

        protected:
            ID m_id = ID::nullID();
            long int m_ptrId = 0;
            virtual void reindex(ID newID, Element* el) = 0;
            virtual void releasePtr() = 0;
            virtual void erasePtr() = 0;
    };

    template <class T = Element>
    class UmlPtr : public AbstractUmlPtr {

        template <class U, class V> friend class Singleton;
        friend class UmlManager;
        template <class AccessPolicy, class PersistencePolicy> friend class Manager;
        friend class AbstractAccessPolicy;
        template <class U> friend class UmlPtr;

        private:
            UmlManager* m_manager = 0;
            AbstractManager* m_manager2 = 0;
            T* m_ptr = 0;
            ManagerNode* m_node = 0;
        protected:
            void reindex(ID newID, Element* el) override {
                m_id = newID;
                m_ptr = dynamic_cast<T*>(el);
                m_node = m_ptr->m_node;
            };
            void releasePtr() override {
                m_ptr = 0;
            };
            void erasePtr() override {
                m_id = ID::nullID();
                m_ptr = 0;
                m_node = 0;
            };
            void removeFromGraph() {
                if (m_node) {
                    // TODO lock this node during this scope when multithreading
                    m_node->m_ptrs.remove_if([this](const AbstractUmlPtr* ptr) {
                        return ptr->m_ptrId == m_ptrId;
                    });
                    if (m_node->m_ptrs.empty() && !m_node->m_managerElementMemory) {
                        if (m_manager) {
                            m_manager->m_graph.erase(m_id);
                        } else if (m_manager2) {
                            m_manager2->removeNode(m_id);
                        }
                    }
                }
            };
            template <class U = Element>
            void reassignPtr(const UmlPtr<U>& rhs) {
                removeFromGraph();
                m_id = rhs.m_id;
                m_manager = rhs.m_manager;
                m_manager2 = rhs.m_manager2;
                m_ptr = 0;
                m_ptrId = 0;
                m_node = rhs.m_node;
                if (m_node) {
                    m_ptr = dynamic_cast<T*>(rhs.m_ptr);
                    if (m_node->m_ptrs.size() > 0) {
                        m_ptrId = m_node->m_ptrs.back()->m_ptrId + 1;
                    }
                    m_node->m_ptrs.push_back(const_cast<UmlPtr<T>*>(this));
                }
            };
        public:
            T& operator*() const {
                if (m_id == ID::nullID()) {
                    throw NullPtrException();
                } else if (m_ptr) {
                    return *m_ptr;
                } else {
                    ElementPtr temp(0);
                    if (m_manager2) {
                        temp = m_manager2->get(m_id);
                    } else {
                        temp = &m_manager->get(m_id);
                    }
                    if (!m_ptr) {
                        const_cast<UmlPtr<T>*>(this)->m_ptr = dynamic_cast<T*>(temp.ptr());
                    }
                    const_cast<UmlPtr<T>*>(this)->m_node = m_ptr->m_node;
                    return  *dynamic_cast<T*>(temp.ptr());
                }
            };
            T* operator->() const {
                if (m_id == ID::nullID()) {
                    throw NullPtrException();
                } else if (m_ptr) {
                    return m_ptr;
                } else {
                    ElementPtr temp(0);
                    if (m_manager2) {
                        temp = m_manager2->get(m_id);
                    } else {
                        temp = &m_manager->get(m_id);
                    }
                    if (!m_ptr) {
                        const_cast<UmlPtr<T>*>(this)->m_ptr = dynamic_cast<T*>(temp.ptr());
                    }
                    const_cast<UmlPtr<T>*>(this)->m_node = m_ptr->m_node;
                    return  dynamic_cast<T*>(temp.ptr());
                }
            };
            operator bool() const {
                return m_id != ID::nullID();
            };
            inline friend bool operator==(const Element* lhs, const UmlPtr& rhs) {
                return lhs->m_id == rhs.m_id;
            };
            inline friend bool operator!=(const Element* lhs, const UmlPtr& rhs) {
                return lhs->m_id != rhs.m_id;
            };
            inline friend bool operator==(const UmlPtr& lhs, const UmlPtr& rhs) {
                return lhs.m_id == rhs.m_id;
            };
            inline friend bool operator!=(const UmlPtr& lhs, const UmlPtr& rhs) {
                return lhs.m_id != rhs.m_id;
            };
            template <class U = Element>
            inline friend bool operator==(const UmlPtr& lhs, const UmlPtr<U>& rhs) {
                return lhs.id() == rhs.id();
            }
            template <class U = Element>
            inline friend bool operator!=(const UmlPtr& lhs, const UmlPtr<U>& rhs) {
                return lhs.id() != rhs.id();
            }
            inline friend bool operator==(const Element& lhs, const UmlPtr& rhs) {
                return lhs.m_id == rhs.m_id;
            };
            inline friend bool operator!=(const Element& lhs, const UmlPtr& rhs) {
                return lhs.m_id != rhs.m_id;
            };
            void operator=(const T* el) {
                removeFromGraph();
                if (el) {
                    m_id = el->getID();
                    m_ptr = const_cast<T*>(el);
                    m_node = m_ptr->m_node;
                    m_manager = el->m_manager; // delete
                    m_manager2 = el->m_manager2;
                    m_ptrId = 0;
                    if (el->m_node->m_ptrs.size() > 0) {
                        m_ptrId = el->m_node->m_ptrs.back()->m_ptrId + 1;
                    }
                    el->m_node->m_ptrs.push_back(const_cast<UmlPtr<T>*>(this));
                }
            };
            void operator=(const UmlPtr& rhs) {
                reassignPtr(rhs);
            };
            UmlPtr(const UmlPtr& rhs) {
                reassignPtr(rhs);
            };
            template <class U = Element>
            UmlPtr(const UmlPtr<U>& rhs) {
                reassignPtr(rhs);
            };
            T* ptr() {
                if (m_id == ID::nullID()) {
                    return 0;
                } else if (m_ptr) {
                    return m_ptr;
                } else {
                    ElementPtr temp(0);
                    if (m_manager2) {
                        temp = m_manager2->get(m_id);
                    } else {
                        temp = &m_manager->get(m_id);
                    }
                    if (!m_ptr) {
                        const_cast<UmlPtr<T>*>(this)->m_ptr = dynamic_cast<T*>(temp.ptr());
                    }
                    const_cast<UmlPtr<T>*>(this)->m_node = m_ptr->m_node;
                    return  dynamic_cast<T*>(temp.ptr());
                }
            };
            ID id() const {
                return m_id;
            };
            bool has() const {
                return m_id != ID::nullID();
            };
            bool loaded() const {
                if (m_id != ID::nullID()) {
                    return m_ptr != 0;
                }
                return false;
            };
            void release() {
                if (m_ptr) {
                    if (m_manager) { // TODO delete
                        m_manager->release(*m_ptr);
                    } else if (m_manager2) {
                        m_manager2->release(*m_ptr);
                    }
                }
            };
            void aquire() {
                ElementPtr temp(0);
                if (m_manager) {
                    temp = m_manager->aquire(m_id);
                } else if (m_manager2) {
                    temp = m_manager2->get(m_id);
                }
                m_ptr = dynamic_cast<T*>(temp.ptr());
                m_node = m_ptr->m_node;
                m_node->m_ptrs.push_back(const_cast<UmlPtr<T>*>(this));
            };
            UmlPtr(T* el) {
                if (el) {
                    m_id = el->getID();
                    m_ptr = el;
                    m_manager = el->m_manager;
                    m_manager2 = el->m_manager2;
                    if (el->m_node->m_ptrs.size() > 0) {
                        m_ptrId = el->m_node->m_ptrs.back()->m_ptrId + 1;
                    }
                    el->m_node->m_ptrs.push_back(const_cast<UmlPtr<T>*>(this));
                    m_node = m_ptr->m_node;
                }
            };
            UmlPtr() {
                // nothing
            };
            virtual ~UmlPtr() {
                if (m_id == ID::nullID()) {
                    return;
                }
                if (m_ptr) {
                    removeFromGraph();
                    if (m_ptr->m_node->m_ptrs.empty()) {
                        if (m_manager && !m_manager->m_mountBase.empty() && !m_manager->m_lazy) {
                            m_manager->mountEl(*m_ptr);
                            m_manager->releaseNode(*m_ptr);
                            m_manager->m_graph.erase(m_id);
                            delete m_ptr;
                        }
                    }
                } else {
                    if (m_node) {
                        removeFromGraph();
                        // if (m_node->m_ptrs.empty()) {
                        //     m_manager->m_graph.erase(m_id);
                        // }
                    }
                }
            };
    };
}

#endif