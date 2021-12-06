#ifndef INSTANCEVALUEH
#define INSTANCEVALUEH

#include "valueSpecification.h"

namespace UML {

    class InstanceSpecification;
    namespace Parsers {
        class SetInstance;
    }

    class InstanceValue : public ValueSpecification {

        friend class UmlManager;
        friend class Parsers::SetInstance;

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
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::INSTANCE_VALUE;
            };
    };
}

#endif