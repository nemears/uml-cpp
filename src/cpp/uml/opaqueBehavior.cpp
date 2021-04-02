#include "uml/opaqueBehavior.h"

using namespace UML;

ElementType OpaqueBehavior::getElementType() {
    return ElementType::OPAQUE_BEHAVIOR;
}

string OpaqueBehavior::getSingletonBody() {
    return singletonBody;
}

void OpaqueBehavior::setSingletonBody(const string& sb) {
    bodies.push_front(sb);
    if (bodies.size() == 2) {
        bodies.pop_back();
    }
    singletonBody = sb;
}

bool OpaqueBehavior::isSubClassOf(ElementType eType) {
    bool ret = Behavior::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::OPAQUE_BEHAVIOR;
    }

    return ret;
}