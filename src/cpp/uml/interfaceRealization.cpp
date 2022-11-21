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

void InterfaceRealization::RemoveContractPolicy::apply(Interface& el, InterfaceRealization& me) {
    if (me.getImplementingClassifier()) {
        for (auto& pair : me.m_implementingClassifier.get()->m_node->m_references) {
            if (!pair.second.node || !pair.second.node->m_managerElementMemory) {
                // TODO aquire to not have loss, get rid of continue
                continue;
            }
            if (pair.second.node->m_managerElementMemory->isSubClassOf(ElementType::PORT) && 
                pair.second.node->m_managerElementMemory->as<Port>().getType().id() == me.m_implementingClassifier.get().id()) {
                    Port& port = pair.second.node->m_managerElementMemory->as<Port>();
                    SetLock portLck = me.lockEl(port);
                    if (port.isConjugated()) {
                        if (port.getRequired().contains(el.getID())) {
                            me.m_contract.innerRemoveFromOtherSet(port.getRequired(), el.getID());
                        }
                    } else {
                        if (port.getProvided().contains(el.getID())) {
                            me.m_contract.innerRemoveFromOtherSet(port.getProvided(), el.getID());
                        }
                    }
            }
        }
    }
}

void InterfaceRealization::SetContractPolicy::apply(Interface& el, InterfaceRealization& me) {
    if (me.getImplementingClassifier()) {
        for (auto& pair : me.m_implementingClassifier.get()->m_node->m_references) {
            if (!pair.second.node || !pair.second.node->m_managerElementMemory) {
                // TODO aquire to not have loss, get rid of continue
                continue;
            }
            if (pair.second.node->m_managerElementMemory->isSubClassOf(ElementType::PORT) && 
                pair.second.node->m_managerElementMemory->as<Port>().getType().id() == me.m_implementingClassifier.get().id()) {
                    Port& port = pair.second.node->m_managerElementMemory->as<Port>();
                    SetLock portLck = me.lockEl(port);
                    if (port.isConjugated()) {
                        if (!port.getRequired().contains(el.getID())) {
                            me.m_contract.innerAddToOtherSet(port.getRequired(), el);
                        }
                    } else {
                        if (!port.getProvided().contains(el.getID())) {
                             me.m_contract.innerAddToOtherSet(port.getProvided(), el);
                        }
                    }
            }
        }
    }
}

TypedSet<Interface, InterfaceRealization>& InterfaceRealization::getContractSingleton() {
    return m_contract;
}

TypedSet<BehavioredClassifier, InterfaceRealization>& InterfaceRealization::getImplementingClassifierSingleton() {
    return m_implementingClassifier;
}

void InterfaceRealization::init() {
    m_contract.subsets(m_suppliers);
    m_implementingClassifier.subsets(m_clients);
    m_implementingClassifier.subsets(*m_owner);
    m_implementingClassifier.opposite(&BehavioredClassifier::getInterfaceRealizations);
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