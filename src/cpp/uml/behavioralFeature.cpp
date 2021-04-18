#include "uml/behavioralFeature.h"

using namespace UML;

void BehavioralFeature::AddMethodFunctor::operator()(Element& el) const {
    if (!dynamic_cast<Behavior&>(el).getSpecification()) {
        dynamic_cast<Behavior&>(el).setSpecification(dynamic_cast<BehavioralFeature*>(m_el));
    }

    if (!dynamic_cast<BehavioralFeature*>(m_el)->getMembers().count(el.getID())) {
        dynamic_cast<BehavioralFeature*>(m_el)->getMembers().add(dynamic_cast<Behavior&>(el));
    }
}

BehavioralFeature::BehavioralFeature() {
    m_methods = new Sequence<Behavior>;
    m_methods->addProcedures.push_back(new AddMethodFunctor(this));
    m_ownedParameters = new Sequence<Parameter>;
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