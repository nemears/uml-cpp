#include "uml/opaqueBehavior.h"
#include "uml/literalString.h"
#include "uml/uml-stable.h"

namespace UML {

Set<LiteralString, OpaqueBehavior>& OpaqueBehavior::getBodiesSet() {
    return m_bodies;
}

void OpaqueBehavior::init() {
    m_bodies.subsets(*m_ownedElements);
    m_bodies.m_signature = &OpaqueBehavior::getBodiesSet;
}

void OpaqueBehavior::copy(const OpaqueBehavior& rhs) {
    m_bodies = rhs.m_bodies;
}

OpaqueBehavior::OpaqueBehavior() : Element(ElementType::OPAQUE_BEHAVIOR) {
    init();
}

OpaqueBehavior::OpaqueBehavior(const OpaqueBehavior& rhs) : Element(rhs, ElementType::OPAQUE_BEHAVIOR) {
    init();
    Element::copy(rhs);
    NamedElement::copy(rhs);
    Namespace::copy(rhs);
    PackageableElement::copy(rhs);
    Classifier::copy(rhs);
    StructuredClassifier::copy(rhs);
    Class::copy(rhs);
    Behavior::copy(rhs);
    copy(rhs);
    if (!m_copiedElementFlag) {
        delete &rhs;
    }
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