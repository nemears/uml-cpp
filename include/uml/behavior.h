#ifndef BEHAVIORH
#define BEHAVIORH
#include "class.h"
#include "parameter.h"

namespace UML {

    class Operation;

    class Behavior : public Class {
        protected:
            Sequence<Parameter>* m_parameters;
            Operation* m_specification;
        public:
            Behavior();
            ~Behavior();
            Sequence<Parameter>& getParameters();
            Operation* getSpecification();
            void setSpecification(Operation* specification);
            ElementType getElementType() override;
    };
}

#endif