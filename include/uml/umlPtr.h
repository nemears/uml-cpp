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

    class AbstractPtr {
        
        friend struct ManagerNode;

        template <class Tlist, class P1, class P2>
        friend class Manager;
        
        protected:
            std::shared_ptr<NodeReference> m_node = 0;
            ID m_id = ID::nullID();
            virtual void releasePtr() = 0 ;
    };

    template <class T>
    class UmlPtr : public AbstractPtr {

        template <class U, class ApiPolicy, class V> friend class Singleton;
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
            std::shared_ptr<T> getPtr() const {
                if (m_id == ID::nullID()) {
                    throw NullPtrException();
                } else if (m_ptr) {
                    return m_ptr;
                } else {
                    AbstractElementPtr temp = m_node->m_node.m_manager.get(m_id);
                    if (!m_ptr) {
                        const_cast<UmlPtr<T>*>(this)->m_ptr = std::dynamic_pointer_cast<T>(temp->m_node->m_node.m_ptr);
                    }
                    const_cast<UmlPtr<T>*>(this)->m_node = m_ptr->m_node;
                    return  m_ptr;
                }            
            }
            void setFromRaw(const T* rawPtr) {
                if (rawPtr) {
                    m_id = rawPtr->getID();
                    m_node = rawPtr->m_node;
                    m_ptr = std::dynamic_pointer_cast<T>(m_node->m_node.m_ptr);
                    m_node->m_node.addPtr(this);
                } 
            }
        protected:
            std::shared_ptr<T> m_ptr = 0;
            long int m_ptrId = 0;
            void reindex(ID newID, AbstractElement* el) {
                m_id = newID;
                m_ptr = el;
                m_node = m_ptr->m_node;
            }
            void releasePtr() override {
                m_ptr = 0;
            }
            void erasePtr() {
                m_ptr = 0;
                m_node = 0;
            }
            template <class U = AbstractElement>
            void reassignPtr(const UmlPtr<U>& rhs) {
                if (m_ptr) {
                    m_ptr->m_node->m_node.removePtr(this);
                }
                m_id = rhs.m_id;
                m_ptr = 0;
                m_ptrId = 0;
                m_node = rhs.m_node;
                if (m_node) {
                    m_ptr = std::dynamic_pointer_cast<T>(rhs.m_ptr);
                    m_ptr->m_node->m_node.addPtr(this);
                }
            }
        public:
            T& operator*() const {
                return *getPtr();
            }
            std::shared_ptr<T> operator->() const {
                return getPtr(); 
            }
            operator bool() const {
                return m_id != ID::nullID();
            }
            inline friend bool operator==(const AbstractElement* lhs, const UmlPtr& rhs) {
                return lhs->m_id == rhs.m_id;
            }
            inline friend bool operator!=(const AbstractElement* lhs, const UmlPtr& rhs) {
                return lhs->m_id != rhs.m_id;
            }
            inline friend bool operator==(const UmlPtr& lhs, const UmlPtr& rhs) {
                return lhs.m_id == rhs.m_id;
            }
            inline friend bool operator!=(const UmlPtr& lhs, const UmlPtr& rhs) {
                return lhs.m_id != rhs.m_id;
            }
            template <class U>
            inline friend bool operator==(const UmlPtr& lhs, const UmlPtr<U>& rhs) {
                return lhs.id() == rhs.id();
            }
            template <class U>
            inline friend bool operator!=(const UmlPtr& lhs, const UmlPtr<U>& rhs) {
                return lhs.id() != rhs.id();
            }
            inline friend bool operator==(const AbstractElement& lhs, const UmlPtr& rhs) {
                return lhs.m_id == rhs.m_id;
            }
            inline friend bool operator!=(const AbstractElement& lhs, const UmlPtr& rhs) {
                return lhs.m_id != rhs.m_id;
            }
            void operator=(const T* el) {
                if (m_node) {
                    m_node->m_node.removePtr(this);
                }
                setFromRaw(el);
            }
            void operator=(const UmlPtr& rhs) {
                reassignPtr(rhs);
            }
            UmlPtr(const UmlPtr& rhs) {
                reassignPtr(rhs);
            }
            template <class U>
            UmlPtr(const UmlPtr<U>& rhs) {
                reassignPtr(rhs);
            }
            std::shared_ptr<T> ptr() {
                return getPtr();
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
                    m_node->m_node.m_manager.release(*m_ptr);
                }
            }
            void aquire() {
                AbstractElementPtr temp = m_node->m_node.m_manager.get(m_id);
                m_ptr = std::dynamic_pointer_cast<T>(temp.ptr());
                m_node = m_ptr->m_node;
            }
            UmlPtr(T* el) {
                setFromRaw(el);
            }
            UmlPtr() {
                // nothing
            }
            virtual ~UmlPtr() {
                if (m_id == ID::nullID()) {
                    return;
                }
                if (m_node) {
                    m_node->m_node.removePtr(this);
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
