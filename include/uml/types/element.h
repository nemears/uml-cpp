#pragma once

#include "uml/managers/abstractManager.h"
#include "uml/managers/dummyManager.h"
#include "uml/managers/typeInfo.h"
#include "uml/set/set.h"
#include "uml/set/singleton.h"
#include <cstddef>

namespace UML {
    template <class ManagerPolicy>
    class Element : public ManagerPolicy {
        public:
            using Info = TypeInfo<Element>;
        protected:
            ReadOnlySet<Element, Element> m_ownedElements = ReadOnlySet<Element, Element>(this);
            ReadOnlySingleton<Element, Element> m_owner = ReadOnlySingleton<Element, Element>(this);
            void init() {
                m_ownedElements.opposite(&decltype(m_owner)::ManagedType::getOwnerSingleton);
                m_owner.opposite(&decltype(m_ownedElements)::ManagedType::getOwnedElements);
            }
            static constexpr std::size_t elementElementType = ManagerPolicy::manager::template ElementType<Element>::result;
            Element() :
                ManagerPolicy::manager::BaseElement(elementElementType, dummyManager),
                ManagerPolicy(elementElementType, dummyManager)
            {
                init();
            }
        public:
            // TODO delete when enforce abstract
            Element(std::size_t elementType, AbstractManager& manager) :
                ManagerPolicy::manager::BaseElement(elementType, manager),
                ManagerPolicy(elementType, manager)
            {
                init();
            }
            ReadOnlySet<Element, Element>& getOwnedElements() {
                return m_ownedElements;
            }
            ReadOnlySingleton<Element, Element>& getOwnerSingleton() {
                return m_owner;
            }
            using ElementPtr = UmlPtr<Element>;
            ElementPtr getOwner() {
                return m_owner.get();
            }
    };

    template <>
    struct ElementInfo<Element> : public DefaultInfo {};
}
