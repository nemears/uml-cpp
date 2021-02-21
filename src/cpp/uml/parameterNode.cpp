#include "uml/parameterNode.h"

using namespace UML;

Parameter* ParameterNode::getParameter() {
    return parameter;
}

void ParameterNode::setParameter(Parameter* parameter) {
    this->parameter = parameter;
}

ElementType ParameterNode::getElementType() {
    return ElementType::PARAMETER_NODE;
}