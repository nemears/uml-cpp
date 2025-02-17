#pragma once

#include "egm/egm-basic.h"
#include "uml/util/boolDataPolicy.h"

namespace UML {
    template <class>
    class RedefinableElement;

    template <class>
    class Classifier;

    template <class ManagerPolicy>
    class Feature : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Feature, EGM::TemplateTypeList<RedefinableElement>>;
        protected:
            using FeaturingClassifierSingleton = EGM::ReadOnlySingleton<Classifier, Feature>;
            FeaturingClassifierSingleton m_featuringClassifer = FeaturingClassifierSingleton(this);
            bool m_static = false;
        private:
            void init() {}
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Feature);
            FeaturingClassifierSingleton& getFeaturingClassifierSingleton() { return m_featuringClassifer; }
            using ClassifierPtr = EGM::ManagedPtr<typename FeaturingClassifierSingleton::ManagedType>;
            ClassifierPtr getFeaturingClassifier() { return m_featuringClassifer.get(); }
            bool isStatic() const { return m_static; }
            void setStatic(bool val) { m_static = val; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Feature> {
        static const bool abstract = true;
        static std::string name() { return "Feature"; }
        template <class Policy>
        static SetList sets(UML::Feature<Policy>& el) {
            return SetList {
                make_set_pair("featuringClassifier", el.getFeaturingClassifierSingleton())
            };
        }
        template <class Policy>
        struct StaticPolicy : public UML::BooleanDataPolicy {
            EGM::ManagedPtr<UML::Feature<Policy>> el;
            StaticPolicy(UML::Feature<Policy>& ref) : el(&ref) {}
            bool getBool() override { return el->isStatic(); }
            void setBool(bool b) override { el->setStatic(b); }
        };
        template <class Policy>
        static DataList data(UML::Feature<Policy>& el) {
            return DataList {
                createDataPair<StaticPolicy<Policy>>("static", el)
            };
        }
    };
}
