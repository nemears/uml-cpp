#include "uml/types/classifier.h"
#include "uml/types/namedElement.h"
#include "uml/types/packageableElement.h"
#include "uml/types/parameterableElement.h"
#include "uml/uml-stable.h"

using namespace UML;

void BehavioredClassifier::InterfaceRealizationPolicy::elementRemoved(InterfaceRealization& el, BehavioredClassifier& me) {
    if (el.getContract()) {
        std::list<ClassifierPtr> queue = {&me};
        while (!queue.empty()) {
            ClassifierPtr front = queue.front();
            queue.pop_front();
            for (auto& referencePair  : me.getNode(*front)->m_references) {
                auto& reference = referencePair.second;
                auto referenceEl = reference.m_node.lock()->m_ptr;
                if (referenceEl->is<Port>()) {
                    Port& port = referenceEl->as<Port>();
                    if (front->getID() == port.getType().id()) {
                        if (port.isConjugated()) {
                            if (port.getRequired().contains(el.as<InterfaceRealization>().getContract().id())) {
                                me.removeFromReadonlySet(port.m_required, *el.getContract());
                            }
                        } else {
                            if (port.getProvided().contains(el.as<InterfaceRealization>().getContract().id())) {
                                me.removeFromReadonlySet(port.m_provided, *el.getContract());
                            }
                        }
                    }
                } else if (referenceEl->is<Classifier>()) {
                    if (referenceEl->as<Classifier>().getGenerals().contains(*front)) {
                        queue.push_back(&referenceEl->as<Classifier>());
                    }
                }
            }   
        }
    }
}

void BehavioredClassifier::InterfaceRealizationPolicy::elementAdded(InterfaceRealization& el, BehavioredClassifier& me) {
    if (el.getContract()) {
        std::list<ClassifierPtr> queue = {&me};
        while (!queue.empty()) {
            ClassifierPtr front = queue.front();
            queue.pop_front();
            for (auto& referencePair : me.getNode(*front)->m_references) {
                auto& reference = referencePair.second;
                auto referencedEl = reference.m_node.lock()->m_ptr;
                if (referencedEl->is<Port>()) {
                    Port& port = referencedEl->as<Port>();
                    if (port.getType().id() == front.id()) {
                        if (port.isConjugated()) {
                            if (!port.getRequired().contains(el.as<InterfaceRealization>().getContract().id())) {
                                me.addToReadonlySet(port.m_required, *el.getContract());
                            }
                        } else {
                            if (!port.getProvided().contains(el.as<InterfaceRealization>().getContract().id())) {
                                me.addToReadonlySet(port.m_provided, *el.getContract());
                            }
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
