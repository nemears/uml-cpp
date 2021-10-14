#include "uml/opaqueBehavior.h"
#include "uml/literalString.h"

namespace UML {

void OpaqueBehavior::AddBodyFunctor::operator()(LiteralString& el) const {
    if (el.hasOwner()) {
        if (*el.getOwner() != *m_el) {
            el.setOwner(m_el);
        }
    } else {
        el.setOwner(m_el);
    }
    updateCopiedSequenceAddedTo(el, &OpaqueBehavior::getBodies);
}

void OpaqueBehavior::RemoveBodyFunctor::operator()(LiteralString& el) const {
    if (el.hasOwner()) {
        if (*el.getOwner() == *m_el) {
            el.setOwner(0);
        }
    }
    updateCopiedSequenceRemovedFrom(el, &OpaqueBehavior::getBodies);
}

void OpaqueBehavior::referencingReleased(ID id) {
    Behavior::referencingReleased(id);
    m_bodies.elementReleased(id, &OpaqueBehavior::getBodies);
}

void OpaqueBehavior::referenceReindexed(ID oldID, ID newID) {
    Behavior::referenceReindexed(oldID, newID);
    m_bodies.reindex(oldID, newID, &OpaqueBehavior::getBodies);
}

void OpaqueBehavior::referenceErased(ID id) {
    Behavior::referenceErased(id);
    m_bodies.elementErased(id);
}

OpaqueBehavior::OpaqueBehavior() {
    m_bodies.addProcedures.push_back(new AddBodyFunctor(this));
    m_bodies.removeProcedures.push_back(new RemoveBodyFunctor(this));
}

OpaqueBehavior::OpaqueBehavior(const OpaqueBehavior& rhs) {
    m_bodies = rhs.m_bodies;
    m_bodies.m_el = this;
    m_bodies.addProcedures.clear();
    m_bodies.removeProcedures.clear();
    m_bodies.addProcedures.push_back(new AddBodyFunctor(this));
    m_bodies.removeProcedures.push_back(new RemoveBodyFunctor(this));
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