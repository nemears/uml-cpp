#ifndef SLOT_H
#define SLOT_H

#include "sequence.h"

namespace UML {

    class InstanceSpecification;
    class StructuralFeature;
    namespace Parsers {
        class SetOwningInstance;
    }

    class Slot : public Element {
        friend class UmlManager;
        friend class Parsers::SetOwningInstance;
        protected:
            Singleton<StructuralFeature, Slot> m_definingFeature = Singleton<StructuralFeature, Slot>(this);
            class RemoveDefiningFeatureProcedure : public AbstractSingletonProcedure<StructuralFeature, Slot> {
                public:
                    RemoveDefiningFeatureProcedure(Slot* me) : AbstractSingletonProcedure<StructuralFeature, Slot>(me) {};
                    void operator()(StructuralFeature* el) const override;
            };
            class AddDefiningFeatureProcedure : public AbstractSingletonProcedure<StructuralFeature, Slot> {
                public:
                    AddDefiningFeatureProcedure(Slot* me) : AbstractSingletonProcedure<StructuralFeature, Slot>(me) {};
                    void operator()(StructuralFeature* el) const override;
            };
            Sequence<ValueSpecification> m_values = Sequence<ValueSpecification>(this);
            Singleton<InstanceSpecification, Slot> m_owningInstance = Singleton<InstanceSpecification, Slot>(this);
            class RemoveOwningInstanceProcedure : public AbstractSingletonProcedure<InstanceSpecification, Slot> {
                public:
                    RemoveOwningInstanceProcedure(Slot* me) : AbstractSingletonProcedure<InstanceSpecification, Slot>(me) {};
                    void operator()(InstanceSpecification* el) const override;
            };
            class AddOwningInstanceProcedure : public AbstractSingletonProcedure<InstanceSpecification, Slot> {
                public:
                    AddOwningInstanceProcedure(Slot* me) : AbstractSingletonProcedure<InstanceSpecification, Slot>(me) {};
                    void operator()(InstanceSpecification* el) const override;
                    void operator()(ID id) const override;
            };
            void setManager(UmlManager* manager) override;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
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
            Slot();
        public:
            Slot(const Slot& rhs);
            virtual ~Slot();
            Sequence<ValueSpecification>& getValues();
            StructuralFeature* getDefiningFeature();
            StructuralFeature& getDefiningFeatureRef();
            ID getDefiningFeatureID() const;
            bool hasDefiningFeature() const;
            void setDefiningFeature(StructuralFeature& definingFeature);
            void setDefiningFeature(StructuralFeature* definingFeature);
            InstanceSpecification* getOwningInstance();
            InstanceSpecification& getOwningInstanceRef();
            ID getOwningInstanceID() const;
            bool hasOwningInstance() const;
            void setOwningInstance(InstanceSpecification& inst);
            void setOwningInstance(InstanceSpecification* inst);
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