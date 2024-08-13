#pragma once

#include "valueSpecification.h"

namespace UML {

    class InstanceSpecification;
    typedef UmlPtr<InstanceSpecification> InstanceSpecificationPtr;

    class InstanceValue : public ValueSpecification {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            Singleton<InstanceSpecification, InstanceValue> m_instance = Singleton<InstanceSpecification, InstanceValue>(this);
            void referenceErased(ID id) override;
            Singleton<InstanceSpecification, InstanceValue>& getInstanceSingleton();
            InstanceValue();
        public:
            virtual ~InstanceValue();
            InstanceSpecificationPtr getInstance() const;
            void setInstance(InstanceSpecification& inst);
            void setInstance(InstanceSpecificationPtr inst);
            void setInstance(ID id);
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::INSTANCE_VALUE;
            };
    };
}
