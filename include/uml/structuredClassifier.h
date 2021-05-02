#ifndef STRUCTURED_CLASSIFIER_H
#define STRUCTURED_CLASSIFIER_H

#include "classifier.h"
#include "connectableElement.h"

namespace UML {
    class StructuredClassifier : public Classifier {
        protected:
            Sequence<Property> m_ownedAttributes;
            Sequence<ConnectableElement> m_role;
            Sequence<Property> m_parts;
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
            class AddRoleFunctor : public AbstractSequenceFunctor {
                public:
                    AddRoleFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveRoleFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveRoleFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
        public:
            StructuredClassifier();
            StructuredClassifier(const StructuredClassifier& clazz);
            ~StructuredClassifier();
            Sequence<Property>& getOwnedAttributes();
            Sequence<ConnectableElement>& getRole();
            Sequence<Property>& getParts();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif