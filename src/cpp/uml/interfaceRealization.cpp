#include "uml/interfaceRealization.h"
#include "uml/uml-stable.h"

using namespace UML;

void InterfaceRealization::RemoveContractFunctor::operator()(Element& el) const {
    if (m_el.as<InterfaceRealization>().hasImplementingClassifier()) {
        for (auto& pair : m_el.as<InterfaceRealization>().m_implementingClassifier.getRef().m_node->m_references) {
            if (pair.second && 
                pair.second->m_managerElementMemory->isSubClassOf(ElementType::PORT) && 
                pair.second->m_managerElementMemory->as<Port>().getTypeID() == m_el.as<InterfaceRealization>().m_implementingClassifier.id()) {
                if (pair.second->m_managerElementMemory->as<Port>().isConjugated()) {
                    if (pair.second->m_managerElementMemory->as<Port>().getRequired().contains(el.getID())) {
                        pair.second->m_managerElementMemory->as<Port>().getRequired().removeReadOnly(el.getID());
                    }
                } else {
                    if (pair.second->m_managerElementMemory->as<Port>().getProvided().contains(el.getID())) {
                        pair.second->m_managerElementMemory->as<Port>().getProvided().removeReadOnly(el.getID());
                    }
                }
            }
        }
    }
}

void InterfaceRealization::SetContractFunctor::operator()(Element& el) const {
    if (m_el.as<InterfaceRealization>().hasImplementingClassifier()) {
        for (auto& pair : m_el.as<InterfaceRealization>().m_implementingClassifier.getRef().m_node->m_references) {
            if (pair.second && 
                pair.second->m_managerElementMemory->isSubClassOf(ElementType::PORT) && 
                pair.second->m_managerElementMemory->as<Port>().getTypeID() == m_el.as<InterfaceRealization>().m_implementingClassifier.id()) {
                if (pair.second->m_managerElementMemory->as<Port>().isConjugated()) {
                    if (!pair.second->m_managerElementMemory->as<Port>().getRequired().contains(el.getID())) {
                        pair.second->m_managerElementMemory->as<Port>().getRequired().nonOppositeAdd(el.as<Interface>());
                    }
                } else {
                    if (!pair.second->m_managerElementMemory->as<Port>().getProvided().contains(el.getID())) {
                        pair.second->m_managerElementMemory->as<Port>().getProvided().nonOppositeAdd(el.as<Interface>());
                    }
                }
            }
        }
    }
}

Set<Interface, InterfaceRealization>& InterfaceRealization::getContractSingleton() {
    return m_contract;
}

Set<BehavioredClassifier, InterfaceRealization>& InterfaceRealization::getImplementingClassifierSingleton() {
    return m_implementingClassifier;
}

void InterfaceRealization::init() {
    m_contract.subsets(m_supplier);
    m_contract.m_signature = &InterfaceRealization::getContractSingleton;
    m_contract.m_addFunctors.insert(new SetContractFunctor(this));
    m_contract.m_removeFunctors.insert(new RemoveContractFunctor(this));
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
    mountAndRelease();
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
    if (!m_copiedElementFlag) {
        delete &rhs;
    }
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