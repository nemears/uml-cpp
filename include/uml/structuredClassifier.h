#ifndef STRUCTURED_CLASSIFIER_H
#define STRUCTURED_CLASSIFIER_H

#include "classifier.h"

namespace UML {
    class StructuredClassifier : public Classifier {
        protected:
            Sequence<Property>* m_ownedAttributes;
        public:
            StructuredClassifier();
            ~StructuredClassifier();
            Sequence<Property>& getOwnedAttributes();
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif