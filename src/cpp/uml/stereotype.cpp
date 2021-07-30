#include "uml/stereotype.h"
#include "uml/profile.h"
#include "uml/universalFunctions.h"

using namespace UML;

Stereotype::Stereotype() {
    m_profilePtr = 0;
}

Stereotype::Stereotype(const Stereotype& stereotype) {
    m_profileID = stereotype.m_profileID;
    if (!stereotype.m_manager) {
        m_profilePtr = stereotype.m_profilePtr;
    }
}

Stereotype::~Stereotype() {

}

Profile* Stereotype::getProfile() {
    return universalGet<Profile>(m_profileID, m_profilePtr, m_manager);
}

void Stereotype::setProfile(Profile* profile) {
    if (!m_profileID.isNull()) {
        m_profileID = ID::nullID();
        m_profilePtr = 0;
    }

    if (profile) {
        m_profileID = profile->getID();
    }

    if (!m_manager) {
        m_profilePtr = profile;
    }
}

ElementType Stereotype::getElementType() const {
    return ElementType::STEREOTYPE;
}

bool Stereotype::isSubClassOf(ElementType eType) const {
    bool ret = Class::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::STEREOTYPE;
    }

    return ret;
}