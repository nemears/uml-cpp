#include "uml/uml-stable.h"

using namespace UML;

void Stereotype::OwningPackagePolicy::elementAdded(Package& el, Stereotype& me) {
    PackagePtr possibleProfile = &el;
    while (possibleProfile && !possibleProfile->is<Profile>()) {
        possibleProfile = possibleProfile->getOwningPackage();
    }
    if (possibleProfile) {
        me.m_profile.set(el.as<Profile>());
    }
}

void Stereotype::OwningPackagePolicy::elementRemoved(Package& el, Stereotype& me) {
    me.m_profile.set(0);
}

Stereotype::Stereotype(std::size_t elementType, AbstractManager& manager) :
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    PackageableElement(elementType, manager),
    Classifier(elementType, manager),
    Class(elementType, manager)
{}

Singleton<Profile, Stereotype>& Stereotype::getProfileSingleton() {
    return m_profile;
}

ProfilePtr Stereotype::getProfile() const {
    return m_profile.get();
}
