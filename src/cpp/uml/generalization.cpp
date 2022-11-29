#include "uml/generalization.h"
#include "uml/behavior.h"
#include "uml/umlPtr.h"
#include "uml/property.h"
#include "uml/package.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/generalizationSet.h"
#include "uml/interface.h"
#include "uml/deployment.h"

using namespace UML;

void Generalization::AddGeneralPolicy::apply(Classifier& el, Generalization& me) {
    if (me.getSpecific() && !me.getSpecific()->getGenerals().contains(el.getID())) {
        me.getSpecific()->getGenerals().add(el);
    }
    // el.setReference(&m_el);
}

void Generalization::RemoveGeneralPolicy::apply(Classifier& el, Generalization& me) {
    if (me.getSpecific() && me.getSpecific()->getGenerals().contains(el.getID())) {
        me.getSpecific()->getGenerals().remove(el);
    }
    // el.removeReference(m_el.getID());
}

void Generalization::referenceReindexed(ID newID) {
    DirectedRelationship::referenceReindexed(newID);
    m_generalizationSets.reindex(newID);
}

void Generalization::referenceErased(ID id) {
    DirectedRelationship::referenceErased(id);
    m_generalizationSets.eraseElement(id);
}

void Generalization::restoreReference(Element* el) {
    DirectedRelationship::restoreReference(el);
    // m_general.restore(el);
    if (m_specific.get().id() == el->getID() && m_general.get() && !m_specific.get()->getGenerals().contains(m_general.get().id())) {
        m_specific.get()->getGenerals().add(*m_general.get());
    } else if (m_general.get().id() == el->getID()) {
        m_general.get()->setReference(this);
    }
}

TypedSet<Classifier, Generalization>& Generalization::getGeneralSingleton() {
    return m_general;
}

TypedSet<Classifier, Generalization>& Generalization::getSpecificSingleton() {
    return m_specific;
}

void Generalization::init() {
    m_general.subsets(m_targets);
    m_specific.subsets(*m_owner);
    m_specific.subsets(m_sources);
    m_specific.opposite(&Classifier::getGeneralizations);
    m_generalizationSets.opposite(&GeneralizationSet::getGeneralizations);
}

Generalization::Generalization() : Element(ElementType::GENERALIZATION) {
    init();
}

Generalization::~Generalization() {
    mountAndRelease();
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