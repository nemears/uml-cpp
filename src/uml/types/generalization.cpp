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

Singleton<Classifier, Generalization, Generalization::GeneralPolicy>& Generalization::getGeneralSingleton() {
    return m_general;
}

Singleton<Classifier, Generalization>& Generalization::getSpecificSingleton() {
    return m_specific;
}

Generalization::Generalization(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    DirectedRelationship(elementType, manager)
{
    m_general.subsets(m_targets);
    m_specific.subsets(m_owner);
    m_specific.subsets(m_sources);
    m_specific.opposite(&Classifier::getGeneralizations);
    m_generalizationSets.opposite(&GeneralizationSet::getGeneralizations);
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
