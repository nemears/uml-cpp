#include "uml/uml-stable.h"

using namespace UML;

void BehavioralFeature::referenceErased(ID id) {
    Namespace::referenceErased(id);
    Feature::referenceErased(id);
    eraseFromSet(id, m_methods);
    eraseFromSet(id, m_raisedExceptions);
}

BehavioralFeature::BehavioralFeature() : Element(ElementType::BEHAVIORAL_FEATURE) {
    m_methods.opposite(&Behavior::getSpecificationSingleton);
    m_ownedParameters.subsets(m_ownedMembers);
    m_ownedParameterSets.subsets(*m_ownedElements);
}

BehavioralFeature::~BehavioralFeature() {

}

Set<Behavior, BehavioralFeature>& BehavioralFeature::getMethods() {
    return m_methods;
}

Set<Parameter, BehavioralFeature>& BehavioralFeature::getOwnedParameters() {
    return m_ownedParameters;
}

Set<Type, BehavioralFeature>& BehavioralFeature::getRaisedExceptions() {
    return m_raisedExceptions;
}

Set<ParameterSet, BehavioralFeature>& BehavioralFeature::getOwnedParameterSets() {
    return m_ownedParameterSets;
}

bool BehavioralFeature::isAbstract() {
    return m_methods.size() > 0;
}

CallConcurrencyKind BehavioralFeature::getConcurrency() const {
    return m_concurrency;
}

void BehavioralFeature::setConcurrency(CallConcurrencyKind concurrency) {
    m_concurrency = concurrency;
}

bool BehavioralFeature::is(ElementType eType) const {
    bool ret = Feature::is(eType);

    if (!ret) {
        ret = Namespace::is(eType);
    }

    if (!ret) {
        ret = eType == ElementType::BEHAVIORAL_FEATURE;
    }

    return ret;
}
