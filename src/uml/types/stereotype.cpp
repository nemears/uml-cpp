#include "uml/types/stereotype.h"
#include "uml/types/profile.h"
#include "uml/types/behavior.h"
#include "uml/types/association.h"
#include "uml/types/dataType.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

void Stereotype::SetOwningPackagePolicy::apply(Package& el, Stereotype& me) {
    PackagePtr possibleProfile = &el;
    while (possibleProfile && !possibleProfile->isSubClassOf(ElementType::PROFILE)) {
        possibleProfile = possibleProfile->getOwningPackage();
    }
    if (possibleProfile) {
        me.m_profile.set(el.as<Profile>());
    }
}

void Stereotype::RemoveOwningPackagePolicy::apply(Package& el, Stereotype& me) {
    me.m_profile.set(0);
}

TypedSet<Profile, Stereotype>& Stereotype::getProfileSingleton() {
    return m_profile;
}

void Stereotype::restoreReferences() {
    PackagePtr possibleProfile = getOwningPackage();
    while (possibleProfile && !possibleProfile->isSubClassOf(ElementType::PROFILE)) {
        possibleProfile = possibleProfile->getOwningPackage();
    }
    if (possibleProfile) {
        m_profile.set(possibleProfile);
    }
}

Stereotype::Stereotype() : Element(ElementType::STEREOTYPE) {
    m_stereotypeOwningPackage.redefines(m_owningPackage);
    m_profile.subsets(m_stereotypeOwningPackage);
    m_profile.opposite(&Profile::getOwnedStereotypes);
}

Stereotype::~Stereotype() {
    
}

ProfilePtr Stereotype::getProfile() const {
    return m_profile.get();
}

bool Stereotype::isSubClassOf(ElementType eType) const {
    bool ret = Class::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::STEREOTYPE;
    }

    return ret;
}