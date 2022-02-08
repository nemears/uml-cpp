#include "uml/encapsulatedClassifier.h"
#include "uml/uml-stable.h"

using namespace UML;

void EncapsulatedClassifier::AddOwnedAttributeFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::PORT)) {
        m_el.as<EncapsulatedClassifier>().m_ownedPorts.nonOppositeAdd(el.as<Port>());
    }
}

void EncapsulatedClassifier::RemoveOwnedAttributeFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::PORT)) {
        m_el.as<EncapsulatedClassifier>().m_ownedPorts.nonOppositeRemove(el.getID());
    }
}

void EncapsulatedClassifier::init() {
    m_ownedPorts.subsets(m_ownedAttributes);
    m_ownedPorts.m_readOnly = true;
    m_ownedPorts.m_signature = &EncapsulatedClassifier::getOwnedPorts;
    m_ownedAttributes.m_addFunctors.insert(new AddOwnedAttributeFunctor(this));
    m_ownedAttributes.m_removeFunctors.insert(new RemoveOwnedAttributeFunctor(this));
}

void EncapsulatedClassifier::copy(const EncapsulatedClassifier& rhs) {
    m_ownedPorts = rhs.m_ownedPorts;
}

EncapsulatedClassifier::EncapsulatedClassifier() : Element(ElementType::ENCAPSULATED_CLASSIFIER) {
    init();
}

EncapsulatedClassifier::~EncapsulatedClassifier() {

}

EncapsulatedClassifier::EncapsulatedClassifier(const EncapsulatedClassifier& rhs) : Element(rhs, ElementType::ENCAPSULATED_CLASSIFIER) {
    // abstract
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

