#include "uml/encapsulatedClassifier.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/package.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"

using namespace UML;

// void EncapsulatedClassifier::AddOwnedAttributeFunctor::operator()(Element& el) const {
//     if (el.isSubClassOf(ElementType::PORT)) {
//         m_el.as<EncapsulatedClassifier>().m_ownedPorts.nonOppositeAdd(el.as<Port>());
//     }
// }

// void EncapsulatedClassifier::RemoveOwnedAttributeFunctor::operator()(Element& el) const {
//     if (el.isSubClassOf(ElementType::PORT)) {
//         m_el.as<EncapsulatedClassifier>().m_ownedPorts.nonOppositeRemove(el.getID());
//     }
// }

EncapsulatedClassifier::EncapsulatedClassifier() : Element(ElementType::ENCAPSULATED_CLASSIFIER) {
    m_ownedPorts.subsets(m_ownedAttributes);
    m_ownedPorts.m_readOnly = true;
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

