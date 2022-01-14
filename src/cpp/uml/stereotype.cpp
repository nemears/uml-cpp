#include "uml/stereotype.h"
#include "uml/profile.h"
#include "uml/uml-stable.h"

using namespace UML;

void Stereotype::referencingReleased(ID id) {
    Class::referencingReleased(id);
    m_profile.release(id);
}

void Stereotype::referenceReindexed(ID oldID, ID newID) {
    Class::referenceReindexed(oldID, newID);
    m_profile.reindex(oldID, newID);
}

void Stereotype::restoreReferences() {
    Class::restoreReferences();
    // m_profile.restoreReference();
}

void Stereotype::referenceErased(ID id) {
    Class::referenceErased(id);
    m_profile.eraseElement(id);
}

Set<Profile, Stereotype>& Stereotype::getProfileSingleton() {
    return m_profile;
}

void Stereotype::init() {
    m_profile.subsets(m_owningPackage);
    m_profile.opposite(&Profile::getOwnedStereotypes);
    m_profile.m_signature = &Stereotype::getProfileSingleton;
}

void Stereotype::copy(const Stereotype& rhs) {
    m_profile = rhs.m_profile;
}

Stereotype::Stereotype() : Element(ElementType::STEREOTYPE) {
    init();
}

Stereotype::Stereotype(const Stereotype& rhs) : Element(rhs, ElementType::STEREOTYPE) {
    init();
    Element::copy(rhs);
    NamedElement::copy(rhs);
    Namespace::copy(rhs);
    ParameterableElement::copy(rhs);
    PackageableElement::copy(rhs);
    TemplateableElement::copy(rhs);
    RedefinableElement::copy(rhs);
    StructuredClassifier::copy(rhs);
    BehavioredClassifier::copy(rhs);
    Classifier::copy(rhs);
    copy(rhs);
}

Stereotype::~Stereotype() {

}

Profile* Stereotype::getProfile() {
    return m_profile.get();
}

Profile& Stereotype::getProfileRef() {
    return m_profile.getRef();
}

ID Stereotype::getProfileID() const {
    return m_profile.id();
}

bool Stereotype::hasProfile() const {
    return m_profile.has();
}

void Stereotype::setProfile(Profile* profile) {
    m_profile.set(profile);
}

void Stereotype::setProfile(Profile& profile) {
    m_profile.set(profile);
}

void Stereotype::setProfile(ID id) {
    m_profile.set(id);
}

bool Stereotype::isSubClassOf(ElementType eType) const {
    bool ret = Class::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::STEREOTYPE;
    }

    return ret;
}