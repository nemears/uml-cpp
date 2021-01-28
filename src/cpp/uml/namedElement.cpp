#include "uml/namedElement.h"

void UML::NamedElement::setName(const string &name) {
    this->name = name;
}

string UML::NamedElement::getName() {
    return name;
}