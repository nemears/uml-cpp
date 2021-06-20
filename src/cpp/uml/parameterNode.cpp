#include "uml/parameterNode.h"

using namespace UML;

Parameter* ParameterNode::getParameter() {
    return parameter;
}

void ParameterNode::setParameter(Parameter* parameter) {
    this->parameter = parameter;
}

ElementType ParameterNode::getElementType() const {
    return ElementType::PARAMETER_NODE;
}

bool ParameterNode::isSubClassOf(ElementType eType) const {
    bool ret = ObjectNode::isSubClassOf(eType);

    if(!ret) {
        ret = eType == ElementType::PARAMETER_NODE;
    }

    return ret;
}