#pragma once

#include "egm/egm-basic.h"

namespace UML {

    template <class>
    class Comment;

    template <class ManagerPolicy>
    class Element : public ManagerPolicy {
        friend struct EGM::ElementInfo<Element>;
        public:
            using Info = EGM::TypeInfo<Element>;
        protected:
            EGM::ReadOnlySet<Element, Element> m_ownedElements = EGM::ReadOnlySet<Element, Element>(this);
            EGM::ReadOnlySingleton<Element, Element> m_owner = EGM::ReadOnlySingleton<Element, Element>(this);
            EGM::Set<Comment, Element> m_ownedComments = EGM::Set<Comment, Element>(this);
        private:
            void init() {
                m_ownedElements.opposite(&decltype(m_owner)::ManagedType::getOwnerSingleton);
                m_ownedElements.setComposition(EGM::CompositionType::COMPOSITE);
                m_owner.opposite(&decltype(m_ownedElements)::ManagedType::getOwnedElements);
                m_owner.setComposition(EGM::CompositionType::ANTI_COMPOSITE);
                m_ownedComments.subsets(m_ownedElements);
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
            EGM::Set<Comment, Element>& getOwnedComments() {
                return m_ownedComments;
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
                make_set_pair("owner", el.m_owner),
                make_set_pair("ownedComments", el.m_ownedComments)
            };
        }
    };
}
