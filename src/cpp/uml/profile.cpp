#include "uml/profile.h"
#include "uml/stereotype.h"
#include "uml/uml-stable.h"

using namespace UML;

void Profile::referencingReleased(ID id) {
    Package::referencingReleased(id);
    m_profileOwnedStereotypes.release(id);
}

void Profile::referenceReindexed(ID oldID, ID newID) {
    Package::referenceReindexed(oldID, newID);
    m_profileOwnedStereotypes.reindex(oldID, newID);
}

void Profile::restoreReferences() {
    Package::restoreReferences();
    //
}

void Profile::referenceErased(ID id) {
    Package::referenceErased(id);
    m_profileOwnedStereotypes.eraseElement(id);
}

void Profile::init() {
    m_profileOwnedStereotypes.redefines(m_ownedStereotypes);
    m_profileOwnedStereotypes.opposite(&Stereotype::getProfileSingleton);
}

void Profile::copy(const Profile& rhs) {
    m_profileOwnedStereotypes = rhs.m_profileOwnedStereotypes;
}

Profile::Profile() : Element(ElementType::PROFILE) {
    init();
}

Profile::Profile(const Profile& rhs) : Element(rhs, ElementType::PROFILE) {
    init();
    Element::copy(rhs);
    NamedElement::copy(rhs);
    Namespace::copy(rhs);
    ParameterableElement::copy(rhs);
    PackageableElement::copy(rhs);
    TemplateableElement::copy(rhs);
    Package::copy(rhs);
    copy(rhs);
}

Profile::~Profile() {

}

Set<Stereotype, Profile>& Profile::getOwnedStereotypes() {
    return m_profileOwnedStereotypes;
}

bool Profile::isSubClassOf(ElementType eType) const {
    bool ret = Package::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PROFILE;
    }

    return ret;
}