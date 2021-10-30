#include "uml/profile.h"
#include "uml/stereotype.h"

using namespace UML;

void Profile::ProfileAddOwnedStereotypeFunctor::operator()(Stereotype& el) const {
    if (el.getProfileID() != m_el->getID()) {
        el.setProfile(m_el);
    }
}

void Profile::ProfileRemoveOwnedStereotypeFunctor::operator()(Stereotype& el) const {
    if (el.getProfileID() == m_el->getID()) {
        el.setProfile(0);
    }
}

Profile::Profile() : Element(ElementType::PROFILE) {
    m_ownedStereotypes.addProcedures.push_back(new ProfileAddOwnedStereotypeFunctor(this));
    m_ownedStereotypes.removeProcedures.push_back(new ProfileRemoveOwnedStereotypeFunctor(this));
}

Profile::Profile(const Profile& profile) : Element(profile, ElementType::PROFILE) {
    m_ownedStereotypes.addProcedures.push_back(new ProfileAddOwnedStereotypeFunctor(this));
    m_ownedStereotypes.removeProcedures.push_back(new ProfileRemoveOwnedStereotypeFunctor(this));
}

Profile::~Profile() {

}

bool Profile::isSubClassOf(ElementType eType) const {
    bool ret = Package::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PROFILE;
    }

    return ret;
}