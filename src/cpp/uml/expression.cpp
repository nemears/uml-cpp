#include "uml/expression.h"

using namespace UML;

string Expression::getSymbol() {
    return symbol;
}

void Expression::setSymbol(string sym) {
    this->symbol = sym;
}

ElementType getElementType() {
    return ElementType::EXPRESSION;
}