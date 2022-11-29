#include "uml/behavioralFeature.h"
#include "uml/behavior.h"
#include "uml/parameter.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"

using namespace UML;

void BehavioralFeature::referenceReindexed(ID newID) {
    Feature::referenceReindexed(newID);
    Namespace::referenceReindexed(newID);
    m_methods.reindex(newID);
    m_raisedExceptions.reindex(newID);
}

void BehavioralFeature::referenceErased(ID id) {
    Namespace::referenceErased(id);
    Feature::referenceErased(id);
    m_methods.eraseElement(id);
    m_raisedExceptions.eraseElement(id);
}

void BehavioralFeature::init() {
    m_methods.opposite(&Behavior::getSpecificationSingleton);
    m_ownedParameters.subsets(m_ownedMembers);
    m_ownedParameterSets.subsets(*m_ownedElements);
}

BehavioralFeature::BehavioralFeature() : Element(ElementType::BEHAVIORAL_FEATURE) {
    init();
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

bool BehavioralFeature::isSubClassOf(ElementType eType) const {
    bool ret = Feature::isSubClassOf(eType);

    if (!ret) {
        ret = Namespace::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::BEHAVIORAL_FEATURE;
    }

    return ret;
}