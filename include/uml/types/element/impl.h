#pragma once

#include "definition.h"
#include "uml/managers/abstractManager.h"
#include "uml/managers/dummyManager.h"
#include "uml/set/set.h"
#include "uml/set/singleton.h"
#include <cstddef>

namespace UML {
    template <class ManagerPolicy>
    class Element : public ElementDefinition<ManagerPolicy> {
        protected:
            // ReadOnlySet<ElementDefinition, Element> m_ownedElemnts = ReadOnlySet<ElementDefinition, Element>(this);
            // ReadOnlySingleton<ElementDefinition, Element> m_owner = ReadOnlySingleton<ElementDefinition, Element>(this);
            void init() {
                // m_ownedElemnts.opposite(&decltype(m_owner)::ManagedElement::getOwnerSingleton);
                // m_owner.opposite(&decltype(m_ownedElemnts)::ManagedElement::getOwnedElements);
            }
            static constexpr std::size_t elementElementType = ManagerPolicy::Manager::template ElementType<Element>::result;
            Element() :
                ManagerPolicy::Manager::BaseElement(elementElementType, dummyManager),
                ElementDefinition<ManagerPolicy>(elementElementType, dummyManager)
            
            {
                init();
            }
        public:
            // using ElementImpl = ElementDefinition<ManagerPolicy>::ElementImpl;
            // using ElementPtr = ElementDefinition<ManagerPolicy>::ElementPtr;
            // TODO delete when enforce abstract
            Element(std::size_t elementType, AbstractManager& manager) :
                ManagerPolicy::Manager::BaseElement(elementType, manager),
                ElementDefinition<ManagerPolicy>(elementType, manager)
            {
                init();
            }
            // ReadOnlySet<ElementDefinition, ElementImpl>& getOwnedElements() override {
            //     return m_ownedElemnts;
            // }
            // ReadOnlySingleton<ElementDefinition, ElementImpl>& getOwnerSingleton() override {
            //     return m_owner;
            // }
            // ElementPtr getOwner() override {
            //     return m_owner.get();
            // }
    };
}
