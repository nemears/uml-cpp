#pragma once

#include "redefinableElement.h"
#include "uml/set/singleton.h"

namespace UML {

    class Classifier;
    typedef UmlPtr<Classifier> ClassifierPtr;

    class Feature : virtual public RedefinableElement {

        friend class Classifier;

        protected:
            Singleton<Classifier, Feature> m_featuringClassifier = Singleton<Classifier, Feature>(this);
            bool m_static = false;
            void referenceErased(ID id) override;
            Singleton<Classifier, Feature>& getFeaturingClassifierSingleton();
            Feature();
        public:
            ClassifierPtr getFeaturingClassifier() const;
            bool isStatic();
            void setStatic(bool isStatic);
            bool is(ElementType eType) const override;
            Feature& operator=(Feature&&) {
                return *this;
            };
            static ElementType elementType() {
                return ElementType::FEATURE;
            };
    };
}
