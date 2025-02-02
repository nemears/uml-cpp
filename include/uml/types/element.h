#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class ManagerPolicy>
    class Element : public ManagerPolicy {
        friend struct EGM::ElementInfo<Element>;
        public:
            using Info = EGM::TypeInfo<Element>;
        protected:
            EGM::ReadOnlySet<Element, Element> m_ownedElements = EGM::ReadOnlySet<Element, Element>(this);
            EGM::ReadOnlySingleton<Element, Element> m_owner = EGM::ReadOnlySingleton<Element, Element>(this);
        private:
            void init() {
                m_ownedElements.opposite(&decltype(m_owner)::ManagedType::getOwnerSingleton);
                m_owner.opposite(&decltype(m_ownedElements)::ManagedType::getOwnedElements);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Element);
            EGM::ReadOnlySet<Element, Element>& getOwnedElements() {
                return m_ownedElements;
            }
            EGM::ReadOnlySingleton<Element, Element>& getOwnerSingleton() {
                return m_owner;
            }
            using ElementPtr = EGM::ManagedPtr<Element>;
            ElementPtr getOwner() {
                return m_owner.get();
            }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Element> {
        static std::string name() { return "Element"; }
        static const bool abstract = true;
        template <class Policy>
        static SetList sets(UML::Element<Policy>& el) {
            return SetList {
                make_set_pair("ownedElements", el.m_ownedElements),
                make_set_pair("owner", el.m_owner)
            };
        }
    };
}
