#include "uml/uml-stable.h"

using namespace UML;

void InterfaceRealization::ContractPolicy::elementRemoved(Interface& el, InterfaceRealization& me) {
    if (me.getImplementingClassifier()) {
        for (auto& referencePair : me.getNode(*me.m_implementingClassifier.get())->m_references) {
            auto referencedEl = referencePair.second.m_node.lock()->m_ptr;
            if (referencedEl->is<Port>() && 
                referencedEl->as<Port>().getType().id() == me.m_implementingClassifier.get().id()) {
                    Port& port = referencedEl->as<Port>();
                    if (port.isConjugated()) {
                        if (port.getRequired().contains(el.getID())) {
                            me.removeFromReadonlySet(port.m_required, el);
                        }
                    } else {
                        if (port.getProvided().contains(el.getID())) {
                            me.removeFromReadonlySet(port.m_provided, el);
                        }
                    }
            }
        }
    }
}

void InterfaceRealization::ContractPolicy::elementAdded(Interface& el, InterfaceRealization& me) {
    if (me.getImplementingClassifier()) {
        for (auto& reference : me.getNode(*me.m_implementingClassifier.get())->m_references) {
            auto referencedEl = reference.second.m_node.lock()->m_ptr;
            if (
                referencedEl->is<Port>() && 
                referencedEl->as<Port>().getType().id() == me.m_implementingClassifier.get().id()
            ) {
                Port& port = referencedEl->as<Port>();
                if (port.isConjugated()) {
                    if (!port.getRequired().contains(el.getID())) {
                        me.addToReadonlySet(port.m_required, el);
                    }
                } else {
                    if (!port.getProvided().contains(el.getID())) {
                        me.addToReadonlySet(port.m_provided, el);
                    }
                }
            }
        }
    }
}

Singleton<Interface, InterfaceRealization, InterfaceRealization::ContractPolicy>& InterfaceRealization::getContractSingleton() {
    return m_contract;
}

Singleton<BehavioredClassifier, InterfaceRealization>& InterfaceRealization::getImplementingClassifierSingleton() {
    return m_implementingClassifier;
}

InterfaceRealization::InterfaceRealization(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    Realization(elementType, manager) 
{
    m_contract.subsets(m_suppliers);
    m_implementingClassifier.subsets(m_clients);
    m_implementingClassifier.subsets(m_owner);
    m_implementingClassifier.opposite(&BehavioredClassifier::getInterfaceRealizations);
}

InterfacePtr InterfaceRealization::getContract() const {
    return m_contract.get();
}

void InterfaceRealization::setContract(InterfacePtr contract) {
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

void InterfaceRealization::setImplementingClassifier(BehavioredClassifierPtr implementingClassifier) {
    m_implementingClassifier.set(implementingClassifier);
}

void InterfaceRealization::setImplementingClassifier(BehavioredClassifier& implementingClassifier) {
    m_implementingClassifier.set(implementingClassifier);
}

void InterfaceRealization::setImplementingClassifier(ID id) {
    m_implementingClassifier.set(id);
}
