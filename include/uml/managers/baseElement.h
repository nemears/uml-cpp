#pragma once

#include <memory>
#include "uml/id.h"
#include "uml/managers/typeID.h"

namespace UML {

    class AbstractManager;
    class NodeReference;

    // non template class
    class AbstractElement {
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
    
    };

    // typed for api
    template <class Tlist>
    class BaseElement : public AbstractElement, public ManagerTypes<Tlist> {
    };
}
