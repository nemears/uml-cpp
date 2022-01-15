#include "uml/generalization.h"
#include "uml/classifier.h"
#include "uml/generalizationSet.h"
#include "uml/uml-stable.h"

using namespace UML;

void Generalization::AddGeneralFunctor::operator()(Element& el) const {
    if (m_el.as<Generalization>().hasSpecific() && !m_el.as<Generalization>().getSpecificRef().getGenerals().contains(el.getID())) {
        m_el.as<Generalization>().getSpecificRef().getGenerals().add(el.as<Classifier>());
    }
    el.setReference(&m_el);
}

void Generalization::RemoveGeneralFunctor::operator()(Element& el) const {
    if (m_el.as<Generalization>().hasSpecific() && m_el.as<Generalization>().getSpecificRef().getGenerals().contains(el.getID())) {
        m_el.as<Generalization>().getSpecificRef().getGenerals().remove(el.as<Classifier>());
    }
    el.removeReference(m_el.getID());
}

void Generalization::referenceReindexed(ID oldID, ID newID) {
    DirectedRelationship::referenceReindexed(oldID, newID);
    m_general.reindex(oldID, newID);
    m_specific.reindex(oldID, newID);
    m_generalizationSets.reindex(oldID, newID);
}

void Generalization::referencingReleased(ID id) {
    DirectedRelationship::referencingReleased(id);
    m_general.release(id);
    m_specific.release(id);
    m_generalizationSets.release(id);
}

void Generalization::referenceErased(ID id) {
    DirectedRelationship::referenceErased(id);
    m_general.eraseElement(id);
    m_specific.eraseElement(id);
    m_generalizationSets.eraseElement(id);
}

void Generalization::restoreReference(Element* el) {
    DirectedRelationship::restoreReference(el);
    if (m_specific.id() == el->getID() && m_general.has() && !m_specific.get()->getGenerals().contains(m_general.id())) {
        m_specific.get()->getGenerals().add(m_general.getRef());
    } else if (m_general.id() == el->getID()) {
        m_general.getRef().setReference(this);
    }
}

Set<Classifier, Generalization>& Generalization::getGeneralSingleton() {
    return m_general;
}

Set<Classifier, Generalization>& Generalization::getSpecificSingleton() {
    return m_specific;
}

void Generalization::init() {
    m_general.subsets(m_targets);
    m_general.m_signature = &Generalization::getGeneralSingleton;
    m_general.m_addFunctors.insert(new AddGeneralFunctor(this));
    m_general.m_removeFunctors.insert(new RemoveGeneralFunctor(this));
    m_specific.subsets(*m_owner);
    m_specific.subsets(m_sources);
    m_specific.opposite(&Classifier::getGeneralizations);
    m_specific.m_signature = &Generalization::getSpecificSingleton;
    m_generalizationSets.opposite(&GeneralizationSet::getGeneralizations);
    m_generalizationSets.m_signature = &Generalization::getGeneralizationSets;
}

void Generalization::copy(const Generalization& rhs) {
    m_general = rhs.m_general;
    m_specific = rhs.m_general;
    m_generalizationSets = rhs.m_generalizationSets;
}

Generalization::Generalization() : Element(ElementType::GENERALIZATION) {
    init();
}

Generalization::Generalization(const Generalization& rhs) : Element(rhs, ElementType::GENERALIZATION) {
    init();
    Relationship::copy(rhs);
    DirectedRelationship::copy(rhs);
    copy(rhs);
}

Generalization::~Generalization() {
    
}

Classifier* Generalization::getGeneral() {
    return m_general.get();
}

Classifier& Generalization::getGeneralRef() {
    return m_general.getRef();
}

ID Generalization::getGeneralID() const {
    return m_general.id();
}

bool Generalization::hasGeneral() const {
    return m_general.has();
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

Classifier* Generalization::getSpecific() {
    return m_specific.get();
}

Classifier& Generalization::getSpecificRef() {
    return m_specific.getRef();
}

ID Generalization::getSpecificID() const {
    return m_specific.id();
}

bool Generalization::hasSpecific() const {
    return m_specific.has();
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