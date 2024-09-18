#include "uml/uml-stable.h"

using namespace UML;

void Stereotype::OwningPackagePolicy::elementAdded(Package& el, Stereotype& me) {
    PackagePtr possibleProfile = &el;
    while (possibleProfile && !possibleProfile->is<Profile>()) {
        possibleProfile = possibleProfile->getOwningPackage();
    }
    if (possibleProfile) {
        me.addToReadonlySet(me.m_profile, el.as<Profile>());
    }
}

void Stereotype::OwningPackagePolicy::elementRemoved(Package& el, Stereotype& me) {
    if (me.m_profile.contains(&el)) {
        me.removeFromReadonlySet(me.m_profile, el);
    }
}

Stereotype::Stereotype(std::size_t elementType, AbstractManager& manager) :
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    PackageableElement(elementType, manager),
    Classifier(elementType, manager),
    Class(elementType, manager)
{
    m_stereotypeOwningPackage.redefines(m_owningPackage);
}

ReadOnlySingleton<Profile, Stereotype>& Stereotype::getProfileSingleton() {
    return m_profile;
}

ProfilePtr Stereotype::getProfile() const {
    return m_profile.get();
}
