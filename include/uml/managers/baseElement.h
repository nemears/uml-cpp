#pragma once

#include <memory>
#include "uml/id.h"
#include "uml/managers/typeID.h"

namespace UML {

    class AbstractManager;
    struct ManagerNode;
    template <class T>
    class UmlPtr;

    // non template class
    class AbstractElement {

        template <class T>
        friend class UmlPtr;

        friend struct ManagerNode;

        template <class T, class U, class DataTypePolicy, class ApiPolicy>
        friend class PrivateSet;

        template <class T, class U, class ApiPolicy>
        friend class Singleton;
        
        template <class Tlist, class P1, class P2>
        friend class Manager;

        protected:
            const std::size_t m_elementType;
            AbstractManager& m_manager;
            std::weak_ptr<ManagerNode> m_node;
            ID m_id = ID::randomID();
            AbstractElement(std::size_t elementType, AbstractManager& manager) : m_elementType(elementType), m_manager(manager) {}
        public:
            virtual ~AbstractElement() {}
            AbstractElement(const AbstractElement&) = delete;
            AbstractElement& operator=(const AbstractElement&) = delete;
            ID getID() const {
                return m_id;
            }
            void setID(ID id);
            inline friend bool operator==(const AbstractElement& lhs, const AbstractElement& rhs) {
                return lhs.m_id == rhs.m_id;
            };
            inline friend bool operator!=(const AbstractElement& lhs, const AbstractElement& rhs) {
                return lhs.m_id != rhs.m_id;
            };
            std::size_t getElementType() const {
                return m_elementType;
            }
    };

    // typed for api
    // TList is a std::tuple<> with all of the types your manager can make
    template <class Tlist>
    class BaseElement : public AbstractElement, public ManagerTypes<Tlist> {
        protected:
            BaseElement(std::size_t elementType, AbstractManager& manager) : AbstractElement(elementType, manager) {}
        public:
            template <class T>
            bool is() {
                return ManagerTypes<Tlist>::template is<T>(m_elementType);
            }
            
            template <class T>
            T& as() {
                if (!is<T>()) {
                    // TODO throw
                }
                return dynamic_cast<T&>(*this);
            }
    };
}
