#include "uml/types/encapsulatedClassifier.h"
#include "uml/types/property.h"
#include "uml/types/generalization.h"
#include "uml/types/package.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"

using namespace UML;

void EncapsulatedClassifier::AddOwnedAttributePolicy::apply(Property& el, EncapsulatedClassifier& me) {
    if (el.isSubClassOf(ElementType::PORT) && !me.m_ownedPorts.contains(el.getID())) {
        me.m_ownedPorts.addReadOnly(el.as<Port>());
    }
}

void EncapsulatedClassifier::RemoveOwnedAttributePolicy::apply(Property& el, EncapsulatedClassifier& me) {
    if (el.isSubClassOf(ElementType::PORT) && me.m_ownedPorts.contains(el.getID())) {
        me.m_ownedPorts.removeReadOnly(el.getID());
    }
}

EncapsulatedClassifier::EncapsulatedClassifier() : Element(ElementType::ENCAPSULATED_CLASSIFIER) {
    m_ownedPorts.subsets(m_ownedAttributes);
    m_ownedPorts.m_readOnly = true;
    m_encapsulatedClassifierOwnedAttributes.redefines(m_ownedAttributes);
}

EncapsulatedClassifier::~EncapsulatedClassifier() {

}

Set<Port, EncapsulatedClassifier>& EncapsulatedClassifier::getOwnedPorts() {
    return m_ownedPorts;
}

bool EncapsulatedClassifier::isSubClassOf(ElementType eType) const {
    bool ret = StructuredClassifier::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ENCAPSULATED_CLASSIFIER;
    }

    return ret;
}

