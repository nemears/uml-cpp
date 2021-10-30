#include "uml/parameterNode.h"

using namespace UML;

ParameterNode::ParameterNode() : Element(ElementType::PARAMETER_NODE) {
    parameter = 0;
}

ParameterNode::ParameterNode(const ParameterNode& rhs) : Element(rhs, ElementType::PARAMETER_NODE) {}

Parameter* ParameterNode::getParameter() {
    return parameter;
}

void ParameterNode::setParameter(Parameter* parameter) {
    this->parameter = parameter;
}

bool ParameterNode::isSubClassOf(ElementType eType) const {
    bool ret = ObjectNode::isSubClassOf(eType);

    if(!ret) {
        ret = eType == ElementType::PARAMETER_NODE;
    }

    return ret;
}