#include "uml/uml-stable.h"

using namespace UML;

void Stereotype::OwningPackagePolicy::elementAdded(Package& el, Stereotype& me) {
    PackagePtr possibleProfile = &el;
    while (possibleProfile && !possibleProfile->is(ElementType::PROFILE)) {
        possibleProfile = possibleProfile->getOwningPackage();
    }
    if (possibleProfile) {
        me.m_profile.set(el.as<Profile>());
    }
}

void Stereotype::OwningPackagePolicy::elementRemoved(Package& el, Stereotype& me) {
    me.m_profile.set(0);
}

Singleton<Profile, Stereotype>& Stereotype::getProfileSingleton() {
    return m_profile;
}

void Stereotype::restoreReferences() {
    PackagePtr possibleProfile = getOwningPackage();
    while (possibleProfile && !possibleProfile->is(ElementType::PROFILE)) {
        possibleProfile = possibleProfile->getOwningPackage();
    }
    if (possibleProfile) {
        m_profile.set(possibleProfile);
    }
}

Stereotype::Stereotype() : Element(ElementType::STEREOTYPE) {
    m_stereotypeOwningPackage.redefines(m_owningPackage);
}

Stereotype::~Stereotype() {
    
}

ProfilePtr Stereotype::getProfile() const {
    return m_profile.get();
}

bool Stereotype::is(ElementType eType) const {
    bool ret = Class::is(eType);

    if (!ret) {
        ret = eType == ElementType::STEREOTYPE;
    }

    return ret;
}
