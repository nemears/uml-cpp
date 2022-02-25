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
                    if (!m_ptr) {
                        m_ptr = dynamic_cast<T&>(*m_manager->get<T>(m_id));
                    }
                    return *m_ptr;
                }
            };
            T* operator->() const {
                if (m_id == ID::nullID()) {
                    throw NullPtrException();
                } else {
                    if (!m_ptr) {
                        m_ptr = dynamic_cast<T&>(*m_manager->get<T>(m_id));
                    }
                    return m_ptr;
                }
            };
            bool has() const {
                return m_id == ID::nullID();
            };
            ID id() const {
                return m_id;
            };
            UmlPtr(T* el) {
                if (el) {
                    m_id = el->getID();
                    m_ptr = el;
                    m_manager = el->m_manager;
                }
            };
    };
}

#endif