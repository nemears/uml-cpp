#pragma once

#include "managers/baseElement.h"
#include "managers/managerNode.h"
#include "managers/abstractManager.h"
#include <memory>

namespace std {
    template <class T>
    struct hash<UML::UmlPtr<T>> {
        std::size_t operator()(const UML::UmlPtr<T>& ptr) const;
    };
}

namespace UML {

    struct NullPtrException : public std::exception {
        const char* what() const  throw() override {
            return "tried to access null UmlPtr";
        };
    };

    template <
                class T,
                class DataTypePolicy,
                class ApiPolicy,
                class U
            >
    class PrivateSet;

    template <class T>
    class UmlPtr {

        template <class U, class ApiPolicy, class V> friend class Singleton;
 //       template <class SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class AbstractAccessPolicy;
        template <class U> friend class UmlPtr;
        template <
                class V,
                class DataTypePolicy,
                class ApiPolicy,
                class U
            >
        friend class PrivateSet;

        private:
            std::shared_ptr<T> m_ptr = 0;
            std::shared_ptr<NodeReference> m_node = 0;
        protected:
            ID m_id = ID::nullID();
            long int m_ptrId = 0;
            void reindex(ID newID, Element* el) {
                m_id = newID;
                m_ptr = dynamic_cast<T*>(el);
                m_node = m_ptr->m_node;
            }
            void releasePtr() {
                m_ptr = 0;
            }
            void erasePtr() {
                m_ptr = 0;
                m_node = 0;
            }
            template <class U = Element>
            void reassignPtr(const UmlPtr<U>& rhs) {
                if (m_ptr) {
                    m_manager->removePtr(*this);
                    m_manager->destroyPtr(*this);
                }
                m_id = rhs.m_id;
                m_manager = rhs.m_manager;
                m_ptr = 0;
                m_ptrId = 0;
                m_node = rhs.m_node;
                if (m_node) {
                    m_ptr = dynamic_cast<T*>(rhs.m_ptr);
                    m_manager->assignPtr(*this);
                }
            }
        public:
            T& operator*() const {
                if (m_id == ID::nullID()) {
                    throw NullPtrException();
                } else if (m_ptr) {
                    return *m_ptr;
                } else {
                    ElementPtr temp = m_manager->get(m_id);
                    if (!m_ptr) {
                        const_cast<UmlPtr<T>*>(this)->m_ptr = dynamic_cast<T*>(temp.ptr());
                    }
                    const_cast<UmlPtr<T>*>(this)->m_node = m_ptr->m_node;
                    return  *dynamic_cast<T*>(temp.ptr());
                }
            }
            T* operator->() const {
                if (m_id == ID::nullID()) {
                    throw NullPtrException();
                } else if (m_ptr) {
                    return m_ptr;
                } else {
                    ElementPtr temp = m_manager->get(m_id);
                    if (!m_ptr) {
                        const_cast<UmlPtr<T>*>(this)->m_ptr = dynamic_cast<T*>(temp.ptr());
                    }
                    const_cast<UmlPtr<T>*>(this)->m_node = m_ptr->m_node;
                    return  dynamic_cast<T*>(temp.ptr());
                }
            }
            operator bool() const {
                return m_id != ID::nullID();
            }
            inline friend bool operator==(const Element* lhs, const UmlPtr& rhs) {
                return lhs->m_id == rhs.m_id;
            }
            inline friend bool operator!=(const Element* lhs, const UmlPtr& rhs) {
                return lhs->m_id != rhs.m_id;
            }
            inline friend bool operator==(const UmlPtr& lhs, const UmlPtr& rhs) {
                return lhs.m_id == rhs.m_id;
            }
            inline friend bool operator!=(const UmlPtr& lhs, const UmlPtr& rhs) {
                return lhs.m_id != rhs.m_id;
            }
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
            }
            inline friend bool operator!=(const Element& lhs, const UmlPtr& rhs) {
                return lhs.m_id != rhs.m_id;
            }
            void operator=(const T* el) {
                if (m_manager) {
                    m_manager->removePtr(*this);
                    m_manager->destroyPtr(*this);
                }
                if (el) {
                    m_id = el->getID();
                    m_ptr = const_cast<T*>(el);
                    m_node = m_ptr->m_node;
                    m_manager = el->m_manager;
                    m_ptrId = 0;
                    m_manager->assignPtr(*this);
                }
            }
            void operator=(const UmlPtr& rhs) {
                reassignPtr(rhs);
            }
            UmlPtr(const UmlPtr& rhs) {
                reassignPtr(rhs);
            }
            template <class U = Element>
            UmlPtr(const UmlPtr<U>& rhs) {
                reassignPtr(rhs);
            }
            T* ptr() {
                if (m_id == ID::nullID()) {
                    return 0;
                } else if (m_ptr) {
                    return m_ptr;
                } else {
                    ElementPtr temp = m_manager->get(m_id);
                    if (!m_ptr) {
                        const_cast<UmlPtr<T>*>(this)->m_ptr = dynamic_cast<T*>(temp.ptr());
                    }
                    const_cast<UmlPtr<T>*>(this)->m_node = m_ptr->m_node;
                    return  dynamic_cast<T*>(temp.ptr());
                }
            }
            ID id() const {
                return m_id;
            }
            bool has() const {
                return m_id != ID::nullID();
            }
            bool loaded() const {
                if (m_id != ID::nullID()) {
                    return m_ptr != 0;
                }
                return false;
            }
            void release() {
                if (m_ptr) {
                    m_manager->release(*m_ptr);
                }
            }
            void aquire() {
                ElementPtr temp = m_manager->get(m_id);
                m_ptr = dynamic_cast<T*>(temp.ptr());
                m_node = m_ptr->m_node;
                m_manager->restorePtr(*this);
            }
            UmlPtr(T* el) {
                if (el) {
                    m_id = el->getID();
                    m_ptr = el;
                    m_manager = el->m_manager;
                    m_node = m_ptr->m_node;
                    m_manager->assignPtr(*this);
                }
            }
            UmlPtr() {
                // nothing
            }
            virtual ~UmlPtr() {
                if (m_id == ID::nullID()) {
                    return;
                }
                if (m_node) {
                    m_manager->removePtr(*this);
                    m_manager->destroyPtr(*this);
                }
            }
    };
}

// hash operator injection
namespace std {
    template <class T> 
    std::size_t hash<UML::UmlPtr<T>>::operator()(const UML::UmlPtr<T>& ptr) const {

        /**
         * TODO better hashing
         **/

        hash<string> hasher;
        return hasher(ptr.id().string() /*+ to_string(ptr.m_ptrId)*/);
    }
}
