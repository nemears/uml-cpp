#include "uml/parameterableElement.h"

using namespace UML;

ParameterableElement::ParameterableElement() {

}

ParameterableElement::ParameterableElement(const ParameterableElement& el) : Element(el) {

}

ParameterableElement::~ParameterableElement() {

}

ElementType ParameterableElement::getElementType() const {
    return ElementType::PARAMETERABLE_ELEMENT;
}

bool ParameterableElement::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PARAMETERABLE_ELEMENT;
    }

    return ret;
}