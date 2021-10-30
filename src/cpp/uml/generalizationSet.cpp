#include "uml/generalizationSet.h"
#include "uml/classifier.h"

using namespace UML;

void GeneralizationSet::RemovePowerTypeProcedure::operator()(Classifier* el) const {
    if (el->getPowerTypeExtent().count(m_me->getID())) {
        el->getPowerTypeExtent().remove(*m_me);
    }
}

void GeneralizationSet::AddPowerTypeProcedure::operator()(Classifier* el) const {
    if (!el->getPowerTypeExtent().count(m_me->getID())) {
        el->getPowerTypeExtent().add(*m_me);
    }
}

GeneralizationSet::GeneralizationSet() : Element(ElementType::GENERALIZATION_SET) {
    m_powerType.m_signature = &GeneralizationSet::m_powerType;
    m_powerType.m_addProcedures.push_back(new AddPowerTypeProcedure(this));
    m_powerType.m_removeProcedures.push_back(new RemovePowerTypeProcedure(this));
}

GeneralizationSet::GeneralizationSet(const GeneralizationSet& rhs) :
PackageableElement(rhs),
ParameterableElement(rhs),
NamedElement(rhs),
Element(rhs, ElementType::GENERALIZATION_SET) {
    m_powerType = rhs.m_powerType;
    m_powerType.m_me = this;
    m_powerType.m_addProcedures.clear();
    m_powerType.m_removeProcedures.clear();
    m_powerType.m_addProcedures.push_back(new AddPowerTypeProcedure(this));
    m_powerType.m_removeProcedures.push_back(new RemovePowerTypeProcedure(this));
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

bool GeneralizationSet::isSubClassOf(ElementType eType) const {
    bool ret = PackageableElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::GENERALIZATION_SET;
    }

    return ret;
}