#include "uml/uml-stable.h"

using namespace UML;

void EncapsulatedClassifier::OwnedAttributePolicy::elementAdded(Property& el, EncapsulatedClassifier& me) {
    if (el.is(ElementType::PORT) && !me.m_ownedPorts.contains(el.getID())) {
        me.m_ownedPorts.innerAdd(&el);
    }
}

void EncapsulatedClassifier::OwnedAttributePolicy::elementRemoved(Property& el, EncapsulatedClassifier& me) {
    if (el.is(ElementType::PORT) && me.m_ownedPorts.contains(el.getID())) {
        me.m_ownedPorts.innerRemove(&el);
    }
}

EncapsulatedClassifier::EncapsulatedClassifier() : Element(ElementType::ENCAPSULATED_CLASSIFIER) {
    m_ownedPorts.subsets(m_ownedAttributes);
    m_encapsulatedClassifierOwnedAttributes.redefines(m_ownedAttributes);
}

EncapsulatedClassifier::~EncapsulatedClassifier() {

}

ReadOnlySet<Port, EncapsulatedClassifier>& EncapsulatedClassifier::getOwnedPorts() {
    return m_ownedPorts;
}

bool EncapsulatedClassifier::is(ElementType eType) const {
    bool ret = StructuredClassifier::is(eType);

    if (!ret) {
        ret = eType == ElementType::ENCAPSULATED_CLASSIFIER;
    }

    return ret;
}

