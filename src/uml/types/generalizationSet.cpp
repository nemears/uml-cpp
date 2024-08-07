#include "uml/set/singleton.h"
#include "uml/uml-stable.h"

using namespace UML;

void GeneralizationSet::referenceErased(ID id) {
    PackageableElement::referenceErased(id);
    eraseFromSet(id, m_powerType);
    eraseFromSet(id, m_generalizations);
}

Singleton<Classifier, GeneralizationSet>& GeneralizationSet::getPowerTypeSingleton() {
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

bool GeneralizationSet::is(ElementType eType) const {
    bool ret = PackageableElement::is(eType);

    if (!ret) {
        ret = eType == ElementType::GENERALIZATION_SET;
    }

    return ret;
}
