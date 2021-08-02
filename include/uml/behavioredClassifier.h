#ifndef BEHAVIORED_CLASSIFIER_H
#define BEHAVIORED_CLASSIFIER_H

#include "uml/classifier.h"

namespace UML {
    class BehavioredClassifier : public Classifier {
        public:
            BehavioredClassifier();
            BehavioredClassifier(const BehavioredClassifier& classifier);
            virtual ~BehavioredClassifier();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::BEHAVIORED_CLASSIFIER;
            };
    };
}

#endif