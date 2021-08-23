#include "uml/behavior.h"
#include "uml/behavioralFeature.h"
#include "uml/operation.h"
#include "uml/parameter.h"
#include "uml/universalFunctions.h"

using namespace UML;

void Behavior::AddParameterFunctor::operator()(Parameter& el) const {
    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalAdd(el);
    }

    if (m_el->getSpecification()) {
        el.setOperation(dynamic_cast<Operation*>(m_el->getSpecification()));
    }
}

void Behavior::RemoveParameterFunctor::operator()(Parameter& el) const {
    if (m_el->getSpecification()) {
        if (el.getOperation() == m_el->getSpecification()) {

            bool usedElsewhere = false;

            // note slow performance for removing
            for (auto& method : dynamic_cast<Operation*>(el.getOperation())->getMethods()) {
                if (method.getID() != m_el->getID()) {
                    if (method.getParameters().count(el.getID())) {
                        usedElsewhere = true;
                    }
                }
            }

            if (!usedElsewhere) {
                el.setOperation(0);
            }
        }
    }

    if (m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalRemove(el);
    }
}

Behavior::Behavior() {
    m_parameters.addProcedures.push_back(new AddParameterFunctor(this));
    m_parameters.removeProcedures.push_back(new RemoveParameterFunctor(this));
    m_specificationPtr = 0;
    m_behavioredClassifierPtr = 0;
}

Behavior::~Behavior() {
    
}

Sequence<Parameter>& Behavior::getParameters() {
    return m_parameters;
}

BehavioralFeature* Behavior::getSpecification() {
    return universalGet<BehavioralFeature>(m_specificationID, m_specificationPtr, m_manager);
}

void Behavior::setSpecification(BehavioralFeature* specification) {
    if (!isSameOrNull(m_specificationID, specification)) {
        if (!m_specificationPtr) {
            m_specificationPtr = &m_manager->get<BehavioralFeature>(m_specificationID);
        }

        if (m_specificationPtr->getMethods().count(m_id)) {
            m_specificationPtr->getMethods().remove(*this);
        }

        m_specificationPtr = 0;
        m_specificationID = ID::nullID();
    }

    if (specification) {
        m_specificationID = specification->getID();
    }

    if (!m_manager) {
        m_specificationPtr = specification;
    }

    if (specification) {
        if (!specification->getMethods().count(m_id)) {
            specification->getMethods().add(*this);
        }
    }
}

BehavioredClassifier* Behavior::getBehavioredClassifier() {
    return universalGet<BehavioredClassifier>(m_behavioredClassifierID, m_behavioredClassifierPtr, m_manager);
}

void Behavior::setBehavioredClassifier(BehavioredClassifier* classifier) {
    if (!isSameOrNull(m_behavioredClassifierID, classifier)) {
        if (!m_behavioredClassifierPtr) {
            m_behavioredClassifierPtr = &m_manager->get<BehavioredClassifier>(m_behavioredClassifierID);
        }

        if (m_behavioredClassifierPtr->getOwnedBehaviors().count(m_id)) {
            m_behavioredClassifierPtr->getOwnedBehaviors().remove(*this);
        }

        m_behavioredClassifierPtr = 0;
        m_behavioredClassifierID = ID::nullID();
    }

    if (classifier) {
        m_behavioredClassifierID = classifier->getID();
    }

    if (!m_manager) {
        m_behavioredClassifierPtr = classifier;
    }

    if (classifier) {
        if (!classifier->getOwnedBehaviors().count(m_id)) {
            classifier->getOwnedBehaviors().add(*this);
        }
    }
}

ElementType Behavior::getElementType() const {
    return ElementType::BEHAVIOR;
}

bool Behavior::isSubClassOf(ElementType eType) const {
    bool ret = Class::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::BEHAVIOR;
    }
    
    return ret;
}