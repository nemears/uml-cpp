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
                if (!pair.second.node || !pair.second.node->m_managerElementMemory) {
                    // TODO aquire?
                }
                if (pair.second.node && pair.second.node->m_managerElementMemory->isSubClassOf(ElementType::PORT)) {
                    Port& port = pair.second.node->m_managerElementMemory->as<Port>();
                    if (front->getID() == port.getType().id()) {
                        if (port.isConjugated()) {
                            if (port.getRequired().contains(el.as<InterfaceRealization>().getContract().id())) {
                                port.getRequired().removeReadOnly(el.as<InterfaceRealization>().getContract().id());
                            }
                        } else {
                            if (port.getProvided().contains(el.as<InterfaceRealization>().getContract().id())) {
                                port.getProvided().removeReadOnly(el.as<InterfaceRealization>().getContract().id());
                            }
                        }
                    }
                } else if (pair.second.node && pair.second.node->m_managerElementMemory->isSubClassOf(ElementType::CLASSIFIER)) {
                    if (pair.second.node->m_managerElementMemory->as<Classifier>().getGenerals().contains(*front)) {
                        queue.push_back(&pair.second.node->m_managerElementMemory->as<Classifier>());
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
                if (!pair.second.node || !pair.second.node->m_managerElementMemory) {
                    // TODO aquire?
                }
                if (pair.second.node && pair.second.node->m_managerElementMemory->isSubClassOf(ElementType::PORT)) {
                    Port& port = pair.second.node->m_managerElementMemory->as<Port>();
                    if (port.getType().id() == front->m_id) {
                        if (port.isConjugated()) {
                            if (!port.getRequired().contains(el.as<InterfaceRealization>().getContract().id())) {
                                port.getRequired().nonOppositeAdd(*el.as<InterfaceRealization>().getContract());
                            }
                        } else {
                            if (!port.getProvided().contains(el.as<InterfaceRealization>().getContract().id())) {
                                port.getProvided().nonOppositeAdd(*el.as<InterfaceRealization>().getContract());
                            }
                        }
                    }
                } else if (pair.second.node && pair.second.node->m_managerElementMemory->isSubClassOf(ElementType::CLASSIFIER)) {
                    if (pair.second.node->m_managerElementMemory->as<Classifier>().getGenerals().contains(*front)) {
                        queue.push_back(&pair.second.node->m_managerElementMemory->as<Classifier>());
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
    m_classifierBehavior.subsets(m_ownedBehaviors);
    m_interfaceRealizations.subsets(*m_ownedElements);
    m_interfaceRealizations.opposite(&InterfaceRealization::getImplementingClassifierSingleton);
    m_interfaceRealizations.addFunctor(new AddInterfaceRealizationFunctor(this));
    m_interfaceRealizations.removeFunctor(new RemoveInterfaceRealizationFunctor(this));
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