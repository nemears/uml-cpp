#pragma once

#include "packageableElement.h"
#include "deployedArtifact.h"
#include "deploymentTarget.h"
#include "uml/set/indexableSet.h"
#include "uml/types/element.h"

namespace UML{

    class Slot;
    class Classifier;
    class InstanceValue;
    class ValueSpecification;
    typedef UmlPtr<ValueSpecification> ValueSpecificationPtr;

    class InstanceSpecification : public PackageableElement , public DeployedArtifact, public DeploymentTarget {

        friend class Classifier;
        friend class InstanceValue;
        friend struct ElementInfo<InstanceSpecification>;

        protected:
            bool m_setFlag = false;
            IndexableSet<Classifier, InstanceSpecification> m_classifiers = IndexableSet<Classifier, InstanceSpecification>(this);
            Set<Slot, InstanceSpecification> m_slots = Set<Slot, InstanceSpecification>(this);
            Singleton <ValueSpecification, InstanceSpecification> m_specification = Singleton<ValueSpecification, InstanceSpecification>(this);
            Singleton<ValueSpecification, InstanceSpecification>& getSpecificationSingleton();
        public:
            InstanceSpecification(std::size_t elementType, AbstractManager& manager);
            IndexableSet<Classifier, InstanceSpecification>& getClassifiers();
            Set<Slot, InstanceSpecification>& getSlots();
            ValueSpecificationPtr getSpecification() const;
            void setSpecification(ValueSpecificationPtr specification);
            void setSpecification(ValueSpecification& specification);
            void setSpecification(ID id);
            typedef TypeInfo<std::tuple<PackageableElement, DeployedArtifact, DeploymentTarget>, InstanceSpecification> Info;
    };

    template <>
    struct ElementInfo<InstanceSpecification> {
        static const bool abstract = false;
        inline static const std::string name {"InstanceSpecification"};
        static SetList sets(InstanceSpecification& el) {
            return SetList {
                makeSetPair("", el.m_classifiers),
                makeSetPair("", el.m_slots),
                makeSetPair("", el.m_specification)
            };
        }
    };
}
