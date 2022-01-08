#include "uml/generalizationSet.h"
#include "uml/classifier.h"
#include "uml/generalization.h"
#include "uml/uml-stable.h"

using namespace UML;

void GeneralizationSet::referenceReindexed(ID oldID, ID newID) {
    PackageableElement::referenceReindexed(oldID, newID);
    m_powerType.reindex(oldID, newID);
    m_generalizations.reindex(oldID, newID);
}

void GeneralizationSet::referencingReleased(ID id) {
    PackageableElement::referencingReleased(id);
    m_powerType.release(id);
    m_generalizations.release(id);
}

void GeneralizationSet::restoreReferences() {
    PackageableElement::restoreReferences();
    // m_powerType.restoreReference();
    // m_generalizations.restoreReferences();
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

void GeneralizationSet::copy(const GeneralizationSet& rhs) {
    m_generalizations = rhs.m_generalizations;
    m_powerType = rhs.m_powerType;
    m_covering = rhs.m_covering;
    m_disjoint = rhs.m_disjoint;
}

GeneralizationSet::GeneralizationSet() : Element(ElementType::GENERALIZATION_SET) {
    init();
}
GeneralizationSet::GeneralizationSet(const GeneralizationSet& rhs) : Element(rhs, ElementType::GENERALIZATION_SET) {
    init();
    Element::copy(rhs);
    NamedElement::copy(rhs);
    PackageableElement::copy(rhs);
    copy(rhs);
}

bool GeneralizationSet::isCovering() const {
    return m_covering;
}

bool GeneralizationSet::isDisjoint() const {
    return m_disjoint;
}

void GeneralizationSet::setCovering(bool covering) {
    m_covering = covering;
    updateCopiesScalar(covering, &GeneralizationSet::m_covering);
}

void GeneralizationSet::setDisjoint(bool disjoint) {
    m_disjoint = disjoint;
    updateCopiesScalar(disjoint, &GeneralizationSet::m_disjoint);
}

Classifier* GeneralizationSet::getPowerType() {
    return m_powerType.get();
}

Classifier& GeneralizationSet::getPowerTypeRef() {
    return m_powerType.getRef();
}

ID GeneralizationSet::getPowerTypeID() const {
    return m_powerType.id();
}

bool GeneralizationSet::hasPowerType() const {
    return m_powerType.has();
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