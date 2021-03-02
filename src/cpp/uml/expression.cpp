#include "uml/expression.h"

using namespace UML;

string Expression::getSymbol() {
    return symbol;
}

void Expression::setSymbol(string sym) {
    this->symbol = sym;
}

ElementType Expression::getElementType() {
    return ElementType::EXPRESSION;
}