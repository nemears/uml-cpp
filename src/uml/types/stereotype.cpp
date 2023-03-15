#include "uml/types/stereotype.h"
#include "uml/types/profile.h"
#include "uml/types/behavior.h"
#include "uml/types/association.h"
#include "uml/types/dataType.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
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