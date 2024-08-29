#pragma once

#include <memory>
#include "uml/id.h"
#include "uml/managers/typeID.h"

namespace UML {

    class AbstractManager;
    struct NodeReference;
    template <class T>
    class UmlPtr;

    // non template class
    class AbstractElement {

        template <class T>
        friend class UmlPtr;

        friend struct ManagerNode;

        protected:
            const std::size_t m_elementType;
            AbstractManager& m_manager;
            std::shared_ptr<NodeReference> m_node = 0;
            ID m_id = ID::randomID();
            AbstractElement(std::size_t elementType, AbstractManager& manager) : m_elementType(elementType), m_manager(manager) {}
        public:
            ID getID() const {
                return m_id;
            }
            void setID(ID id);
            template <class T>
            bool is() {
                return is<T>(m_elementType);
            }
            template <class T>
            T& as() {
                if (!is<T>()) {
                    // TODO throw
                }
                return dynamic_cast<T&>(*this);
            }
    };

    // typed for api
    // TList is a std::tuple<> with all of the types your manager can make
    template <class Tlist>
    class BaseElement : public AbstractElement, public ManagerTypes<Tlist> {
        protected:
            BaseElement(std::size_t elementType, AbstractManager& manager) : AbstractElement(elementType, manager) {}
    };
}
