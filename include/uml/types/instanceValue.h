#pragma once

#include "valueSpecification.h"

namespace UML {

    class InstanceSpecification;
    typedef UmlPtr<InstanceSpecification> InstanceSpecificationPtr;

    class InstanceValue : public ValueSpecification {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            CustomSingleton<InstanceSpecification, InstanceValue> m_instance = CustomSingleton<InstanceSpecification, InstanceValue>(this);
            void referenceErased(ID id) override;
            TypedSet<InstanceSpecification, InstanceValue>& getInstanceSingleton();
            InstanceValue();
        public:
            virtual ~InstanceValue();
            InstanceSpecificationPtr getInstance() const;
            void setInstance(InstanceSpecification& inst);
            void setInstance(InstanceSpecification* inst);
            void setInstance(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::INSTANCE_VALUE;
            };
    };
}