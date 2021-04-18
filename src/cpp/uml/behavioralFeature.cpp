#include "uml/behavioralFeature.h"

using namespace UML;

void BehavioralFeature::AddMethodFunctor::operator()(Element& el) const {
    if (!dynamic_cast<Behavior&>(el).getSpecification()) {
        dynamic_cast<Behavior&>(el).setSpecification(dynamic_cast<BehavioralFeature*>(m_el));
    }
}

BehavioralFeature::BehavioralFeature() {
    m_methods = new Sequence<Behavior>;
    m_methods->addProcedures.push_back(new AddMethodFunctor(this));
}

BehavioralFeature::~BehavioralFeature() {
    delete m_methods;
}

Sequence<Behavior>& BehavioralFeature::getMethods() {
    return *m_methods;
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