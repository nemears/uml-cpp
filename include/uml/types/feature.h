#pragma once

#include "redefinableElement.h"
#include "uml/set/singleton.h"
#include "uml/types/element.h"

namespace UML {

    class Classifier;
    typedef UmlPtr<Classifier> ClassifierPtr;

    class Feature : virtual public RedefinableElement {

        friend class Classifier;
        friend struct ElementInfo<Feature>;

        protected:
            Singleton<Classifier, Feature> m_featuringClassifier = Singleton<Classifier, Feature>(this);
            bool m_static = false;
            Singleton<Classifier, Feature>& getFeaturingClassifierSingleton();
            Feature(std::size_t elementType,AbstractManager& manager);
        public:
            ClassifierPtr getFeaturingClassifier() const;
            bool isStatic();
            void setStatic(bool isStatic);
            typedef TypeInfo<std::tuple<RedefinableElement>, Feature> Info;
    };

    template <>
    struct ElementInfo<Feature> {
        static const bool abstract = true;
        inline static const std::string name {"Feature"};
        static SetList sets(Feature& el) {
            return SetList {
                makeSetPair("featuringClassifier", el.m_featuringClassifier)
            };
        }
    };
}
