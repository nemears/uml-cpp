#include "uml/signal.h"
#include "uml/uml-stable.h"

using namespace UML;

Set<Property, Signal>& Signal::getOwnedAttributesSet() {
    return m_ownedAttributes;
}

void Signal::init() {
    m_ownedAttributes.subsets(m_attributes);
    m_ownedAttributes.subsets(m_ownedMembers);
    m_ownedAttributes.m_signature = &Signal::getOwnedAttributesSet;
}

void Signal::copy(const Signal& rhs) {
    m_ownedAttributes = rhs.m_ownedAttributes;
}

Signal::Signal() : Element(ElementType::SIGNAL) {
    init();
}

Signal::Signal(const Signal& rhs) : Element(rhs, ElementType::SIGNAL) {
    init();
    Element::copy(rhs);
    NamedElement::copy(rhs);
    Namespace::copy(rhs);
    ParameterableElement::copy(rhs);
    PackageableElement::copy(rhs);
    TemplateableElement::copy(rhs);
    RedefinableElement::copy(rhs);
    Classifier::copy(rhs);
    copy(rhs);
    if (!m_copiedElementFlag) {
        delete &rhs;
    }
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