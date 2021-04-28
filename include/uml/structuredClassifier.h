#ifndef STRUCTURED_CLASSIFIER_H
#define STRUCTURED_CLASSIFIER_H

#include "classifier.h"

namespace UML {
    class StructuredClassifier : public Classifier {
        protected:
            Sequence<Property>* m_ownedAttributes;
            class AddOwnedAttributeFunctor : public AbstractSequenceFunctor {
                public:
                    AddOwnedAttributeFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveOwnedAttributeFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveOwnedAttributeFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
        public:
            StructuredClassifier();
            ~StructuredClassifier();
            Sequence<Property>& getOwnedAttributes();
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif