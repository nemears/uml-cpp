#include "uml/types/generalizationSet.h"
#include "uml/types/profile.h"
#include "uml/types/artifact.h"
#include "uml/types/operation.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/deployment.h"
#include "uml/types/manifestation.h"
#include "uml/umlPtr.h"

using namespace UML;

void GeneralizationSet::referenceReindexed(ID newID) {
    PackageableElement::referenceReindexed(newID);
    m_powerType.reindex(newID);
    m_generalizations.reindex(newID);
}

void GeneralizationSet::referenceErased(ID id) {
    PackageableElement::referenceErased(id);
    m_powerType.eraseElement(id);
    m_generalizations.eraseElement(id);
}

TypedSet<Classifier, GeneralizationSet>& GeneralizationSet::getPowerTypeSingleton() {
    return m_powerType;
}

GeneralizationSet::GeneralizationSet() : Element(ElementType::GENERALIZATION_SET) {
    m_generalizations.opposite(&Generalization::getGeneralizationSets);
    m_powerType.opposite(&Classifier::getPowerTypeExtent);
}

GeneralizationSet::~GeneralizationSet() {
    
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