#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class Feature;

    template <class>
    class Namespace;

    template <class ManagerPolicy>
    class BehavioralFeature : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<BehavioralFeature, EGM::TemplateTypeList<Feature, Namespace>>; 
        private:
            void init() {}
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(BehavioralFeature);
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::BehavioralFeature> {
        static const bool abstract = true;
        static std::string name() { return "BehavioralFeature"; }
    };
}
