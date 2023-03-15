#include "uml/types/opaqueBehavior.h"
#include "uml/types/literalString.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"

namespace UML {

OpaqueBehavior::OpaqueBehavior() : Element(ElementType::OPAQUE_BEHAVIOR) {
    m_bodies.subsets(*m_ownedElements);
}

OpaqueBehavior::~OpaqueBehavior() {
    
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