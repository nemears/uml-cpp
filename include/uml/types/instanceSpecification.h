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
    struct ElementInfo<InstanceSpecification> : public DefaultInfo {
        static const bool abstract = false;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "InstanceSpecification"; }
        static SetList sets(InstanceSpecification& el) {
            return SetList {
                makeSetPair("classifiers", el.m_classifiers),
                makeSetPair("slots", el.m_slots),
                makeSetPair("specification", el.m_specification)
            };
        }
    };
}
