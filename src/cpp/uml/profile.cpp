#include "uml/profile.h"
#include "uml/stereotype.h"
#include "uml/uml-stable.h"

using namespace UML;

void Profile::init() {
    m_profileOwnedStereotypes.redefines(m_ownedStereotypes);
    m_profileOwnedStereotypes.opposite(&Stereotype::getProfileSingleton);
    m_profileOwnedStereotypes.m_signature = &Profile::getOwnedStereotypes;
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
    mountAndRelease();
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