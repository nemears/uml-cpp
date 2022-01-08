#ifndef INSTANCEVALUEH
#define INSTANCEVALUEH

#include "valueSpecification.h"
#include "yaml-cpp/yaml.h"

namespace UML {

    class InstanceSpecification;
    namespace Parsers {
        void parseInstanceValue(YAML::Node node, InstanceValue& val, Parsers::ParserMetaData& data);
    }

    class InstanceValue : public ValueSpecification {

        friend class UmlManager;
        friend void Parsers::parseInstanceValue(YAML::Node node, InstanceValue& val, Parsers::ParserMetaData& data);

        protected:
            Singleton<InstanceSpecification, InstanceValue> m_instance = Singleton<InstanceSpecification, InstanceValue>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Set<InstanceSpecification, InstanceValue>& getInstanceSingleton();
            void init();
            void copy(const InstanceValue& rhs);
            InstanceValue();
        public:
            InstanceValue(const InstanceValue& rhs);
            InstanceSpecification* getInstance();
            InstanceSpecification& getInstanceRef();
            ID getInstanceID();
            bool hasInstance() const;
            void setInstance(InstanceSpecification& inst);
            void setInstance(InstanceSpecification* inst);
            void setInstance(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::INSTANCE_VALUE;
            };
    };
}

#endif