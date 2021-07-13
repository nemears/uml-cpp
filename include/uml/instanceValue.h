#ifndef INSTANCEVALUEH
#define INSTANCEVALUEH

#include "valueSpecification.h"

namespace UML {

    class InstanceSpecification;

    class InstanceValue : public ValueSpecification {
        protected:
            ID m_instanceID;
            InstanceSpecification* m_instancePtr;
        public:
            InstanceSpecification* getInstance();
            void setInstance(InstanceSpecification* inst);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::INSTANCE_VALUE;
            };
    };
}

#endif