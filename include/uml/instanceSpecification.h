#ifndef INSTANCESPECIFICATIONH
#define INSTANCESPECIFICATIONH

#include "packageableElement.h"
#include "deployedArtifact.h"
#include "deploymentTarget.h"
#include "sequence.h"

namespace UML{

    class Slot;
    class Classifier;
    class InstanceValue;
    namespace Parsers {
        class InstanceSpecificationSetClassifier;
        class SetSpecification;
    }

    class InstanceSpecification : public PackageableElement , public DeployedArtifact, public DeploymentTarget {

        friend class UmlManager;
        friend class Classifier;
        friend class InstanceValue;
        friend class Parsers::InstanceSpecificationSetClassifier;
        friend class Parsers::SetSpecification;

        protected:
            Singleton<Classifier, InstanceSpecification> m_classifier = Singleton<Classifier, InstanceSpecification>(this);
            class RemoveClassifierProcedure : public AbstractSingletonProcedure<Classifier, InstanceSpecification> {
                public:
                    RemoveClassifierProcedure(InstanceSpecification* me) : AbstractSingletonProcedure<Classifier, InstanceSpecification>(me) {};
                    void operator()(Classifier* el) const override;
            };
            class AddClassifierProcedure : public AbstractSingletonProcedure<Classifier, InstanceSpecification>{
                public:
                    AddClassifierProcedure(InstanceSpecification * me) : AbstractSingletonProcedure<Classifier, InstanceSpecification>(me) {};
                    void operator()(Classifier* el) const override;
            };
            Sequence<Slot> m_slots = Sequence<Slot>(this);
            Singleton <ValueSpecification, InstanceSpecification> m_specification = Singleton<ValueSpecification, InstanceSpecification>(this);
            class RemoveSpecificationProcedure : public AbstractSingletonProcedure<ValueSpecification, InstanceSpecification> {
                public:
                    RemoveSpecificationProcedure(InstanceSpecification* me) : AbstractSingletonProcedure<ValueSpecification, InstanceSpecification>(me) {};
                    void operator()(ValueSpecification* el) const override;
            };
            class AddSpecificationProcedure : public AbstractSingletonProcedure<ValueSpecification, InstanceSpecification> {
                public:
                    AddSpecificationProcedure(InstanceSpecification* me) : AbstractSingletonProcedure<ValueSpecification, InstanceSpecification>(me) {};
                    void operator()(ValueSpecification* el) const override;
                    void operator()(ID id) const override;
            };
            class AddSlotFunctor : public TemplateAbstractSequenceFunctor<Slot,InstanceSpecification> {
                public:
                    AddSlotFunctor(InstanceSpecification* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Slot& el) const override;
                    void operator()(ID id) const override;
            };
            class RemoveSlotFunctor : public TemplateAbstractSequenceFunctor<Slot,InstanceSpecification> {
                public:
                    RemoveSlotFunctor(InstanceSpecification* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Slot& el) const override;
            };
            void setManager(UmlManager* manager) override;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            InstanceSpecification();
        public:
            InstanceSpecification(const InstanceSpecification& inst);
            virtual ~InstanceSpecification();
            Classifier* getClassifier();
            Classifier& getClassifierRef();
            bool hasClassifier() const;
            void setClassifier(Classifier* classifier);
            void setClassifier(Classifier& classifier);
            Sequence<Slot>& getSlots();
            ValueSpecification* getSpecification();
            ValueSpecification& getSpecificationRef();
            bool hasSpecification() const;
            void setSpecification(ValueSpecification* specification);
            void setSpecification(ValueSpecification& specification);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::INSTANCE_SPECIFICATION;
            };
    };
}

#endif