#include "uml/profile.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"

using namespace UML;

Profile::Profile() : Element(ElementType::PROFILE) {
    m_profileOwnedStereotypes.redefines(m_ownedStereotypes);
    m_profileOwnedStereotypes.opposite(&Stereotype::getProfileSingleton);
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