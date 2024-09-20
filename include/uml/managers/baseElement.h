#pragma once

#include <memory>
#include "uml/id.h"
#include "uml/managers/managerTypes.h"

namespace UML {

    class AbstractManager;
    struct ManagerNode;
    template <class T>
    class UmlPtr;
    class AbstractSet;

    // non template class
    class AbstractElement {

        template <class T>
        friend class UmlPtr;

        friend struct ManagerNode;

        template <class T, class U, class DataTypePolicy, class ApiPolicy>
        friend class PrivateSet;

        template <class T, class U, class ApiPolicy>
        friend class Singleton;

        template <class T, class U, class ApiPolicy>
        friend class Set;
        
        template <class T, class U, class ApiPolicy>
        friend class OrderedSet;

        template <class Tlist, class P1, class P2>
        friend class Manager;

        protected:
            const std::size_t m_elementType;
            AbstractManager& m_manager;
            std::weak_ptr<ManagerNode> m_node;
            ID m_id = ID::randomID();

            void addToReadonlySet(AbstractSet& set, AbstractElement& el);
            void removeFromReadonlySet(AbstractSet& set, AbstractElement& el);

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
        private:
            class BadUmlCast: public std::exception {
                public:
                    virtual const char* what() const throw() {
                        return "tried to access multiplicity but it was not specified yet";
                    }
            } badCastException;
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
                    throw badCastException;
                }
                return dynamic_cast<T&>(*this);
            }
    };
}
