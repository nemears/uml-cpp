#ifndef INSTANCEVALUEH
#define INSTANCEVALUEH

#include "valueSpecification.h"
#include "instanceSpecification.h"

namespace UML {
    class InstanceValue : public ValueSpecification {
        public:
            InstanceSpecification* getInstance();
            void setInstance(InstanceSpecification* inst);
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
        protected:
            InstanceSpecification* instance;
    };
}

#endif