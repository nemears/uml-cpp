#include "uml/class.h"
#include "uml/operation.h"

using namespace UML;

ElementType Class::getElementType() {
    return ElementType::CLASS;
}

void Class::addOperation(Operation& o) {
    operations.push_back(&o);
}

void Class::removeOperation(Operation& o) {
    list<Operation*>::iterator i = operations.begin();
    while (i != operations.end()) {
        if ((*i)->uuid == o.uuid) {
            operations.erase(i);
            break;
        }
        ++i;
    }
}