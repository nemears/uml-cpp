#include "uml/types/generalization.h"
#include "uml/types/behavior.h"
#include "uml/umlPtr.h"
#include "uml/types/property.h"
#include "uml/types/package.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/generalizationSet.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"

using namespace UML;

void Generalization::AddGeneralPolicy::apply(Classifier& el, Generalization& me) {
    if (me.getSpecific() && !me.getSpecific()->getGenerals().contains(el.getID())) {
        me.getSpecific()->getGenerals().add(el);
    }
}

void Generalization::RemoveGeneralPolicy::apply(Classifier& el, Generalization& me) {
    if (me.getSpecific() && me.getSpecific()->getGenerals().contains(el.getID())) {
        me.getSpecific()->getGenerals().remove(el);
    }
}

void Generalization::referenceErased(ID id) {
    DirectedRelationship::referenceErased(id);
    m_generalizationSets.eraseElement(id);
}

void Generalization::restoreReferences() {
    DirectedRelationship::restoreReferences();
    if (m_specific.get() && m_general.get() && !m_specific.get()->getGenerals().contains(m_general.get().id())) {
        m_specific.get()->getGenerals().add(*m_general.get());
    }
}

TypedSet<Classifier, Generalization>& Generalization::getGeneralSingleton() {
    return m_general;
}

TypedSet<Classifier, Generalization>& Generalization::getSpecificSingleton() {
    return m_specific;
}

Generalization::Generalization() : Element(ElementType::GENERALIZATION) {
    m_general.subsets(m_targets);
    m_specific.subsets(*m_owner);
    m_specific.subsets(m_sources);
    m_specific.opposite(&Classifier::getGeneralizations);
    m_generalizationSets.opposite(&GeneralizationSet::getGeneralizations);
}

Generalization::~Generalization() {

}

ClassifierPtr Generalization::getGeneral() const {
    return m_general.get();
}

void Generalization::setGeneral(Classifier* general) {
    m_general.set(general);
}

void Generalization::setGeneral(Classifier& general) {
    m_general.set(general);
}

void Generalization::setGeneral(ID id) {
    m_general.set(id);
}

ClassifierPtr Generalization::getSpecific() const {
    return m_specific.get();
}

void Generalization::setSpecific(Classifier* specific) {
    m_specific.set(specific);
}

void Generalization::setSpecific(Classifier& specific) {
    m_specific.set(specific);
}

void Generalization::setSpecific(ID id) {
    m_specific.set(id);
}

Set<GeneralizationSet, Generalization>& Generalization::getGeneralizationSets() {
    return m_generalizationSets;
}

bool Generalization::isSubClassOf(ElementType eType) const {
    bool ret = DirectedRelationship::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::GENERALIZATION;
    }

    return ret;
}