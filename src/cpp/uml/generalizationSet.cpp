#include "uml/generalizationSet.h"
#include "uml/profile.h"
#include "uml/artifact.h"
#include "uml/operation.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"
#include "uml/manifestation.h"
#include "uml/umlPtr.h"

using namespace UML;

void GeneralizationSet::referenceReindexed(ID oldID, ID newID) {
    PackageableElement::referenceReindexed(oldID, newID);
    m_powerType.reindex(oldID, newID);
    m_generalizations.reindex(oldID, newID);
}

void GeneralizationSet::reindexName(std::string oldName, std::string newName) {
    PackageableElement::reindexName(oldName, newName);
    m_powerType.reindexName(oldName, newName);
    m_generalizations.reindexName(oldName, newName);
}

void GeneralizationSet::referencingReleased(ID id) {
    PackageableElement::referencingReleased(id);
    m_powerType.release(id);
    m_generalizations.release(id);
}

void GeneralizationSet::referenceErased(ID id) {
    PackageableElement::referenceErased(id);
    m_powerType.eraseElement(id);
    m_generalizations.eraseElement(id);
}

Set<Classifier, GeneralizationSet>& GeneralizationSet::getPowerTypeSingleton() {
    return m_powerType;
}

void GeneralizationSet::init() {
    m_generalizations.opposite(&Generalization::getGeneralizationSets);
    m_generalizations.m_signature = &GeneralizationSet::getGeneralizations;
    m_powerType.opposite(&Classifier::getPowerTypeExtent);
    m_powerType.m_signature = &GeneralizationSet::getPowerTypeSingleton;
}

GeneralizationSet::GeneralizationSet() : Element(ElementType::GENERALIZATION_SET) {
    init();
}

GeneralizationSet::~GeneralizationSet() {
    mountAndRelease();
}

bool GeneralizationSet::isCovering() const {
    return m_covering;
}

bool GeneralizationSet::isDisjoint() const {
    return m_disjoint;
}

void GeneralizationSet::setCovering(bool covering) {
    m_covering = covering;
}

void GeneralizationSet::setDisjoint(bool disjoint) {
    m_disjoint = disjoint;
}

ClassifierPtr GeneralizationSet::getPowerType() const {
    return m_powerType.get();
}

void GeneralizationSet::setPowerType(Classifier* powerType) {
    m_powerType.set(powerType);
}

void GeneralizationSet::setPowerType(Classifier& powerType) {
    m_powerType.set(powerType);
}

void GeneralizationSet::setPowerType(ID id) {
    m_powerType.set(id);
}

Set<Generalization, GeneralizationSet>& GeneralizationSet::getGeneralizations() {
    return m_generalizations;
}

bool GeneralizationSet::isSubClassOf(ElementType eType) const {
    bool ret = PackageableElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::GENERALIZATION_SET;
    }

    return ret;
}