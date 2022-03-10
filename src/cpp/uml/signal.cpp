#include "uml/signal.h"
#include "uml/interface.h"
#include "uml/operation.h"
#include "uml/manifestation.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"

using namespace UML;

Set<Property, Signal>& Signal::getOwnedAttributesSet() {
    return m_ownedAttributes;
}

void Signal::init() {
    m_ownedAttributes.subsets(m_attributes);
    m_ownedAttributes.subsets(m_ownedMembers);
    m_ownedAttributes.m_signature = &Signal::getOwnedAttributesSet;
}

Signal::Signal() : Element(ElementType::SIGNAL) {
    init();
}

Signal::~Signal() {
    mountAndRelease();
}

OrderedSet<Property, Signal>& Signal::getOwnedAttributes() {
    return m_ownedAttributes;
}

bool Signal::isSubClassOf(ElementType eType) const {
    bool ret = Classifier::isSubClassOf(eType);
    
    if (!ret) {
        ret = eType == ElementType::SIGNAL;
    }

    return ret;
}