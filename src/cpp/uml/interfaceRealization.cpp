#include "uml/interfaceRealization.h"
#include "uml/uml-stable.h"

using namespace UML;

Set<Interface, InterfaceRealization>& InterfaceRealization::getContractSingleton() {
    return m_contract;
}

Set<BehavioredClassifier, InterfaceRealization>& InterfaceRealization::getImplementingClassifierSingleton() {
    return m_implementingClassifier;
}

void InterfaceRealization::init() {
    m_contract.subsets(m_supplier);
    m_contract.m_signature = &InterfaceRealization::getContractSingleton;
    m_implementingClassifier.subsets(m_client);
    m_implementingClassifier.subsets(*m_owner);
    m_implementingClassifier.opposite(&BehavioredClassifier::getInterfaceRealizations);
    m_implementingClassifier.m_signature = &InterfaceRealization::getImplementingClassifierSingleton;
}

void InterfaceRealization::copy(const InterfaceRealization& rhs) {
    m_contract = rhs.m_contract;
    m_implementingClassifier = rhs.m_implementingClassifier;
}

InterfaceRealization::InterfaceRealization() : Element(ElementType::INTERFACE_REALIZATION) {
    init();
}

InterfaceRealization::~InterfaceRealization() {
    
}

InterfaceRealization::InterfaceRealization(const InterfaceRealization& rhs) : Element(rhs, ElementType::INTERFACE_REALIZATION) {
    init();
    Element::copy(rhs);
    NamedElement::copy(rhs);
    ParameterableElement::copy(rhs);
    PackageableElement::copy(rhs);
    Relationship::copy(rhs);
    DirectedRelationship::copy(rhs);
    Dependency::copy(rhs);
    copy(rhs);
}

Interface* InterfaceRealization::getContract() {
    return m_contract.get();
}

Interface& InterfaceRealization::getContractRef() {
    return m_contract.getRef();
}

bool InterfaceRealization::hasContract() const {
    return m_contract.has();
}

ID InterfaceRealization::getContractID() const {
    return m_contract.id();
}

void InterfaceRealization::setContract(Interface* contract) {
    m_contract.set(contract);
}

void InterfaceRealization::setContract(Interface& contract) {
    m_contract.set(contract);
}

void InterfaceRealization::setContract(ID id) {
    m_contract.set(id);
}

BehavioredClassifier* InterfaceRealization::getImplementingClassifier() {
    return m_implementingClassifier.get();
}

BehavioredClassifier& InterfaceRealization::getImplementingClassifierRef() {
    return m_implementingClassifier.getRef();
}

bool InterfaceRealization::hasImplementingClassifier() const {
    return m_implementingClassifier.has();
}

ID InterfaceRealization::getImplementingClassifierID() const {
    return m_implementingClassifier.id();
}

void InterfaceRealization::setImplementingClassifier(BehavioredClassifier* implementingClassifier) {
    m_implementingClassifier.set(implementingClassifier);
}

void InterfaceRealization::setImplementingClassifier(BehavioredClassifier& implementingClassifier) {
    m_implementingClassifier.set(implementingClassifier);
}

void InterfaceRealization::setImplementingClassifier(ID id) {
    m_implementingClassifier.set(id);
}

bool InterfaceRealization::isSubClassOf(ElementType eType) const {
    bool ret = Realization::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::INTERFACE_REALIZATION;
    }

    return ret;
}