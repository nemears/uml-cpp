#ifndef CREATE_OBJECT_ACTION_H
#define CREATE_OBJECT_ACTION_H

#include "action.h"
#include "classifier.h"

namespace UML {
    class CreateObjectAction : public Action {
        protected:
            Classifier* classifier;
        public:
            Classifier* getClassifier();
            void setClassifier(Classifier* classifier);
            CreateObjectAction() {
                classifier = NULL;
            };
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CREATE_OBJECT_ACTION;
            };
    };
}

#endif