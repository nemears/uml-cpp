#include "uml/managers/abstractManager.h"
#include "uml/types/dependency.h"
#include "uml/types/namedElement.h"
#include "uml/types/parameterableElement.h"
#include "uml/uml-stable.h"

using namespace UML;

void Usage::ClientPolicy::elementRemoved(NamedElement& el, Usage& me) {
    for (auto& supplier : me.m_suppliers) {
        if (supplier.is<Interface>() && el.is<Classifier>()) {
            std::list<ClassifierPtr> queue = {&el.as<Classifier>()};
            while (!queue.empty()) {
                ClassifierPtr front = queue.front();
                queue.pop_front();
                for (auto& reference : me.getNode(*front)->m_references) {
                // for (auto& pair : front->m_node->m_references) {
                    // if (!pair.second.node || !pair.second.node->m_element) {
                    //     // TODO maybe aquire so not lossy
                    //     continue;
                    // }
                    auto referencedEl = std::dynamic_pointer_cast<BaseElement<UmlTypes>>(reference.m_node.lock()->m_ptr);
                    if (referencedEl->is<Port>()) {
                        Port& port = referencedEl->as<Port>();
                        if (port.isConjugated()) {
                            if (port.getProvided().contains(supplier.getID())) {
                                me.removeFromReadonlySet(port.getProvided(), supplier);
                                // port.getProvided().innerRemove(&supplier);
                            }
                        } else {
                            if (port.getRequired().contains(supplier.getID())) {
                                me.removeFromReadonlySet(port.getRequired(), supplier);
                                // port.getRequired().innerRemove(&supplier);
                            }
                        }
                    } else if (referencedEl->is<Classifier>()) {
                        if (referencedEl->as<Classifier>().getGenerals().contains(*front)) {
                            // queue.push_back(&pair.second.node->m_element->as<Classifier>());
                            queue.push_back(&referencedEl->as<Classifier>());
                        }
                    }
                }
            }
        }
    }
}

void Usage::ClientPolicy::elementAdded(NamedElement& el, Usage& me) {
    if (el.is<Classifier>()) {
        for (auto& supplier : me.getSuppliers()) {
            if (supplier.is<Interface>()) {
                std::list<Classifier*> queue = {&el.as<Classifier>()};
                while(!queue.empty()) {
                    ClassifierPtr front = queue.front();
                    queue.pop_front();
                    for (auto& reference : me.getNode(*front)->m_references) {
                        // if (!pair.second.node || !pair.second.node->m_element) {
                        //     // TODO maybe aquire so not lossy
                        //     continue;
                        // }

                        auto referencedEl = std::dynamic_pointer_cast<BaseElement<UmlTypes>>(reference.m_node.lock()->m_ptr);
                        if (referencedEl->is<Port>()) {
                            Port& port = referencedEl->as<Port>();
                            if (port.getType().id() == front->getID()) {
                                if (port.isConjugated()) {
                                    me.addToReadonlySet(port.getProvided(), supplier);
                                    // port.getProvided().innerAdd(&supplier);
                                } else {
                                    me.addToReadonlySet(port.getRequired(), supplier);
                                    // port.getRequired().innerAdd(&supplier);
                                }
                            }
                        } else if (referencedEl->is<Classifier>()) {
                            if (referencedEl->as<Classifier>().getGenerals().contains(*front)) {
                                queue.push_back(&referencedEl->as<Classifier>());
                            }
                        }
                    }

                }
            }
        }
    }
}

Usage::Usage(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    Dependency(elementType, manager)
{
    m_usageClients.redefines(m_clients);
}
