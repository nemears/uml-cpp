#include "uml/uml-stable.h"

namespace UML {

OpaqueBehavior::OpaqueBehavior() : Element(ElementType::OPAQUE_BEHAVIOR) {
    m_bodies.subsets(*m_ownedElements);
}

OpaqueBehavior::~OpaqueBehavior() {
    
}

OrderedSet<LiteralString, OpaqueBehavior>& OpaqueBehavior::getBodies() {
    return m_bodies;
}

bool OpaqueBehavior::is(ElementType eType) const {
    bool ret = Behavior::is(eType);

    if (!ret) {
        ret = eType == ElementType::OPAQUE_BEHAVIOR;
    }

    return ret;
}
}
