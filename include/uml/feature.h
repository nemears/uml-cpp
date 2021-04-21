#ifndef FEATURE_H
#define FEATURE_H

#include "namedElement.h"

namespace UML {

    class Classifier;

    class Feature : virtual public NamedElement { // TODO redefineable element
        protected:
            Classifier* m_featuringClassifier;
            bool m_static;
        public:
            Feature();
            Feature(const Feature& feature);
            Classifier* getFeaturingClassifier();
            void setFeaturingClassifier(Classifier* clazz);
            bool isStatic();
            void setStatic(bool isStatic);
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
            Feature& operator=(Feature&&) {
                return *this;
            };
    };
}

#endif