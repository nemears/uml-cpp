#include "uml/usage.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

// void Usage::RemoveClientFunctor::operator()(Element& el) const {
//     for (auto& supplier : m_el.as<Usage>().m_suppliers) {
//         if (supplier.isSubClassOf(ElementType::INTERFACE_UML) && el.isSubClassOf(ElementType::CLASSIFIER)) {
//             // for (auto& pair : el.m_node->m_references) {
//                 std::list<Classifier*> queue = {&el.as<Classifier>()};
//                 while (!queue.empty()) {
//                     Classifier* front = queue.front();
//                     queue.pop_front();
//                     for (auto& pair : front->m_node->m_references) {
//                         if (!pair.second.node || !pair.second.node->m_managerElementMemory) {
//                             // TODO maybe aquire so not lossy
//                             continue;
//                         }
                        
//                         if (pair.second.node->m_managerElementMemory->isSubClassOf(ElementType::PORT)) {
//                             Port& port = pair.second.node->m_managerElementMemory->as<Port>();
//                             if (port.isConjugated()) {
//                                 if (port.getProvided().contains(supplier.getID())) {
//                                     port.getProvided().removeReadOnly(supplier.getID());
//                                 }
//                             } else {
//                                 if (port.getRequired().contains(supplier.getID())) {
//                                     port.getRequired().removeReadOnly(supplier.getID());
//                                 }
//                             }
//                         } else if (pair.second.node->m_managerElementMemory->isSubClassOf(ElementType::CLASSIFIER)) {
//                             if (pair.second.node->m_managerElementMemory->as<Classifier>().getGenerals().contains(*front)) {
//                                 queue.push_back(&pair.second.node->m_managerElementMemory->as<Classifier>());
//                             }
//                         }
//                     }
//                 }
//             // }
//         }
//     }
// }

// void Usage::SetClientFunctor::operator()(Element& el) const {
//     if (el.isSubClassOf(ElementType::CLASSIFIER)) {
//         for (auto& supplier : m_el.as<Usage>().getSuppliers()) {
//             if (supplier.isSubClassOf(ElementType::INTERFACE_UML)) {
//                 std::list<Classifier*> queue = {&el.as<Classifier>()};
//                 while(!queue.empty()) {
//                     Classifier* front = queue.front();
//                     queue.pop_front();
//                     for (auto& pair : front->m_node->m_references) {
//                         if (!pair.second.node || !pair.second.node->m_managerElementMemory) {
//                             // TODO maybe aquire so not lossy
//                             continue;
//                         }

//                         if (pair.second.node->m_managerElementMemory->isSubClassOf(ElementType::PORT)) {
//                             Port& port = pair.second.node->m_managerElementMemory->as<Port>();
//                             if (port.getType().id() == front->getID()) {
//                                 if (port.isConjugated()) {
//                                     port.getProvided().nonOppositeAdd(supplier.as<Interface>());
//                                 } else {
//                                     port.getRequired().nonOppositeAdd(supplier.as<Interface>());
//                                 }
//                             }
//                         } else if (pair.second.node->m_managerElementMemory->isSubClassOf(ElementType::CLASSIFIER)) {
//                             if (pair.second.node->m_managerElementMemory->as<Classifier>().getGenerals().contains(*front)) {
//                                 queue.push_back(&pair.second.node->m_managerElementMemory->as<Classifier>());
//                             }
//                         }
//                     }

//                 }
//             }
//         }
        
//     }
// }

Usage::Usage() : Element(ElementType::USAGE) {
    
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