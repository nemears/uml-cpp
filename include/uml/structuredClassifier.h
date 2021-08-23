#ifndef STRUCTURED_CLASSIFIER_H
#define STRUCTURED_CLASSIFIER_H

#include "classifier.h"

namespace UML {

    class ConnectableElement;

    class StructuredClassifier : virtual public Classifier {
        protected:
            Sequence<Property> m_ownedAttributes = Sequence<Property>(this);
            Sequence<ConnectableElement> m_role = Sequence<ConnectableElement>(this);
            Sequence<Property> m_parts = Sequence<Property>(this);
            class AddOwnedAttributeFunctor : public TemplateAbstractSequenceFunctor<Property,StructuredClassifier> {
                public:
                    AddOwnedAttributeFunctor(StructuredClassifier* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Property& el) const override;
            };
            class RemoveOwnedAttributeFunctor : public TemplateAbstractSequenceFunctor<Property,StructuredClassifier> {
                public:
                    RemoveOwnedAttributeFunctor(StructuredClassifier* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Property& el) const override;
            };
            class AddRoleFunctor : public TemplateAbstractSequenceFunctor<ConnectableElement,StructuredClassifier> {
                public:
                    AddRoleFunctor(StructuredClassifier* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(ConnectableElement& el) const override;
            };
            class RemoveRoleFunctor : public TemplateAbstractSequenceFunctor<ConnectableElement,StructuredClassifier> {
                public:
                    RemoveRoleFunctor(StructuredClassifier* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(ConnectableElement& el) const override;
            };
            void setManager(UmlManager* manager) override;
            void restoreReleased(ID id, Element* released) override;
            void referencingReleased(ID id) override;
        public:
            StructuredClassifier();
            StructuredClassifier(const StructuredClassifier& clazz);
            virtual ~StructuredClassifier();
            Sequence<Property>& getOwnedAttributes();
            Sequence<ConnectableElement>& getRole();
            Sequence<Property>& getParts();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::STRUCTURED_CLASSIFIER;
            };
    };
}

#endif