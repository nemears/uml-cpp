#include "uml/opaqueBehavior.h"
#include "uml/literalString.h"

namespace UML {

OpaqueBehavior::OpaqueBehavior() {
}

OpaqueBehavior::~OpaqueBehavior() {
    
}

Sequence<LiteralString>& OpaqueBehavior::getBodies() {
    return m_bodies;
}

ElementType OpaqueBehavior::getElementType() const {
    return ElementType::OPAQUE_BEHAVIOR;
}

bool OpaqueBehavior::isSubClassOf(ElementType eType) const {
    bool ret = Behavior::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::OPAQUE_BEHAVIOR;
    }

    return ret;
}
}