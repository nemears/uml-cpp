#include "uml/operation.h"

using namespace UML;

Type* Operation::getType() {
    return type;
}

void Operation::setType(Type* type) {
    this->type = type;
}

ElementType Operation::getElementType(){
    return ElementType::OPERATION;
}

void Operation::addMethod(Behavior& behavior) {
    methods.push_back(&behavior);
}

void Operation::addParameter(Parameter& parameter) {
    parameters.push_back(&parameter);
}

void Operation::removeMethod(Behavior& behavior) {
    list<Behavior*>::iterator i = methods.begin();
    while (i != methods.end()) {
        if ((*i)->uuid == behavior.uuid) {
            methods.erase(i);
            break;
        }
        ++i;
    }
}

void Operation::removeParameter(Parameter& parameter) {
    list<Parameter*>::iterator i = parameters.begin();
    while (i != parameters.end()) {
        if ((*i)->uuid == parameter.uuid) {
            parameters.erase(i);
            break;
        }
        ++i;
    }
}