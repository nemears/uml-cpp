#include "uml/port.h"
#include "uml/operation.h"
#include "uml/manifestation.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

// void Port::setPortInterfaces(BehavioredClassifier& clazz) {
//     for (auto& realization : clazz.getInterfaceRealizations()) {
//         if (realization.getContract()) {
//             if (isConjugated()) {
//                 m_required.addReadOnly(*realization.getContract());
//             } else {
//                m_provided.addReadOnly(*realization.getContract());
//             }
//             realization.getContract()->setReference(this);
//         }
//     }
//     for (auto& dependency : clazz.getClientDependencies()) {
//         if (dependency.isSubClassOf(ElementType::USAGE)) {
//             for (auto& supplier : dependency.getSuppliers()) {
//                 if (supplier.isSubClassOf(ElementType::INTERFACE_UML)) {
//                     if (isConjugated()) {
//                        m_provided.nonOppositeAdd(supplier.as<Interface>());
//                     } else {
//                         m_required.nonOppositeAdd(supplier.as<Interface>());
//                     }
//                 }
//             }
//         }
//     }
//     for (auto& general : clazz.getGenerals()) {
//         if (general.isSubClassOf(ElementType::BEHAVIORED_CLASSIFIER)) {
//             setPortInterfaces(general.as<BehavioredClassifier>());
//         }
//     }
// }

// void Port::SetTypeFunctor::operator()(Element& el) const {
//     if (el.isSubClassOf(ElementType::BEHAVIORED_CLASSIFIER)) {
//         m_el.as<Port>().setPortInterfaces(el.as<BehavioredClassifier>());
//     } else if (el.isSubClassOf(ElementType::INTERFACE_UML)) {
//         if (m_el.as<Port>().isConjugated()) {
//             m_el.as<Port>().m_required.nonOppositeAdd(el.as<Interface>());
//         } else {
//             m_el.as<Port>().m_provided.nonOppositeAdd(el.as<Interface>());
//         }
//     }
// }

// void Port::removePortInterfaces(BehavioredClassifier& clazz) {
//     for (auto& realization : clazz.getInterfaceRealizations()) {
//         if (realization.getContract()) {
//             if (isConjugated()) {
//                 m_required.removeReadOnly(realization.getContract().id());
//             } else {
//                m_provided.removeReadOnly(realization.getContract().id());
//             }
//             realization.getContract()->removeReference(m_id);
//         }
//     }
//     for (auto& dependency : clazz.getClientDependencies()) {
//         if (dependency.isSubClassOf(ElementType::USAGE)) {
//             for (auto& supplier : dependency.getSuppliers()) {
//                 if (supplier.isSubClassOf(ElementType::INTERFACE_UML)) {
//                     if (isConjugated()) {
//                        m_provided.removeReadOnly(supplier.getID());
//                     } else {
//                         m_required.removeReadOnly(supplier.getID());
//                     }
//                 }
//             }
//         }
//     }
//     for (auto& general : clazz.getGenerals()) {
//         if (general.isSubClassOf(ElementType::BEHAVIORED_CLASSIFIER)) {
//             removePortInterfaces(general.as<BehavioredClassifier>());
//         }
//     }
// }

// void Port::RemoveTypeFunctor::operator()(Element& el) const {
//     if (el.isSubClassOf(ElementType::BEHAVIORED_CLASSIFIER)) {
//         m_el.as<Port>().removePortInterfaces(el.as<BehavioredClassifier>());
//     } else if (el.isSubClassOf(ElementType::INTERFACE_UML)) {
//         if (m_el.as<Port>().isComposite()) {
//             m_el.as<Port>().m_required.removeReadOnly(el.getID());
//         } else {
//             m_el.as<Port>().m_provided.removeReadOnly(el.getID());
//         }
//     }
// }

void Port::restoreReference(Element* el) {
    Property::restoreReference(el);
    if (m_required.contains(el->getID())){
        el->setReference(this);
    }
    if (m_provided.contains(el->getID())) {
        el->setReference(this);
    }
}

void Port::referenceReindexed(ID newID) {
    Property::referenceReindexed(newID);
    m_required.reindex(newID);
    m_provided.reindex(newID);
}

void Port::referenceErased(ID id) {
    Property::referenceErased(id);
    m_required.eraseElement(id);
    m_provided.eraseElement(id);
}

Port::Port() : Element(ElementType::PORT) {
    m_required.m_readOnly = true;
    m_provided.m_readOnly = true;
}

Port::~Port() {
    
}

bool Port::isBehavior() const {
    return m_isBehavior;
}

void Port::setIsBehavior(bool isBehavior) {
    m_isBehavior = isBehavior;
}

bool Port::isConjugated() const {
    return m_isConjugated;
}

void Port::setIsConjugated(bool isConjugated) {
    // if (isConjugated != m_isConjugated) {
    //     std::vector<ID> oldRequired;
    //     std::vector<ID> oldProvided;
    //     for (const ID required : m_required.ids()) {
    //         oldRequired.push_back(required);
    //     }
    //     for (const ID provided : m_provided.ids()) {
    //         oldProvided.push_back(provided);
    //     }
    //     for (auto& newProvidedID : oldRequired) {
    //         if (m_required.loaded(newProvidedID)) {
    //             Interface& newProvided = m_required.get(newProvidedID);
    //             m_required.removeReadOnly(newProvidedID);
    //             m_provided.nonOppositeAdd(newProvided);
    //         } else {
    //             m_required.forceRemove(newProvidedID);
    //             m_provided.addReadOnly(newProvidedID);
    //         }
    //     }
    //     for (auto& newRequiredID : oldProvided) {
    //         if (m_provided.loaded(newRequiredID)) {
    //             Interface& newRequired = m_provided.get(newRequiredID);
    //             m_provided.removeReadOnly(newRequiredID);
    //             m_required.nonOppositeAdd(newRequired);
    //         } else {
    //             m_provided.forceRemove(newRequiredID);
    //             m_required.addReadOnly(newRequiredID);
    //         }            
    //     }
    // }
    m_isConjugated = isConjugated;
}

bool Port::isService() const {
    return m_isService;
}

void Port::setIsService(bool isService) {
    m_isService = isService;
}

Set<Interface, Port>& Port::getRequired() {
    return m_required;
}

Set<Interface, Port>& Port::getProvided() {
    return m_provided;
}

bool Port::isSubClassOf(ElementType eType) const {
    bool ret = Property::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PORT;
    }

    return ret;
}