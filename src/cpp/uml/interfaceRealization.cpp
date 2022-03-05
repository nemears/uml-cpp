#include "uml/interfaceRealization.h"
#include "uml/operation.h"
#include "uml/manifestation.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

void InterfaceRealization::RemoveContractFunctor::operator()(Element& el) const {
    if (m_el.as<InterfaceRealization>().getImplementingClassifier()) {
        for (auto& pair : m_el.as<InterfaceRealization>().m_implementingClassifier.get()->m_node->m_references) {
            if (pair.second && 
                pair.second->m_managerElementMemory->isSubClassOf(ElementType::PORT) && 
                pair.second->m_managerElementMemory->as<Port>().getType().id() == m_el.as<InterfaceRealization>().m_implementingClassifier.get().id()) {
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
    if (m_el.as<InterfaceRealization>().getImplementingClassifier()) {
        for (auto& pair : m_el.as<InterfaceRealization>().m_implementingClassifier.get()->m_node->m_references) {
            if (pair.second && 
                pair.second->m_managerElementMemory->isSubClassOf(ElementType::PORT) && 
                pair.second->m_managerElementMemory->as<Port>().getType().id() == m_el.as<InterfaceRealization>().m_implementingClassifier.get().id()) {
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

InterfaceRealization::InterfaceRealization() : Element(ElementType::INTERFACE_REALIZATION) {
    init();
}

InterfaceRealization::~InterfaceRealization() {
    mountAndRelease();
}

InterfacePtr InterfaceRealization::getContract() const {
    return m_contract.get();
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

BehavioredClassifierPtr InterfaceRealization::getImplementingClassifier() const {
    return m_implementingClassifier.get();
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