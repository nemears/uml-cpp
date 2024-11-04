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

        template <class Tlist, class P1>
        friend class Manager;
        
        protected:
            std::weak_ptr<ManagerNode> m_node;
            ID m_id = ID::nullID();
            virtual void setPtr(std::shared_ptr<AbstractElement> ptr) = 0;
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
            std::weak_ptr<T> getPtr() const {
                if (m_id == ID::nullID()) {
                    throw NullPtrException();
                } else if (m_ptr.lock()) {
                    return m_ptr;
                } else {
                    AbstractElementPtr temp = m_node.lock()->m_manager.abstractGet(m_id);
                    if (!m_ptr.lock()) {
                        const_cast<UmlPtr<T>*>(this)->m_ptr = std::dynamic_pointer_cast<T>(temp->m_node.lock()->m_ptr);
                    }
                    const_cast<UmlPtr<T>*>(this)->m_node = m_ptr.lock()->m_node.lock();
                    return  m_ptr;
                }
            }
            void setFromRaw(const T* rawPtr) {
                if (rawPtr) {
                    m_id = rawPtr->getID();
                    m_node = rawPtr->m_node;
                    m_ptr = std::dynamic_pointer_cast<T>(m_node.lock()->m_ptr);
                    m_node.lock()->addPtr(this);
                } 
            }
        protected:

            std::weak_ptr<T> m_ptr;

            void reindex(ID newID, AbstractElement* el) {
                m_id = newID;
                m_ptr = el;
                m_node = m_ptr.lock()->m_node;
            }
            void setPtr(std::shared_ptr<AbstractElement> ptr) override {
                m_ptr = std::dynamic_pointer_cast<T>(ptr);
            }
            template <class U = AbstractElement>
            void reassignPtr(const UmlPtr<U>& rhs) {
                auto myPtr = m_ptr.lock();
                if (myPtr) {
                    myPtr->m_node.lock()->removePtr(this);
                }
                m_id = rhs.m_id;
                m_node = rhs.m_node;
                if (m_node.lock()) {
                    m_ptr = std::dynamic_pointer_cast<T>(rhs.m_ptr.lock());
                    m_node.lock()->addPtr(this);
                }
            }
        public:
            T& operator*() const {
                return *getPtr().lock();
            }
            std::shared_ptr<T> operator->() const {
                return getPtr().lock(); 
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
                if (m_node.lock()) {
                    m_node.lock()->removePtr(this);
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
                return getPtr().lock();
            }
            ID id() const {
                return m_id;
            }
            bool has() const {
                return m_id != ID::nullID();
            }
            bool loaded() const {
                if (m_id != ID::nullID()) {
                    return m_ptr.lock() != 0;
                }
                return false;
            }
            void release() {
                if (!m_ptr.lock()) {
                    return;
                }
                m_node.lock()->m_manager.release(*m_ptr.lock());
            }
            void aquire() {
                AbstractElementPtr temp = m_node.lock()->m_manager.abstractGet(m_id);
                m_ptr = std::dynamic_pointer_cast<T>(temp.ptr());
                m_node = m_ptr.lock()->m_node.lock();
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
                if (m_node.lock()) {
                    m_node.lock()->removePtr(this);
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
        return hasher(ptr.id().string());
    }
}
