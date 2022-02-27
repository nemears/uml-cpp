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
    };

    template <class T = Element>
    class UmlPtr : public AbstractUmlPtr {

        template <class U, class V> friend class Singleton;
        friend class UmlManager;

        private:
            UmlManager* m_manager;
            T* m_ptr = 0;
        protected:
            void reindex(ID newID, Element* el) override {
                m_id = newID;
                m_ptr = dynamic_cast<T*>(el);
            }
        public:
            T& operator*() const {
                if (m_id == ID::nullID()) {
                    throw NullPtrException();
                } else {
                    return *m_ptr;
                }
            };
            T* operator->() const {
                if (m_id == ID::nullID()) {
                    throw NullPtrException();
                } else {
                    return m_ptr;
                }
            };
            operator bool() const {
                return m_id != ID::nullID();
            };
            inline friend bool operator==(const T* lhs, const UmlPtr& rhs) {
                return lhs == rhs.m_ptr;
            };
            inline friend bool operator!=(const T* lhs, const UmlPtr& rhs) {
                return lhs != rhs.m_ptr;
            };
            inline friend bool operator==(const UmlPtr& lhs, const UmlPtr& rhs) {
                return lhs.m_id == rhs.m_id;
            };
            inline friend bool operator!=(const UmlPtr& lhs, const UmlPtr& rhs) {
                return lhs.m_id != rhs.m_id;
            };
            void operator=(const T* el) {
                if (el) {
                    m_id = el->getID();
                    m_ptr = const_cast<T*>(el);
                    m_manager = el->m_manager;
                    el->m_node->m_ptrs.push_back((void*) this);
                }
            };
            UmlPtr(const UmlPtr& rhs) {
                if (rhs.m_ptr) {
                    m_id = rhs.m_id;
                    m_ptr = rhs.m_ptr;
                    m_manager = rhs.m_manager;
                    m_ptr->m_node->m_ptrs.push_back((void*) this);
                }
            };
            T* ptr() {
                return m_ptr;
            };
            ID id() const {
                return m_id;
            };
            bool has() const {
                return m_id != ID::nullID();
            };
            void release() {
                if (m_ptr) {
                    m_manager->mountEl(*m_ptr);
                    m_manager->releaseNode(*m_ptr);
                    m_manager->m_graph.erase(m_id);
                    delete m_ptr;
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
                            if (!m_manager->m_lossless || !m_manager->m_mountBase.empty()) {
                                delete m_ptr;
                                // mountAndRelease();
                                m_manager->m_graph.erase(m_id);
                            }
                        }
                    }
                }
            };
    };
}

#endif