#include "uml/opaqueBehavior.h"

namespace UML {

OpaqueBehavior::OpaqueBehavior() {
    m_bodies = new Sequence<LiteralString>();
}

OpaqueBehavior::~OpaqueBehavior() {
    delete m_bodies;
}

Sequence<LiteralString>& OpaqueBehavior::getBodies() {
    return *m_bodies;
}

ElementType OpaqueBehavior::getElementType() const {
    return ElementType::OPAQUE_BEHAVIOR;
}

bool OpaqueBehavior::isSubClassOf(ElementType eType) {
    bool ret = Behavior::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::OPAQUE_BEHAVIOR;
    }

    return ret;
}
}