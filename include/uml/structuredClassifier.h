#ifndef STRUCTURED_CLASSIFIER_H
#define STRUCTURED_CLASSIFIER_H

#include "classifier.h"

namespace UML {

    class ConnectableElement;

    class StructuredClassifier : virtual public Classifier {
        protected:
            Set<Property, StructuredClassifier> m_ownedAttributes = Set<Property, StructuredClassifier>(this);
            // Sequence<ConnectableElement> m_role = Sequence<ConnectableElement>(this);
            // Sequence<Property> m_parts = Sequence<Property>(this);
            // class AddOwnedAttributeFunctor : public TemplateAbstractSequenceFunctor<Property,StructuredClassifier> {
            //     public:
            //         AddOwnedAttributeFunctor(StructuredClassifier* me) : TemplateAbstractSequenceFunctor(me) {};
            //         void operator()(Property& el) const override;
            //         void operator()(ID id) const override;
            // };
            // class RemoveOwnedAttributeFunctor : public TemplateAbstractSequenceFunctor<Property,StructuredClassifier> {
            //     public:
            //         RemoveOwnedAttributeFunctor(StructuredClassifier* me) : TemplateAbstractSequenceFunctor(me) {};
            //         void operator()(Property& el) const override;
            // };
            // class AddRoleFunctor : public TemplateAbstractSequenceFunctor<ConnectableElement,StructuredClassifier> {
            //     public:
            //         AddRoleFunctor(StructuredClassifier* me) : TemplateAbstractSequenceFunctor(me) {};
            //         void operator()(ConnectableElement& el) const override;
            //         void operator()(ID id) const override;
            // };
            // class RemoveRoleFunctor : public TemplateAbstractSequenceFunctor<ConnectableElement,StructuredClassifier> {
            //     public:
            //         RemoveRoleFunctor(StructuredClassifier* me) : TemplateAbstractSequenceFunctor(me) {};
            //         void operator()(ConnectableElement& el) const override;
            // };
            // class AddPartFunctor : public TemplateAbstractSequenceFunctor<Property, StructuredClassifier> {
            //     public:
            //         AddPartFunctor(StructuredClassifier* me) : TemplateAbstractSequenceFunctor(me) {};
            //         void operator()(Property& el) const override;
            // };
            // class RemovePartFunctor : public TemplateAbstractSequenceFunctor<Property, StructuredClassifier> {
            //     public:
            //         RemovePartFunctor(StructuredClassifier* me) : TemplateAbstractSequenceFunctor(me) {};
            //         void operator()(Property& el) const override;
            // };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID);
            void restoreReferences() override;
            void referenceErased(ID id) override;
            void init();
            void copy(const StructuredClassifier& rhs);
            StructuredClassifier();
        public:
            StructuredClassifier(const StructuredClassifier& clazz);
            virtual ~StructuredClassifier();
            Set<Property, StructuredClassifier>& getOwnedAttributes();
            // Sequence<ConnectableElement>& getRole();
            // Sequence<Property>& getParts();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::STRUCTURED_CLASSIFIER;
            };
    };
}

#endif