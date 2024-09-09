#include "uml/managers/abstractManager.h"
#include "uml/types/dependency.h"
#include "uml/types/namedElement.h"
#include "uml/types/parameterableElement.h"
#include "uml/uml-stable.h"

using namespace UML;

void Usage::ClientPolicy::elementRemoved(NamedElement& el, Usage& me) {
    // for (auto& supplier : me.m_suppliers) {
    //     if (supplier.is<Interface>() && el.is<Classifier>()) {
    //         // for (auto& pair : el.m_node->m_references) {
    //             std::list<Classifier*> queue = {&el.as<Classifier>()};
    //             while (!queue.empty()) {
    //                 Classifier* front = queue.front();
    //                 queue.pop_front();
    //                 for (auto& pair : front->m_node->m_references) {
    //                     if (!pair.second.node || !pair.second.node->m_element) {
    //                         // TODO maybe aquire so not lossy
    //                         continue;
    //                     }
    //                     
    //                     if (pair.second.node->m_element->is(ElementType::PORT)) {
    //                         Port& port = pair.second.node->m_element->as<Port>();
    //                         if (port.isConjugated()) {
    //                             if (port.getProvided().contains(supplier.getID())) {
    //                                 port.getProvided().innerRemove(&supplier);
    //                             }
    //                         } else {
    //                             if (port.getRequired().contains(supplier.getID())) {
    //                                 port.getRequired().innerRemove(&supplier);
    //                             }
    //                         }
    //                     } else if (pair.second.node->m_element->is(ElementType::CLASSIFIER)) {
    //                         if (pair.second.node->m_element->as<Classifier>().getGenerals().contains(*front)) {
    //                             queue.push_back(&pair.second.node->m_element->as<Classifier>());
    //                         }
    //                     }
    //                 }
    //             }
    //         // }
    //     }
    // }
}

void Usage::ClientPolicy::elementAdded(NamedElement& el, Usage& me) {
    // if (el.is(ElementType::CLASSIFIER)) {
    //     for (auto& supplier : me.getSuppliers()) {
    //         if (supplier.is(ElementType::INTERFACE_UML)) {
    //             std::list<Classifier*> queue = {&el.as<Classifier>()};
    //             while(!queue.empty()) {
    //                 Classifier* front = queue.front();
    //                 queue.pop_front();
    //                 for (auto& pair : front->m_node->m_references) {
    //                     if (!pair.second.node || !pair.second.node->m_element) {
    //                         // TODO maybe aquire so not lossy
    //                         continue;
    //                     }

    //                     if (pair.second.node->m_element->is(ElementType::PORT)) {
    //                         Port& port = pair.second.node->m_element->as<Port>();
    //                         if (port.getType().id() == front->getID()) {
    //                             if (port.isConjugated()) {
    //                                 port.getProvided().innerAdd(&supplier);
    //                             } else {
    //                                 port.getRequired().innerAdd(&supplier);
    //                             }
    //                         }
    //                     } else if (pair.second.node->m_element->is(ElementType::CLASSIFIER)) {
    //                         if (pair.second.node->m_element->as<Classifier>().getGenerals().contains(*front)) {
    //                             queue.push_back(&pair.second.node->m_element->as<Classifier>());
    //                         }
    //                     }
    //                 }

    //             }
    //         }
    //     }
    // }
}

Usage::Usage(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    Dependency(elementType, manager)
{
    m_usageClients.redefines(m_clients);
}
