#include "uml/types/profile.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/deployment.h"

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