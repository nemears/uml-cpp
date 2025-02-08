#pragma once

#include "egm/egm-basic.h"

namespace UML {
    
    template <class>
    class Element;

    template <class ManagerPolicy>
    class Relationship : public ManagerPolicy {
        friend struct EGM::ElementInfo<Relationship>;
        public:
            using Info = EGM::TypeInfo<Relationship, EGM::TemplateTypeList<Element>>;
        protected:
            using RelatedElementSet = EGM::ReadOnlySet<Element, Relationship>;
            RelatedElementSet m_relatedElements = RelatedElementSet(this);
        private:
            void init() {}
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Relationship);
            RelatedElementSet& getRelatedElements() {
                return m_relatedElements;
            }
    };
}

namespace EGM {
    template<>
    struct ElementInfo<UML::Relationship> {
        static const bool abstract = true;
        static std::string name() { return "Relationship"; }
        template <class Policy>
        static SetList sets(UML::Relationship<Policy>& el) {
            return SetList {
                make_set_pair("relatedElements", el.m_relatedElements)
            };
        }
    };
}
