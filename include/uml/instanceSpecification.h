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

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;
        friend class Classifier;
        friend class InstanceValue;

        protected:
            bool m_setFlag = false;
            CustomSet<Classifier, InstanceSpecification> m_classifiers = CustomSet<Classifier, InstanceSpecification>(this);
            CustomSet<Slot, InstanceSpecification> m_slots = CustomSet<Slot, InstanceSpecification>(this);
            CustomSingleton <ValueSpecification, InstanceSpecification> m_specification = CustomSingleton<ValueSpecification, InstanceSpecification>(this);
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            TypedSet<ValueSpecification, InstanceSpecification>& getSpecificationSingleton();
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