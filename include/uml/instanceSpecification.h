#ifndef INSTANCESPECIFICATIONH
#define INSTANCESPECIFICATIONH

#include "packageableElement.h"
#include "deployedArtifact.h"
#include "deploymentTarget.h"
#include "sequence.h"

namespace UML{

    class Slot;
    class Classifier;

    class InstanceSpecification : public PackageableElement , public DeployedArtifact, public DeploymentTarget {
        friend class UmlManager;
        protected:
            ID m_classifierID;
            Classifier* m_classifierPtr;
            Sequence<Slot> m_slots;
            ID m_specificationID;
            ValueSpecification* m_specificationPtr;
            class AddSlotFunctor : public AbstractSequenceFunctor {
                public:
                    AddSlotFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveSlotFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveSlotFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            void setManager(UmlManager* manager) override;
        public:
            InstanceSpecification();
            InstanceSpecification(const InstanceSpecification& inst);
            virtual ~InstanceSpecification();
            Classifier* getClassifier();
            void setClassifier(Classifier* classifier);
            Sequence<Slot>& getSlots();
            ValueSpecification* getSpecification();
            void setSpecification(ValueSpecification* specification);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::INSTANCE_SPECIFICATION;
            };
    };
}

#endif