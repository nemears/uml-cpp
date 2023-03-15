#include "uml/types/usage.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

void Usage::RemoveClientPolicy::apply(NamedElement& el, Usage& me) {
    for (auto& supplier : me.m_suppliers) {
        if (supplier.isSubClassOf(ElementType::INTERFACE_UML) && el.isSubClassOf(ElementType::CLASSIFIER)) {
            // for (auto& pair : el.m_node->m_references) {
                std::list<Classifier*> queue = {&el.as<Classifier>()};
                while (!queue.empty()) {
                    Classifier* front = queue.front();
                    queue.pop_front();
                    for (auto& pair : front->m_node->m_references) {
                        if (!pair.second.node || !pair.second.node->m_managerElementMemory) {
                            // TODO maybe aquire so not lossy
                            continue;
                        }
                        
                        if (pair.second.node->m_managerElementMemory->isSubClassOf(ElementType::PORT)) {
                            Port& port = pair.second.node->m_managerElementMemory->as<Port>();
                            if (port.isConjugated()) {
                                if (port.getProvided().contains(supplier.getID())) {
                                    port.getProvided().innerRemove(supplier.getID());
                                }
                            } else {
                                if (port.getRequired().contains(supplier.getID())) {
                                    port.getRequired().innerRemove(supplier.getID());
                                }
                            }
                        } else if (pair.second.node->m_managerElementMemory->isSubClassOf(ElementType::CLASSIFIER)) {
                            if (pair.second.node->m_managerElementMemory->as<Classifier>().getGenerals().contains(*front)) {
                                queue.push_back(&pair.second.node->m_managerElementMemory->as<Classifier>());
                            }
                        }
                    }
                }
            // }
        }
    }
}

void Usage::AddClientPolicy::apply(NamedElement& el, Usage& me) {
    if (el.isSubClassOf(ElementType::CLASSIFIER)) {
        for (auto& supplier : me.getSuppliers()) {
            if (supplier.isSubClassOf(ElementType::INTERFACE_UML)) {
                std::list<Classifier*> queue = {&el.as<Classifier>()};
                while(!queue.empty()) {
                    Classifier* front = queue.front();
                    queue.pop_front();
                    for (auto& pair : front->m_node->m_references) {
                        if (!pair.second.node || !pair.second.node->m_managerElementMemory) {
                            // TODO maybe aquire so not lossy
                            continue;
                        }

                        if (pair.second.node->m_managerElementMemory->isSubClassOf(ElementType::PORT)) {
                            Port& port = pair.second.node->m_managerElementMemory->as<Port>();
                            if (port.getType().id() == front->getID()) {
                                if (port.isConjugated()) {
                                    port.getProvided().innerAdd(supplier.as<Interface>());
                                } else {
                                    port.getRequired().innerAdd(supplier.as<Interface>());
                                }
                            }
                        } else if (pair.second.node->m_managerElementMemory->isSubClassOf(ElementType::CLASSIFIER)) {
                            if (pair.second.node->m_managerElementMemory->as<Classifier>().getGenerals().contains(*front)) {
                                queue.push_back(&pair.second.node->m_managerElementMemory->as<Classifier>());
                            }
                        }
                    }

                }
            }
        }
        
    }
}

Usage::Usage() : Element(ElementType::USAGE) {
    m_usageClients.redefines(m_clients);
}

Usage::~Usage() {
    
}

bool Usage::isSubClassOf(ElementType eType) const {
    bool ret = Dependency::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::USAGE;
    }

    return ret;
}