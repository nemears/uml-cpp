#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class NamedElement;

    template <class>
    class Classifier;

    template <class ManagerPolicy>
    class RedefinableElement : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<RedefinableElement, EGM::TemplateTypeList<NamedElement>>;
        protected:
            using RedefinedElementsSet = EGM::ReadOnlySet<RedefinableElement, RedefinableElement>;
            using RedefinitionContextSet = EGM::ReadOnlySet<Classifier, RedefinableElement>;
            RedefinedElementsSet m_redefinedElements = RedefinedElementsSet(this);
            RedefinitionContextSet m_redefinitionContext = RedefinitionContextSet(this);
        private:
            void init() {
                
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(RedefinableElement);
            RedefinedElementsSet& getRedefinedElements() { return m_redefinedElements; }
            RedefinitionContextSet& getRedefinitionContext() { return m_redefinitionContext; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::RedefinableElement> {
        static const bool abstract = true;
        static std::string name() { return "RedefinableElement"; }
        template <class Policy>
        static SetList sets(UML::RedefinableElement<Policy>& el) {
            return SetList {
                make_set_pair("redefinedElements", el.getRedefinedElements()),
                make_set_pair("redefinitionContext", el.getRedefinitionContext())
            };
        }
    };
}
