#ifndef _UML_UML_PTR_H_
#define _UML_UML_PTR_H_

#include "element.h"

namespace UML {

    struct NullPtrException : public std::exception {
        const char* what() const  throw() override {
            return "tried to access null UmlPtr";
        };
    };

    template <class T, class U> class Singleton;

    class AbstractUmlPtr {

        friend class UmlManager;

        protected:
            ID m_id = ID::nullID();
            virtual void reindex(ID newID, Element* el) = 0;
            virtual void releasePtr() = 0;
            virtual void erasePtr() = 0;
    };

    template <class T = Element>
    class UmlPtr : public AbstractUmlPtr {

        template <class U, class V> friend class Singleton;
        friend class UmlManager;
        template <class U> friend class UmlPtr;

        private:
            UmlManager* m_manager = 0;
            T* m_ptr = 0;
        protected:
            void reindex(ID newID, Element* el) override {
                m_id = newID;
                m_ptr = dynamic_cast<T*>(el);
            };
            void releasePtr() override {
                m_ptr = 0;
            };
            void erasePtr() override {
                m_id = ID::nullID();
                m_ptr = 0;
            };
        public:
            T& operator*() const {
                if (m_id == ID::nullID()) {
                    throw NullPtrException();
                } else if (m_ptr) {
                    return *m_ptr;
                } else {
                    ElementPtr temp = &m_manager->get(m_id);
                    temp->m_node->m_ptrs.push_back((void*) this);
                    if (!m_ptr) {
                        const_cast<UmlPtr<T>*>(this)->m_ptr = dynamic_cast<T*>(temp.ptr());
                    }
                    return  *dynamic_cast<T*>(temp.ptr());
                }
            };
            T* operator->() const {
                if (m_id == ID::nullID()) {
                    throw NullPtrException();
                } else if (m_ptr) {
                    return m_ptr;
                } else {
                    ElementPtr temp = &m_manager->get(m_id);
                    temp->m_node->m_ptrs.push_back((void*) this);
                    if (!m_ptr) {
                        const_cast<UmlPtr<T>*>(this)->m_ptr = dynamic_cast<T*>(temp.ptr());
                    }
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
                if (m_ptr) {
                    m_ptr->m_node->m_ptrs.remove((void*) this);
                }
                if (el) {
                    m_id = el->getID();
                    m_ptr = const_cast<T*>(el);
                    m_manager = el->m_manager;
                    el->m_node->m_ptrs.push_back((void*) this);
                }
            };
            void operator=(const UmlPtr& rhs) {
                if (m_ptr) {
                    m_ptr->m_node->m_ptrs.remove((void*) this);
                }
                m_id = rhs.m_id;
                m_manager = rhs.m_manager;
                if (rhs.m_ptr) {
                    m_ptr = rhs.m_ptr;
                    m_ptr->m_node->m_ptrs.push_back((void*) this);
                }
            };
            UmlPtr(const UmlPtr& rhs) {
                if (m_ptr) {
                    m_ptr->m_node->m_ptrs.remove((void*) this);
                }
                m_id = rhs.m_id;
                m_manager = rhs.m_manager;
                if (rhs.m_ptr) {
                    m_ptr = rhs.m_ptr;
                    m_ptr->m_node->m_ptrs.push_back((void*) this);
                }
            };
            template <class U = Element>
            UmlPtr(const UmlPtr<U>& rhs) {
                if (m_ptr) {
                    m_ptr->m_node->m_ptrs.remove((void*) this);
                }
                m_id = rhs.m_id;
                m_manager = rhs.m_manager;
                if (rhs.m_ptr) {
                    m_ptr = const_cast<T*>(&rhs->template as<T>());
                    m_ptr->m_node->m_ptrs.push_back((void*) this);
                }
            }
            T* ptr() {
                return m_ptr;
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
                    m_manager->release(*m_ptr);
                }
            };
            void aquire() {
                UmlPtr<> temp = m_manager->aquire(m_id);
                m_ptr = dynamic_cast<T*>(temp.ptr());
                temp->m_node->m_ptrs.push_back((void*) this);
            };
            UmlPtr(T* el) {
                if (el) {
                    m_id = el->getID();
                    m_ptr = el;
                    m_manager = el->m_manager;
                    el->m_node->m_ptrs.push_back((void*) this);
                }
            };
            virtual ~UmlPtr() {
                if (m_id != ID::nullID()) {
                    if (m_ptr) {
                        m_ptr->m_node->m_ptrs.remove(this);
                        if (m_ptr->m_node->m_ptrs.empty()) {
                            if (m_manager && (!m_manager->m_lossless || !m_manager->m_mountBase.empty()) && !m_manager->m_lazy) {
                                m_manager->mountEl(*m_ptr);
                                m_manager->releaseNode(*m_ptr);
                                m_manager->m_graph.erase(m_id);
                                delete m_ptr;
                            }
                        }
                    }
                }
            };
    };
}

#endif