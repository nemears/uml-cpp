#include "uml/namedElement.h"
#include "uml/element.h"

using namespace UML;

void NamedElement::setName(const string &name) {
    this->name = name;
}

string NamedElement::getName() {
    return name;
}

ElementType NamedElement::getElementType() {
    return ElementType::NAMED_ELEMENT;
}