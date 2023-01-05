#ifndef _UML_INSTANCE_VALUE_H_
#define _UML_INSTANCE_VALUE_H_

#include "valueSpecification.h"

namespace UML {

    class InstanceSpecification;
    typedef UmlPtr<InstanceSpecification> InstanceSpecificationPtr;

    class InstanceValue : public ValueSpecification {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            CustomSingleton<InstanceSpecification, InstanceValue> m_instance = CustomSingleton<InstanceSpecification, InstanceValue>(this);
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            void restoreReference(Element* el) override;
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

#endif