#include "uml/types/behavioredClassifier.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/umlPtr.h"
#include "uml/types/stereotype.h"
#include "uml/types/deployment.h"

using namespace UML;

void BehavioredClassifier::RemoveInterfaceRealizationPolicy::apply(InterfaceRealization& el, BehavioredClassifier& me) {
    if (el.getContract()) {
        std::list<Classifier*> queue = {&me};
        while (!queue.empty()) {
            Classifier* front = queue.front();
            queue.pop_front();
            for (auto& pair : front->m_node->m_references) {
                if (!pair.second.node || !pair.second.node->m_element) {
                    // TODO aquire?
                }
                if (pair.second.node && pair.second.node->m_element->isSubClassOf(ElementType::PORT)) {
                    Port& port = pair.second.node->m_element->as<Port>();
                    if (front->getID() == port.getType().id()) {
                        if (port.isConjugated()) {
                            if (port.getRequired().contains(el.as<InterfaceRealization>().getContract().id())) {
                                port.m_required.innerRemove(el.getContract().id());
                            }
                        } else {
                            if (port.getProvided().contains(el.as<InterfaceRealization>().getContract().id())) {
                                port.m_provided.innerRemove(el.getContract().id());
                            }
                        }
                    }
                } else if (pair.second.node && pair.second.node->m_element->isSubClassOf(ElementType::CLASSIFIER)) {
                    if (pair.second.node->m_element->as<Classifier>().getGenerals().contains(*front)) {
                        queue.push_back(&pair.second.node->m_element->as<Classifier>());
                    }
                }
            }   
        }
    }
}

void BehavioredClassifier::AddInterfaceRealizationPolicy::apply(InterfaceRealization& el, BehavioredClassifier& me) {
    if (el.getContract()) {
        std::list<Classifier*> queue = {&me};
        while (!queue.empty()) {
            Classifier* front = queue.front();
            queue.pop_front();
            for (auto& pair : front->m_node->m_references) {
                if (!pair.second.node || !pair.second.node->m_element) {
                    // TODO aquire?
                }
                if (pair.second.node && pair.second.node->m_element->isSubClassOf(ElementType::PORT)) {
                    Port& port = pair.second.node->m_element->as<Port>();
                    if (port.getType().id() == front->m_id) {
                        if (port.isConjugated()) {
                            if (!port.getRequired().contains(el.as<InterfaceRealization>().getContract().id())) {
                                port.m_required.innerAdd(*el.getContract());
                            }
                        } else {
                            if (!port.getProvided().contains(el.as<InterfaceRealization>().getContract().id())) {
                                port.m_provided.innerAdd(*el.getContract());
                            }
                        }
                    }
                } else if (pair.second.node && pair.second.node->m_element->isSubClassOf(ElementType::CLASSIFIER)) {
                    if (pair.second.node->m_element->as<Classifier>().getGenerals().contains(*front)) {
                        queue.push_back(&pair.second.node->m_element->as<Classifier>());
                    }
                }
            }
        }
    }
}

TypedSet<Behavior, BehavioredClassifier>& BehavioredClassifier::getClassifierBehaviorSingleton() {
    return m_classifierBehavior;
}

BehavioredClassifier::BehavioredClassifier() : Element(ElementType::BEHAVIORED_CLASSIFIER) {
    m_ownedBehaviors.subsets(m_ownedMembers);
    m_classifierBehavior.subsets(m_ownedBehaviors);
    m_interfaceRealizations.subsets(*m_ownedElements);
    m_interfaceRealizations.opposite(&InterfaceRealization::getImplementingClassifierSingleton);
}

BehavioredClassifier::~BehavioredClassifier() {

}

Set<Behavior, BehavioredClassifier>& BehavioredClassifier::getOwnedBehaviors() {
    return m_ownedBehaviors;
}

BehaviorPtr BehavioredClassifier::getClassifierBehavior() const {
    return m_classifierBehavior.get();
}

void BehavioredClassifier::setClassifierBehavior(Behavior* behavior) {
    m_classifierBehavior.set(behavior);
}

void BehavioredClassifier::setClassifierBehavior(Behavior& behavior) {
    m_classifierBehavior.set(behavior);
}

void BehavioredClassifier::setClassifierBehavior(ID id) {
    m_classifierBehavior.set(id);
}

Set<InterfaceRealization, BehavioredClassifier>& BehavioredClassifier::getInterfaceRealizations() {
    return m_interfaceRealizations;
}

bool BehavioredClassifier::isSubClassOf(ElementType eType) const {
    bool ret = Classifier::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::BEHAVIORED_CLASSIFIER;
    }

    return ret;
}