#include "uml/behavior.h"
#include "uml/behavioralFeature.h"
#include "uml/operation.h"

using namespace UML;

void Behavior::AddParameterFunctor::operator()(Element& el) const {
    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalAdd(el);
    }

    if (dynamic_cast<Behavior*>(m_el)->getSpecification()) {
        dynamic_cast<Parameter&>(el).setOperation(dynamic_cast<Operation*>(dynamic_cast<Behavior*>(m_el)->getSpecification()));
    }
}

void Behavior::RemoveParameterFunctor::operator()(Element& el) const {
    if (dynamic_cast<Behavior*>(m_el)->getSpecification()) {
        if (dynamic_cast<Parameter&>(el).getOperation() == dynamic_cast<Behavior*>(m_el)->getSpecification()) {

            bool usedElsewhere = false;

            // note slow performance for removing
            for (auto& method : dynamic_cast<Operation*>(dynamic_cast<Parameter&>(el).getOperation())->getMethods()) {
                if (method.getID() != m_el->getID()) {
                    if (method.getParameters().count(el.getID())) {
                        usedElsewhere = true;
                    }
                }
            }

            if (!usedElsewhere) {
                dynamic_cast<Parameter&>(el).setOperation(0);
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
    m_specification = 0;
}

Behavior::~Behavior() {
    
}

Sequence<Parameter>& Behavior::getParameters() {
    return m_parameters;
}

BehavioralFeature* Behavior::getSpecification() {
    return m_specification;
}

void Behavior::setSpecification(BehavioralFeature* specification) {
    if (m_specification) {
        if (m_specification != specification) {
            if (m_specification->getMethods().count(m_id)) {
                m_specification->getMethods().remove(*this);
            }
        }
    }
    m_specification = specification;
    if (m_specification) {
        if (!m_specification->getMethods().count(m_id)) {
            m_specification->getMethods().add(*this);
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