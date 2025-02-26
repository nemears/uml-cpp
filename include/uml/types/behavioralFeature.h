#pragma once

#include "egm/egm-basic.h"
#include "uml/util/indexableSet.h"

namespace UML {
    template <class>
    class Feature;

    template <class>
    class Namespace;

    template <class>
    class Parameter;

    template <class ManagerPolicy>
    class BehavioralFeature : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<BehavioralFeature, EGM::TemplateTypeList<Feature, Namespace>>; 
        protected:
            using OwnedParameters = IndexableOrderedSet<Parameter, BehavioralFeature>;
            OwnedParameters m_ownedParameters = OwnedParameters(this);
        private:
            void init() {
                m_ownedParameters.subsets(ManagerPolicy::m_ownedMembers);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(BehavioralFeature);

            OwnedParameters& getOwnedParameters() { return m_ownedParameters; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::BehavioralFeature> {
        static const bool abstract = true;
        static std::string name() { return "BehavioralFeature"; }
        template <class Policy>
        static SetList sets(UML::BehavioralFeature<Policy>& el) {
            return SetList {
                make_set_pair("ownedParameters", el.getOwnedParameters())
            };
        }
    };
}
