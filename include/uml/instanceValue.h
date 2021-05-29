#ifndef INSTANCEVALUEH
#define INSTANCEVALUEH

#include "valueSpecification.h"
#include "instanceSpecification.h"

namespace UML {
    class InstanceValue : public ValueSpecification {
        public:
            InstanceSpecification* getInstance();
            void setInstance(InstanceSpecification* inst);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::INSTANCE_VALUE;
            };
        protected:
            InstanceSpecification* instance;
    };
}

#endif