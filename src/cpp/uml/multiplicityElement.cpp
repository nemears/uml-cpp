#include "uml/multiplicityElement.h"

using namespace UML;

int MultiplicityElement::getLower() {
    if (lowSpecified) {
        return lower;
    } else {
        throw multiplicityNotSpecifiedException;
    }
}

void MultiplicityElement::setLower(const int low) {
    lower = low;
    if (!lowSpecified) {
        lowSpecified = true;
    }
    if (upSpecified) {
        multiplicityIsSpecified = true;
    }
}

int MultiplicityElement::getUpper() {
    if (upSpecified) {
        return upper;
    } else {
        throw multiplicityNotSpecifiedException;
    }
}

void MultiplicityElement::setUpper(const int up) {
    upper = up;
    if (!upSpecified) {
        upSpecified = true;
    }
    if (lowSpecified) {
        multiplicityIsSpecified = true;
    }
}

bool MultiplicityElement::multiplicitySpecified() {
    return multiplicityIsSpecified;
}