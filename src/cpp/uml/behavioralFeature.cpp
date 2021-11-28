#include "uml/behavioralFeature.h"
//#include "uml/operation.h"
#include "uml/behavior.h"
#include "uml/parameter.h"
#include "uml/uml-stable.h"

using namespace UML;

void BehavioralFeature::referencingReleased(ID id) {
    Namespace::referencingReleased(id);
    Feature::referencingReleased(id);
    m_methods.release(id);
    m_ownedParameters.release(id);
}

void BehavioralFeature::referenceReindexed(ID oldID, ID newID) {
    Feature::referenceReindexed(oldID, newID);
    Namespace::referenceReindexed(oldID, newID);
    m_methods.reindex(oldID, newID);
    m_ownedParameters.reindex(oldID, newID);
}

void BehavioralFeature::restoreReferences() {
    Namespace::restoreReferences();
    Feature::restoreReferences();
    // m_methods.restoreReferences();
    // m_ownedParameters.restoreReferences();
}

void BehavioralFeature::referenceErased(ID id) {
    Namespace::referenceErased(id);
    Feature::referenceErased(id);
    m_methods.eraseElement(id);
    m_ownedParameters.eraseElement(id);
}

void BehavioralFeature::init() {
    // m_methods.opposit(&Behavior::getSpecifications);
    m_methods.m_signature = &BehavioralFeature::getMethods;
    m_ownedParameters.subsets(m_ownedMembers);
    m_ownedParameters.m_signature = &BehavioralFeature::getOwnedParameters;
}

void BehavioralFeature::copy(const BehavioralFeature& rhs) {
    m_methods = rhs.m_methods;
    m_ownedParameters = rhs.m_ownedParameters;
    m_returnSpecified = rhs.m_returnSpecified;
}

BehavioralFeature::BehavioralFeature() : Element(ElementType::BEHAVIORAL_FEATURE) {
    init();
}

BehavioralFeature::BehavioralFeature(const BehavioralFeature& el) : Element(ElementType::BEHAVIORAL_FEATURE) {
    // abstract
}

BehavioralFeature::~BehavioralFeature() {

}

Set<Behavior, BehavioralFeature>& BehavioralFeature::getMethods() {
    return m_methods;
}

Set<Parameter, BehavioralFeature>& BehavioralFeature::getOwnedParameters() {
    return m_ownedParameters;
}

bool BehavioralFeature::isAbstract() {
    return m_methods.size() > 0;
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