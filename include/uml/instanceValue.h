#ifndef INSTANCEVALUE
#define INSTANCEVALUE

#include "valueSpecification.h"
#include "instanceSpecification.h"

namespace UML {
    class InstanceValue : public ValueSpecification {
        public:
            InstanceSpecification* getInstance();
            void setInstance(InstanceSpecification* inst);
        protected:
            InstanceSpecification* instance;
    };
}

#endif