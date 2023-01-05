#include "uml/stereotype.h"
#include "uml/profile.h"
#include "uml/behavior.h"
#include "uml/association.h"
#include "uml/dataType.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

TypedSet<Profile, Stereotype>& Stereotype::getProfileSingleton() {
    return m_profile;
}

Stereotype::Stereotype() : Element(ElementType::STEREOTYPE) {
    m_profile.subsets(m_owningPackage);
    m_profile.opposite(&Profile::getOwnedStereotypes);
}

Stereotype::~Stereotype() {
    
}

ProfilePtr Stereotype::getProfile() const {
    return m_profile.get();
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