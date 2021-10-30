#include "uml/generalizationSet.h"
#include "uml/classifier.h"
#include "uml/generalization.h"

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

void GeneralizationSet::AddGeneralizationFunctor::operator()(Generalization& el) const {
    if (!el.getGeneralizationSets().count(m_el->getID())) {
        el.getGeneralizationSets().add(*m_el);
    }
}

void GeneralizationSet::RemoveGeneralizationFunctor::operator()(Generalization& el) const {
    if (el.getGeneralizationSets().count(m_el->getID())) {
        el.getGeneralizationSets().remove(*m_el);
    }
}

void GeneralizationSet::referenceReindexed(ID oldID, ID newID) {
    PackageableElement::referenceReindexed(oldID, newID);
    m_powerType.reindex(oldID, newID);
    m_generalizations.reindex(oldID, newID, &GeneralizationSet::getGeneralizations);
}

void GeneralizationSet::referencingReleased(ID id) {
    PackageableElement::referencingReleased(id);
    m_powerType.release(id);
    m_generalizations.elementReleased(id, &GeneralizationSet::getGeneralizations);
}

void GeneralizationSet::restoreReferences() {
    PackageableElement::restoreReferences();
    m_powerType.restoreReference();
    m_generalizations.restoreReferences();
}

void GeneralizationSet::referenceErased(ID id) {
    PackageableElement::referenceErased(id);
    m_powerType.elementErased(id);
    m_generalizations.elementErased(id);
}

GeneralizationSet::GeneralizationSet() : Element(ElementType::GENERALIZATION_SET) {
    m_powerType.m_signature = &GeneralizationSet::m_powerType;
    m_powerType.m_addProcedures.push_back(new AddPowerTypeProcedure(this));
    m_powerType.m_removeProcedures.push_back(new RemovePowerTypeProcedure(this));
    m_generalizations.addProcedures.push_back(new AddGeneralizationFunctor(this));
    m_generalizations.removeProcedures.push_back(new RemoveGeneralizationFunctor(this));
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
    m_generalizations = rhs.m_generalizations;
    m_generalizations.m_el = this;
    m_generalizations.addProcedures.clear();
    m_generalizations.removeProcedures.clear();
    m_generalizations.addProcedures.push_back(new AddGeneralizationFunctor(this));
    m_generalizations.removeProcedures.push_back(new RemoveGeneralizationFunctor(this));
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

Sequence<Generalization>& GeneralizationSet::getGeneralizations() {
    return m_generalizations;
}

bool GeneralizationSet::isSubClassOf(ElementType eType) const {
    bool ret = PackageableElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::GENERALIZATION_SET;
    }

    return ret;
}