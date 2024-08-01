#pragma once

#include "redefinableElement.h"
#include "uml/set/singleton.h"

namespace UML {

    class Classifier;
    typedef UmlPtr<Classifier> ClassifierPtr;

    class Feature : virtual public RedefinableElement {

        friend class Classifier;

        protected:
            CustomSingleton<Classifier, Feature> m_featuringClassifier = CustomSingleton<Classifier, Feature>(this);
            bool m_static = false;
            void referenceErased(ID id) override;
            TypedSet<Classifier, Feature>& getFeaturingClassifierSingleton();
            Feature();
        public:
            ClassifierPtr getFeaturingClassifier() const;
            bool isStatic();
            void setStatic(bool isStatic);
            bool isSubClassOf(ElementType eType) const override;
            Feature& operator=(Feature&&) {
                return *this;
            };
            static ElementType elementType() {
                return ElementType::FEATURE;
            };
    };
}