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