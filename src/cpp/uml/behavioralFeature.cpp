#include "uml/behavioralFeature.h"
#include "uml/operation.h"

using namespace UML;

void BehavioralFeature::AddMethodFunctor::operator()(Element& el) const {
    if (!dynamic_cast<Behavior&>(el).getSpecification()) {
        dynamic_cast<Behavior&>(el).setSpecification(dynamic_cast<BehavioralFeature*>(m_el));
    }
}

void BehavioralFeature::AddParameterFunctor::operator()(Element& el) const {
    if (m_el->isSubClassOf(ElementType::OPERATION)) {
        if (dynamic_cast<Parameter&>(el).getOperation() != m_el) {
            dynamic_cast<Parameter&>(el).setOperation(dynamic_cast<Operation*>(m_el));
        }
    }

    if (!dynamic_cast<BehavioralFeature*>(m_el)->getOwnedMembers().count(el.getID())) {
        dynamic_cast<BehavioralFeature*>(m_el)->getOwnedMembers().add(dynamic_cast<Parameter&>(el));
    }

    // I think we shouldn't enforce parameters being the same OMG UML 2.5.1 spec pg 289 bottom of page

    // TODO behavior must have same parameters as it's specification, but they are not the same parameters
    // TODO think about whether to be different just in memory or have different ids but same everything else
    // for (auto const& method : dynamic_cast<BehavioralFeature*>(m_el)->getMethods()) {
    //     if (!method->getParameters().count(el.getID())) {
    //         method->getParameters().add(dynamic_cast<Parameter&>(el));
    //     }
    // }
}

void BehavioralFeature::CheckParameterFunctor::operator()(Element& el) const {
    ParameterDirectionKind direction = dynamic_cast<Parameter&>(el).getDirection();
    if (direction == ParameterDirectionKind::RETURN || direction == ParameterDirectionKind::OUT || direction == ParameterDirectionKind::INOUT) {
        if (dynamic_cast<BehavioralFeature*>(m_el)->m_returnSpecified) {
            if (m_el->isSubClassOf(ElementType::OPERATION)) {
                if (dynamic_cast<Parameter&>(el).getOperation() == m_el) {
                    dynamic_cast<Parameter&>(el).setOperation(0);
                }
            }
            throw ReturnParameterException(m_el->getElementTypeString() + " " + m_el->getIDstring());
        } else {
            dynamic_cast<BehavioralFeature*>(m_el)->m_returnSpecified = true;
        }
    }
}

void BehavioralFeature::RemoveParameterFunctor::operator()(Element& el) const {
    if (m_el->isSubClassOf(ElementType::OPERATION)) {
        if (dynamic_cast<Parameter&>(el).getOperation() == m_el) {
            dynamic_cast<Parameter&>(el).setOperation(0);
        }
    }

    if (dynamic_cast<BehavioralFeature*>(m_el)->getMembers().count(el.getID())) {
        dynamic_cast<BehavioralFeature*>(m_el)->getMembers().remove(dynamic_cast<Parameter&>(el));
    }
}

BehavioralFeature::BehavioralFeature() {
    m_methods = new Sequence<Behavior>;
    m_methods->addProcedures.push_back(new AddMethodFunctor(this));
    m_ownedParameters = new Sequence<Parameter>;
    m_ownedParameters->addProcedures.push_back(new AddParameterFunctor(this));
    m_ownedParameters->addChecks.push_back(new CheckParameterFunctor(this));
    m_ownedParameters->removeProcedures.push_back(new RemoveParameterFunctor(this));
}

BehavioralFeature::~BehavioralFeature() {
    delete m_methods;
    delete m_ownedParameters;
}

Sequence<Behavior>& BehavioralFeature::getMethods() {
    return *m_methods;
}

Sequence<Parameter>& BehavioralFeature::getOwnedParameters() {
    return *m_ownedParameters;
}

bool BehavioralFeature::isAbstract() {
    return m_methods->size() > 0;
}

ElementType BehavioralFeature::getElementType() {
    return ElementType::BEHAVIORAL_FEATURE;
}

bool BehavioralFeature::isSubClassOf(ElementType eType) {
    bool ret = Feature::isSubClassOf(eType);

    if (!ret) {
        ret = Namespace::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::BEHAVIORAL_FEATURE;
    }

    return ret;
}