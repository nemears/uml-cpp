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
                for (auto& referencePair : me.getNode(*front)->m_references) {
                    auto& reference = referencePair.second;
                    auto referencedEl = std::dynamic_pointer_cast<BaseElement<UmlTypes>>(reference.m_node.lock()->m_ptr);
                    if (referencedEl->is<Port>()) {
                        Port& port = referencedEl->as<Port>();
                        if (port.isConjugated()) {
                            if (port.getProvided().contains(supplier.getID())) {
                                me.removeFromReadonlySet(port.getProvided(), supplier);
                            }
                        } else {
                            if (port.getRequired().contains(supplier.getID())) {
                                me.removeFromReadonlySet(port.getRequired(), supplier);
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

void Usage::ClientPolicy::elementAdded(NamedElement& el, Usage& me) {
    if (el.is<Classifier>()) {
        for (auto& supplier : me.getSuppliers()) {
            if (supplier.is<Interface>()) {
                std::list<Classifier*> queue = {&el.as<Classifier>()};
                while(!queue.empty()) {
                    ClassifierPtr front = queue.front();
                    queue.pop_front();
                    for (auto& referencePair : me.getNode(*front)->m_references) {
                        auto& reference = referencePair.second;
                        auto referencedEl = std::dynamic_pointer_cast<BaseElement<UmlTypes>>(reference.m_node.lock()->m_ptr);
                        if (referencedEl->is<Port>()) {
                            Port& port = referencedEl->as<Port>();
                            if (port.getType().id() == front->getID()) {
                                if (port.isConjugated()) {
                                    me.addToReadonlySet(port.getProvided(), supplier);
                                } else {
                                    me.addToReadonlySet(port.getRequired(), supplier);
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
