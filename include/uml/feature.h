#ifndef FEATURE_H
#define FEATURE_H

#include "element.h"

namespace UML {

    class Classifier;

    class Feature : virtual public Element { // TODO redefineable element
        protected:
            Classifier* m_featuringClassifier;
            bool m_static;
        public:
            Classifier* getFeaturingClassifier();
            void setFeaturingClassifier(Classifier* clazz);
            bool isStatic();
            void setStatic(bool isStatic);
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif