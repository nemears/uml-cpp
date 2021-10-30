#ifndef PARAMETERNODE_H
#define PARAMETERNODE_H

#include "objectNode.h"
#include "parameter.h"

namespace UML {
    class ParameterNode : public ObjectNode {

        friend class UmlManager;

        protected:
            Parameter* parameter;
            ParameterNode();
        public:
            ParameterNode(const ParameterNode& rhs);
            Parameter* getParameter();
            void setParameter(Parameter* parameter);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PARAMETER_NODE;
            };
    };
}

#endif