#include "uml/types/behavioralFeature.h"
#include "uml/types/behavior.h"
#include "uml/types/parameter.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"

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