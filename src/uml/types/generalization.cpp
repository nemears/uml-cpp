#include "uml/types/generalization.h"
#include "uml/uml-stable.h"

using namespace UML;

void Generalization::GeneralPolicy::elementAdded(Classifier& el, Generalization& me) {
    if (me.getSpecific() && !me.getSpecific()->getGenerals().contains(el.getID())) {
        me.getSpecific()->getGenerals().add(el);
    }
}

void Generalization::GeneralPolicy::elementRemoved(Classifier& el, Generalization& me) {
    if (me.getSpecific() && me.getSpecific()->getGenerals().contains(el.getID())) {
        me.getSpecific()->getGenerals().remove(el);
    }
}

void Generalization::referenceErased(ID id) {
    DirectedRelationship::referenceErased(id);
    eraseFromSet(id, m_generalizationSets);
}

void Generalization::restoreReferences() {
    DirectedRelationship::restoreReferences();
    if (m_specific.get() && m_general.get() && !m_specific.get()->getGenerals().contains(m_general.get().id())) {
        m_specific.get()->getGenerals().add(*m_general.get());
    }
}

Singleton<Classifier, Generalization, Generalization::GeneralPolicy>& Generalization::getGeneralSingleton() {
    return m_general;
}

Singleton<Classifier, Generalization>& Generalization::getSpecificSingleton() {
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

void Generalization::setGeneral(ClassifierPtr general) {
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

void Generalization::setSpecific(ClassifierPtr specific) {
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

bool Generalization::is(ElementType eType) const {
    bool ret = DirectedRelationship::is(eType);

    if (!ret) {
        ret = eType == ElementType::GENERALIZATION;
    }

    return ret;
}
