#include "uml/opaqueBehavior.h"
#include "uml/literalString.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"

namespace UML {

Set<LiteralString, OpaqueBehavior>& OpaqueBehavior::getBodiesSet() {
    return m_bodies;
}

void OpaqueBehavior::init() {
    m_bodies.subsets(*m_ownedElements);
}

OpaqueBehavior::OpaqueBehavior() : Element(ElementType::OPAQUE_BEHAVIOR) {
    init();
}

OpaqueBehavior::~OpaqueBehavior() {
    mountAndRelease();
}

OrderedSet<LiteralString, OpaqueBehavior>& OpaqueBehavior::getBodies() {
    return m_bodies;
}

bool OpaqueBehavior::isSubClassOf(ElementType eType) const {
    bool ret = Behavior::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::OPAQUE_BEHAVIOR;
    }

    return ret;
}
}