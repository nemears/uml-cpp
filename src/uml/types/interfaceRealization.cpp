#include "uml/types/interfaceRealization.h"
#include "uml/set/singleton.h"
#include "uml/uml-stable.h"

using namespace UML;

void InterfaceRealization::ContractPolicy::elementRemoved(Interface& el, InterfaceRealization& me) {
    if (me.getImplementingClassifier()) {
        for (auto& pair : me.m_implementingClassifier.get()->m_node->m_references) {
            if (!pair.second.node || !pair.second.node->m_element) {
                // TODO aquire to not have loss, get rid of continue
                continue;
            }
            if (pair.second.node->m_element->is(ElementType::PORT) && 
                pair.second.node->m_element->as<Port>().getType().id() == me.m_implementingClassifier.get().id()) {
                    Port& port = pair.second.node->m_element->as<Port>();
                    if (port.isConjugated()) {
                        if (port.getRequired().contains(el.getID())) {
                            port.m_required.innerRemove(&el);
                        }
                    } else {
                        if (port.getProvided().contains(el.getID())) {
                            port.m_provided.innerRemove(&el);
                        }
                    }
            }
        }
    }
}

void InterfaceRealization::ContractPolicy::elementAdded(Interface& el, InterfaceRealization& me) {
    if (me.getImplementingClassifier()) {
        for (auto& pair : me.m_implementingClassifier.get()->m_node->m_references) {
            if (!pair.second.node || !pair.second.node->m_element) {
                // TODO aquire to not have loss, get rid of continue
                continue;
            }
            if (pair.second.node->m_element->is(ElementType::PORT) && 
                pair.second.node->m_element->as<Port>().getType().id() == me.m_implementingClassifier.get().id()) {
                    Port& port = pair.second.node->m_element->as<Port>();
                    if (port.isConjugated()) {
                        if (!port.getRequired().contains(el.getID())) {
                            port.m_required.innerAdd(&el);
                        }
                    } else {
                        if (!port.getProvided().contains(el.getID())) {
                             port.m_provided.innerAdd(&el);
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

InterfaceRealization::InterfaceRealization() : Element(ElementType::INTERFACE_REALIZATION) {
    m_contract.subsets(m_suppliers);
    m_implementingClassifier.subsets(m_clients);
    m_implementingClassifier.subsets(*m_owner);
    m_implementingClassifier.opposite(&BehavioredClassifier::getInterfaceRealizations);
}

InterfaceRealization::~InterfaceRealization() {
    
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

bool InterfaceRealization::is(ElementType eType) const {
    bool ret = Realization::is(eType);

    if (!ret) {
        ret = eType == ElementType::INTERFACE_REALIZATION;
    }

    return ret;
}
