#pragma once

#include "egm/egm-basic.h"
#include "egm/manager/typeInfo.h"

namespace UML {
    template <class>
    class Element;

    template <class>
    class Relationship;

    template <class ManagerPolicy>
    class DirectedRelationship : public ManagerPolicy {
        friend struct EGM::ElementInfo<DirectedRelationship>;
        public:
            using Info = EGM::TypeInfo<DirectedRelationship, EGM::TemplateTypeList<Relationship>>;
        protected:
            using TargetAndSourceSet = EGM::ReadOnlySet<Element, DirectedRelationship>;
            TargetAndSourceSet m_targets = TargetAndSourceSet(this);
            TargetAndSourceSet m_sources = TargetAndSourceSet(this);
        private:
            void init() {
                m_targets.subsets(ManagerPolicy::m_relatedElements);
                m_sources.subsets(ManagerPolicy::m_relatedElements);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(DirectedRelationship);
            TargetAndSourceSet& getTargets() { return m_targets; }
            TargetAndSourceSet& getSources() { return m_sources; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::DirectedRelationship> {
        static const bool abstract = true;
        static std::string name() { return "DirectedRelationship"; }
        template <class Policy>
        static SetList sets(UML::DirectedRelationship<Policy>& el) {
            return SetList {
                make_set_pair("targets", el.m_targets),
                make_set_pair("sources", el.m_sources)
            };
        }
    };
}
