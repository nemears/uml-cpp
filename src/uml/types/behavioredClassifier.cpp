#include "uml/types/classifier.h"
#include "uml/types/namedElement.h"
#include "uml/types/packageableElement.h"
#include "uml/types/parameterableElement.h"
#include "uml/uml-stable.h"

using namespace UML;

void BehavioredClassifier::InterfaceRealizationPolicy::elementRemoved(InterfaceRealization& el, BehavioredClassifier& me) {
//     if (el.getContract()) {
//         std::list<Classifier*> queue = {&me};
//         while (!queue.empty()) {
//             Classifier* front = queue.front();
//             queue.pop_front();
//             for (auto& pair : front->m_node->m_references) {
//                 if (!pair.second.node || !pair.second.node->m_element) {
//                     // TODO aquire?
//                 }
//                 if (pair.second.node && pair.second.node->m_element->is(ElementType::PORT)) {
//                     Port& port = pair.second.node->m_element->as<Port>();
//                     if (front->getID() == port.getType().id()) {
//                         if (port.isConjugated()) {
//                             if (port.getRequired().contains(el.as<InterfaceRealization>().getContract().id())) {
//                                 port.m_required.innerRemove(el.getContract());
//                             }
//                         } else {
//                             if (port.getProvided().contains(el.as<InterfaceRealization>().getContract().id())) {
//                                 port.m_provided.innerRemove(el.getContract());
//                             }
//                         }
//                     }
//                 } else if (pair.second.node && pair.second.node->m_element->is(ElementType::CLASSIFIER)) {
//                     if (pair.second.node->m_element->as<Classifier>().getGenerals().contains(*front)) {
//                         queue.push_back(&pair.second.node->m_element->as<Classifier>());
//                     }
//                 }
//             }   
//         }
//     }
}

void BehavioredClassifier::InterfaceRealizationPolicy::elementAdded(InterfaceRealization& el, BehavioredClassifier& me) {
//     if (el.getContract()) {
//         std::list<Classifier*> queue = {&me};
//         while (!queue.empty()) {
//             Classifier* front = queue.front();
//             queue.pop_front();
//             for (auto& pair : front->m_node->m_references) {
//                 if (!pair.second.node || !pair.second.node->m_element) {
//                     // TODO aquire?
//                 }
//                 if (pair.second.node && pair.second.node->m_element->is(ElementType::PORT)) {
//                     Port& port = pair.second.node->m_element->as<Port>();
//                     if (port.getType().id() == front->m_id) {
//                         if (port.isConjugated()) {
//                             if (!port.getRequired().contains(el.as<InterfaceRealization>().getContract().id())) {
//                                 port.m_required.innerAdd(el.getContract());
//                             }
//                         } else {
//                             if (!port.getProvided().contains(el.as<InterfaceRealization>().getContract().id())) {
//                                 port.m_provided.innerAdd(el.getContract());
//                             }
//                         }
//                     }
//                 } else if (pair.second.node && pair.second.node->m_element->is(ElementType::CLASSIFIER)) {
//                     if (pair.second.node->m_element->as<Classifier>().getGenerals().contains(*front)) {
//                         queue.push_back(&pair.second.node->m_element->as<Classifier>());
//                     }
//                 }
//             }
//         }
//     }
}

Singleton<Behavior, BehavioredClassifier>& BehavioredClassifier::getClassifierBehaviorSingleton() {
    return m_classifierBehavior;
}

BehavioredClassifier::BehavioredClassifier(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    PackageableElement(elementType, manager),
    Classifier(elementType, manager)
{
    m_ownedBehaviors.subsets(m_ownedMembers);
    m_classifierBehavior.subsets(m_ownedBehaviors);
    m_interfaceRealizations.subsets(m_ownedElements);
    m_interfaceRealizations.opposite(&InterfaceRealization::getImplementingClassifierSingleton);
}

IndexableSet<Behavior, BehavioredClassifier>& BehavioredClassifier::getOwnedBehaviors() {
    return m_ownedBehaviors;
}

BehaviorPtr BehavioredClassifier::getClassifierBehavior() const {
    return m_classifierBehavior.get();
}

void BehavioredClassifier::setClassifierBehavior(BehaviorPtr behavior) {
    m_classifierBehavior.set(behavior);
}

void BehavioredClassifier::setClassifierBehavior(Behavior& behavior) {
    m_classifierBehavior.set(behavior);
}

void BehavioredClassifier::setClassifierBehavior(ID id) {
    m_classifierBehavior.set(id);
}

Set<InterfaceRealization, BehavioredClassifier, BehavioredClassifier::InterfaceRealizationPolicy>& BehavioredClassifier::getInterfaceRealizations() {
    return m_interfaceRealizations;
}
