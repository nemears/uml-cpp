#ifndef INSTANCESPECIFICATIONH
#define INSTANCESPECIFICATIONH

#include "packageableElement.h"
#include "deployedArtifact.h"
#include "deploymentTarget.h"
#include "yaml-cpp/yaml.h"

namespace UML{

    class Slot;
    class Classifier;
    class InstanceValue;
    namespace Parsers {
        void parseInstanceSpecification(YAML::Node node, InstanceSpecification& inst, Parsers::ParserMetaData& data);
    }

    class InstanceSpecification : public PackageableElement , public DeployedArtifact, public DeploymentTarget {

        friend class UmlManager;
        friend class Classifier;
        friend class InstanceValue;
        friend void Parsers::parseInstanceSpecification(YAML::Node node, InstanceSpecification& inst, Parsers::ParserMetaData& data);

        protected:
            bool m_setFlag = false;
            Set<Classifier, InstanceSpecification> m_classifiers = Set<Classifier, InstanceSpecification>(this);
            Set<Slot, InstanceSpecification> m_slots = Set<Slot, InstanceSpecification>(this);
            Singleton <ValueSpecification, InstanceSpecification> m_specification = Singleton<ValueSpecification, InstanceSpecification>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Set<ValueSpecification, InstanceSpecification>& getSpecificationSingleton();
            void init();
            void copy(const InstanceSpecification& rhs);
            InstanceSpecification();
        public:
            InstanceSpecification(const InstanceSpecification& rhs);
            virtual ~InstanceSpecification();
            Set<Classifier, InstanceSpecification>& getClassifiers();
            Set<Slot, InstanceSpecification>& getSlots();
            ValueSpecification* getSpecification();
            ValueSpecification& getSpecificationRef();
            ID getSpecificationID() const;
            bool hasSpecification() const;
            void setSpecification(ValueSpecification* specification);
            void setSpecification(ValueSpecification& specification);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::INSTANCE_SPECIFICATION;
            };
    };
}

#endif