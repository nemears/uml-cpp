#ifndef _UML_UML_PTR_H_
#define _UML_UML_PTR_H_

#include "element.h"

namespace UML {

    struct NullPtrException : public std::exception {
        const char* what() const  throw() override {
            return "tried to access null UmlPtr";
        };
    };

    template <class T = Element>
    class UmlPtr {
        private:
            UmlManager* m_manager;
            ID m_id = ID::nullID();
            T* m_ptr = 0;
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
                return m_ptr != 0;
            };
            bool has() const {
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
            ID id() const {
                return m_id;
            };
            UmlPtr(T* el) {
                if (el) {
                    m_id = el->getID();
                    m_ptr = el;
                    m_manager = el->m_manager;
                    el->m_node->m_ptrCount++;
                }
            };
            virtual ~UmlPtr() {
                if (m_id != ID::nullID()) {
                    if (!m_ptr) {
                        m_ptr = &m_manager->get<T>(m_id);
                    }
                    m_ptr->m_node->m_ptrCount--;
                    if (m_ptr->m_node->m_ptrCount == 0) {
                        if (!m_manager->m_lossless || !m_manager->m_mountBase.empty()) {
                            delete m_ptr;
                            // mountAndRelease
                            m_manager->m_graph.erase(m_id);
                        }
                    }
                }
            };
    };
}

#endif