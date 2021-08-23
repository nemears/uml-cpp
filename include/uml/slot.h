#ifndef SLOT_H
#define SLOT_H

#include "sequence.h"

namespace UML {

    class InstanceSpecification;
    class StructuralFeature;

    class Slot : public Element {
        friend class UmlManager;
        protected:
            ID m_definingFeatureID;
            StructuralFeature* m_definingFeaturePtr;
            Sequence<ValueSpecification> m_values;
            ID m_owningInstanceID;
            InstanceSpecification* m_owningInstancePtr;
            void reindexID(ID oldID, ID newID) override;
            void setManager(UmlManager* manager) override;
            class AddValueFunctor : public TemplateAbstractSequenceFunctor<ValueSpecification,Slot> {
                public:
                    AddValueFunctor(Slot* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(ValueSpecification& el) const override;
            };
            class RemoveValueFunctor : public TemplateAbstractSequenceFunctor<ValueSpecification,Slot> {
                public:
                    RemoveValueFunctor(Slot* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(ValueSpecification& el) const override;
            };
        public:
            Slot();
            ~Slot();
            Sequence<ValueSpecification>& getValues();
            StructuralFeature* getDefiningFeature();
            InstanceSpecification* getOwningInstance();
            void setOwningInstance(InstanceSpecification* inst);
            void setDefiningFeature(StructuralFeature* definingFeature);
            ElementType getElementType() const override;
            class NullDefiningFeatureException : public std::exception {
                public:
                    virtual const char* what() const throw() {
                        return "Tried to assign value to slot without setting definingFeature";
                    }
            } nullDefiningFeatureException;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::SLOT;
            };
    };
}

#endif