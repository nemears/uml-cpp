#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class Namespace;

    template <class>
    class Type;

    template <class>
    class RedefinableElement;

    template <class>
    class Generalization;

    template <class>
    class Feature;

    template <class>
    class Property;

    template <class ManagerPolicy>
    class Classifier : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Classifier, EGM::TemplateTypeList<Namespace, Type, RedefinableElement>>;
        protected:
            using GeneralizationsSet = EGM::Set<Generalization, Classifier>;
            using FeaturesSet = EGM::ReadOnlySet<Feature, Classifier>;
            using AttributesSet = EGM::ReadOnlySet<Property, Classifier>;
            GeneralizationsSet m_generalizations = GeneralizationsSet(this);
            FeaturesSet m_features = FeaturesSet(this);
            AttributesSet m_attributes = AttributesSet(this);
        private:
            void init() {
                m_generalizations.subsets(ManagerPolicy::m_ownedElements);
                m_features.subsets(ManagerPolicy::m_ownedMembers);
                m_attributes.subsets(ManagerPolicy::m_features);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Classifier);
            GeneralizationsSet& getGeneralizations() {
                return m_generalizations;
            }
            FeaturesSet& getFeatures() { return m_features; }
            AttributesSet& getAttributes() { return m_attributes; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Classifier> {
        static const bool abstract = true;
        static std::string name() { return "Classifier"; }
        template <class Policy>
        static SetList sets(UML::Classifier<Policy>& el) {
            return SetList {
                make_set_pair("generalizations", el.getGeneralizations()),
                make_set_pair("features", el.getFeatures()),
                make_set_pair("attributes", el.getAttributes())
            };
        }
    };
}
