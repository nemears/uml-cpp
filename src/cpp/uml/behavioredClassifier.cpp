#include "uml/behavioredClassifier.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/umlPtr.h"
#include "uml/stereotype.h"
#include "uml/deployment.h"

using namespace UML;

void BehavioredClassifier::RemoveInterfaceRealizationFunctor::operator()(Element& el) const {
    if (el.as<InterfaceRealization>().getContract()) {
        std::list<Classifier*> queue = {&m_el.as<Classifier>()};
        while (!queue.empty()) {
            Classifier* front = queue.front();
            queue.pop_front();
            for (auto& pair : front->m_node->m_references) {
                if (pair.second && pair.second->m_managerElementMemory->isSubClassOf(ElementType::PORT)) {
                    if (front->getID() == pair.second->m_managerElementMemory->as<Port>().getType().id()) {
                        if (pair.second->m_managerElementMemory->as<Port>().isConjugated()) {
                            if (pair.second->m_managerElementMemory->as<Port>().getRequired().contains(el.as<InterfaceRealization>().getContract().id())) {
                                pair.second->m_managerElementMemory->as<Port>().getRequired().removeReadOnly(el.as<InterfaceRealization>().getContract().id());
                            }
                        } else {
                            if (pair.second->m_managerElementMemory->as<Port>().getProvided().contains(el.as<InterfaceRealization>().getContract().id())) {
                                pair.second->m_managerElementMemory->as<Port>().getProvided().removeReadOnly(el.as<InterfaceRealization>().getContract().id());
                            }
                        }
                    }
                } else if (pair.second->m_managerElementMemory->isSubClassOf(ElementType::CLASSIFIER)) {
                    if (pair.second->m_managerElementMemory->as<Classifier>().getGenerals().contains(*front)) {
                        queue.push_back(&pair.second->m_managerElementMemory->as<Classifier>());
                    }
                }
            }   
        }
    }
}

void BehavioredClassifier::AddInterfaceRealizationFunctor::operator()(Element& el) const {
    if (el.as<InterfaceRealization>().getContract()) {
        std::list<Classifier*> queue = {&m_el.as<Classifier>()};
        while (!queue.empty()) {
            Classifier* front = queue.front();
            queue.pop_front();
            for (auto& pair : front->m_node->m_references) {
                if (pair.second && pair.second->m_managerElementMemory->isSubClassOf(ElementType::PORT)) {
                    if (pair.second->m_managerElementMemory->as<Port>().getType().id() == front->m_id) {
                        if (pair.second->m_managerElementMemory->as<Port>().isConjugated()) {
                            if (!pair.second->m_managerElementMemory->as<Port>().getRequired().contains(el.as<InterfaceRealization>().getContract().id())) {
                                pair.second->m_managerElementMemory->as<Port>().getRequired().nonOppositeAdd(*el.as<InterfaceRealization>().getContract());
                            }
                        } else {
                            if (!pair.second->m_managerElementMemory->as<Port>().getProvided().contains(el.as<InterfaceRealization>().getContract().id())) {
                                pair.second->m_managerElementMemory->as<Port>().getProvided().nonOppositeAdd(*el.as<InterfaceRealization>().getContract());
                            }
                        }
                    }
                } else if (pair.second->m_managerElementMemory->isSubClassOf(ElementType::CLASSIFIER)) {
                    if (pair.second->m_managerElementMemory->as<Classifier>().getGenerals().contains(*front)) {
                        queue.push_back(&pair.second->m_managerElementMemory->as<Classifier>());
                    }
                }
            }
        }
    }
}

Set<Behavior, BehavioredClassifier>& BehavioredClassifier::getClassifierBehaviorSingleton() {
    return m_classifierBehavior;
}

void BehavioredClassifier::init() {
    m_ownedBehaviors.subsets(m_ownedMembers);
    m_ownedBehaviors.m_signature = &BehavioredClassifier::getOwnedBehaviors;
    m_classifierBehavior.subsets(m_ownedBehaviors);
    m_classifierBehavior.m_signature = &BehavioredClassifier::getClassifierBehaviorSingleton;
    m_interfaceRealizations.subsets(*m_ownedElements);
    m_interfaceRealizations.opposite(&InterfaceRealization::getImplementingClassifierSingleton);
    m_interfaceRealizations.m_signature = &BehavioredClassifier::getInterfaceRealizations;
    m_interfaceRealizations.m_addFunctors.insert(new AddInterfaceRealizationFunctor(this));
    m_interfaceRealizations.m_removeFunctors.insert(new RemoveInterfaceRealizationFunctor(this));
}

BehavioredClassifier::BehavioredClassifier() : Element(ElementType::BEHAVIORED_CLASSIFIER) {
    init();
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