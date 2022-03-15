#include "uml/behavioralFeature.h"
#include "uml/behavior.h"
#include "uml/parameter.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"

using namespace UML;

void BehavioralFeature::referencingReleased(ID id) {
    Namespace::referencingReleased(id);
    Feature::referencingReleased(id);
    m_methods.release(id);
}

void BehavioralFeature::referenceReindexed(ID oldID, ID newID) {
    Feature::referenceReindexed(oldID, newID);
    Namespace::referenceReindexed(oldID, newID);
    m_methods.reindex(oldID, newID);
}

void BehavioralFeature::reindexName(ID id, std::string newName) {
    Feature::reindexName(id, newName);
    Namespace::reindexName(id, newName);
    m_methods.reindexName(id, newName);
}

void BehavioralFeature::referenceErased(ID id) {
    Namespace::referenceErased(id);
    Feature::referenceErased(id);
    m_methods.eraseElement(id);
}

void BehavioralFeature::init() {
    m_methods.opposite(&Behavior::getSpecificationSingleton);
    m_methods.m_signature = &BehavioralFeature::getMethods;
    m_ownedParameters.subsets(m_ownedMembers);
    m_ownedParameters.m_signature = &BehavioralFeature::getOwnedParameters;
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