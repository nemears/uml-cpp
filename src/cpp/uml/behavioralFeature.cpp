#include "uml/behavioralFeature.h"
#include "uml/operation.h"

using namespace UML;

void BehavioralFeature::AddMethodFunctor::operator()(Element& el) const {
    if (!dynamic_cast<Behavior&>(el).getSpecification()) {
        dynamic_cast<Behavior&>(el).setSpecification(dynamic_cast<BehavioralFeature*>(m_el));
    }

    if (!dynamic_cast<BehavioralFeature*>(m_el)->getMembers().count(el.getID())) {
        dynamic_cast<BehavioralFeature*>(m_el)->getMembers().add(dynamic_cast<Behavior&>(el));
    }
}

void BehavioralFeature::AddParameterFunctor::operator()(Element& el) const {
    if (m_el->isSubClassOf(ElementType::OPERATION)) {
        if (dynamic_cast<Parameter&>(el).getOperation() != m_el) {
            dynamic_cast<Parameter&>(el).setOperation(dynamic_cast<Operation*>(m_el));
        }
    }

    if (!dynamic_cast<BehavioralFeature*>(m_el)->getMembers().count(el.getID())) {
        dynamic_cast<BehavioralFeature*>(m_el)->getMembers().add(dynamic_cast<Parameter&>(el));
    }

    // TODO behavior must have same parameters as it's specification, but they are not the same parameters
    // TODO think about whether to be different just in memory or have different ids but same everything else
    for (auto const& method : dynamic_cast<BehavioralFeature*>(m_el)->getMethods()) {
        if (!method->getParameters().count(el.getID())) {
            method->getParameters().add(dynamic_cast<Parameter&>(el));
        }
    }
}

BehavioralFeature::BehavioralFeature() {
    m_methods = new Sequence<Behavior>;
    m_methods->addProcedures.push_back(new AddMethodFunctor(this));
    m_ownedParameters = new Sequence<Parameter>;
    m_ownedParameters->addProcedures.push_back(new AddParameterFunctor(this));
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