#pragma once

#include "packageableElement.h"
#include "deployedArtifact.h"
#include "deploymentTarget.h"
#include "uml/set/indexableSet.h"

namespace UML{

    class Slot;
    class Classifier;
    class InstanceValue;
    class ValueSpecification;
    typedef UmlPtr<ValueSpecification> ValueSpecificationPtr;

    class InstanceSpecification : public PackageableElement , public DeployedArtifact, public DeploymentTarget {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Classifier;
        friend class InstanceValue;

        protected:
            bool m_setFlag = false;
            IndexableSet<Classifier, InstanceSpecification> m_classifiers = IndexableSet<Classifier, InstanceSpecification>(this);
            Set<Slot, InstanceSpecification> m_slots = Set<Slot, InstanceSpecification>(this);
            Singleton <ValueSpecification, InstanceSpecification> m_specification = Singleton<ValueSpecification, InstanceSpecification>(this);
            void referenceErased(ID id) override;
            Singleton<ValueSpecification, InstanceSpecification>& getSpecificationSingleton();
            InstanceSpecification();
        public:
            virtual ~InstanceSpecification();
            IndexableSet<Classifier, InstanceSpecification>& getClassifiers();
            Set<Slot, InstanceSpecification>& getSlots();
            ValueSpecificationPtr getSpecification() const;
            void setSpecification(ValueSpecificationPtr specification);
            void setSpecification(ValueSpecification& specification);
            void setSpecification(ID id);
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::INSTANCE_SPECIFICATION;
            };
    };
}
