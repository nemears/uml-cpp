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
            ElementType getElementType() const override;
            ParameterNode() {
                parameter = NULL;
            }
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PARAMETER_NODE;
            };
    };
}

#endif