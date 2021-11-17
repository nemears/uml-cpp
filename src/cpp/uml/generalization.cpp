#include "uml/generalization.h"
#include "uml/classifier.h"
// #include "uml/generalizationSet.h"

using namespace UML;

void Generalization::referenceReindexed(ID oldID, ID newID) {
    DirectedRelationship::referenceReindexed(oldID, newID);
    m_general.reindex(oldID, newID);
    m_specific.reindex(oldID, newID);
    // m_generalizationSets.reindex(oldID, newID, &Generalization::getGeneralizationSets);
}

void Generalization::referencingReleased(ID id) {
    DirectedRelationship::referencingReleased(id);
    m_general.release(id);
    m_specific.release(id);
    // m_generalizationSets.elementReleased(id, &Generalization::getGeneralizationSets);
}

void Generalization::restoreReferences() {
    DirectedRelationship::restoreReferences();
    // m_general.restoreReference();
    // m_specific.restoreReference();
    // m_generalizationSets.restoreReferences();
}

void Generalization::referenceErased(ID id) {
    DirectedRelationship::referenceErased(id);
    m_general.eraseElement(id);
    m_specific.eraseElement(id);
    // m_generalizationSets.elementErased(id);
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
    m_specific.subsets(*m_owner);
    m_specific.subsets(m_sources);
    m_specific.opposite(&Classifier::getGeneralizations);
    m_specific.m_signature = &Generalization::getSpecificSingleton;
}

void Generalization::copy(const Generalization& rhs) {
    m_general = rhs.m_general;
    m_general.m_el = this;
    m_specific = rhs.m_general;
    m_specific.m_el = this;
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

// Sequence<GeneralizationSet>& Generalization::getGeneralizationSets() {
//     return m_generalizationSets;
// }

bool Generalization::isSubClassOf(ElementType eType) const {
    bool ret = DirectedRelationship::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::GENERALIZATION;
    }

    return ret;
}