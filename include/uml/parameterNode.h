#ifndef PARAMETERNODE_H
#define PARAMETERNODE_H

#include "objectNode.h"
#include "parameter.h"

namespace UML {
    class ParameterNode : public ObjectNode {
        protected:
            Parameter* parameter;
        public:
            Parameter* getParameter();
            void setParameter(Parameter* parameter);
            ElementType getElementType() override;
            ParameterNode() {
                parameter = NULL;
            }
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif