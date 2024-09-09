#include "uml/uml-stable.h"

using namespace UML;

Singleton<Classifier, GeneralizationSet>& GeneralizationSet::getPowerTypeSingleton() {
    return m_powerType;
}

GeneralizationSet::GeneralizationSet(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    PackageableElement(elementType, manager)
{
    m_generalizations.opposite(&Generalization::getGeneralizationSets);
    m_powerType.opposite(&Classifier::getPowerTypeExtent);
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

void GeneralizationSet::setPowerType(ClassifierPtr powerType) {
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
