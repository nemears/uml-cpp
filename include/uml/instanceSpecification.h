#ifndef _UML_INSTANCE_SPECIFICATION_H_
#define _UML_INSTANCE_SPECIFICATION_H_

#include "packageableElement.h"
#include "deployedArtifact.h"
#include "deploymentTarget.h"
#include "slot.h"

namespace UML{

    class Slot;
    class Classifier;
    class InstanceValue;
    class ValueSpecification;
    typedef UmlPtr<ValueSpecification> ValueSpecificationPtr;

    class InstanceSpecification : public PackageableElement , public DeployedArtifact, public DeploymentTarget {

        friend class UmlManager;
        friend class Classifier;
        friend class InstanceValue;

        protected:
            bool m_setFlag = false;
            Set<Classifier, InstanceSpecification> m_classifiers = Set<Classifier, InstanceSpecification>(this);
            class AddClassifierFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    AddClassifierFunctor(Element* el) : SetFunctor(el) {};
            };
            class RemoveClassifierFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    RemoveClassifierFunctor(Element* el) : SetFunctor(el) {};
            };
            Set<Slot, InstanceSpecification> m_slots = Set<Slot, InstanceSpecification>(this);
            Singleton <ValueSpecification, InstanceSpecification> m_specification = Singleton<ValueSpecification, InstanceSpecification>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(ID id, std::string newName) override;
            void referenceErased(ID id) override;
            void restoreReference(Element* el) override;
            Set<ValueSpecification, InstanceSpecification>& getSpecificationSingleton();
            void init();
            InstanceSpecification();
        public:
            virtual ~InstanceSpecification();
            Set<Classifier, InstanceSpecification>& getClassifiers();
            Set<Slot, InstanceSpecification>& getSlots();
            ValueSpecificationPtr getSpecification() const;
            void setSpecification(ValueSpecification* specification);
            void setSpecification(ValueSpecification& specification);
            void setSpecification(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::INSTANCE_SPECIFICATION;
            };
    };
}

#endif