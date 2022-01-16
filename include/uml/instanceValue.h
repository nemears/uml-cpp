#ifndef INSTANCEVALUEH
#define INSTANCEVALUEH

#include "valueSpecification.h"

namespace UML {

    class InstanceSpecification;

    class InstanceValue : public ValueSpecification {

        friend class UmlManager;

        protected:
            Singleton<InstanceSpecification, InstanceValue> m_instance = Singleton<InstanceSpecification, InstanceValue>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(std::string oldName, std::string newName) override;
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