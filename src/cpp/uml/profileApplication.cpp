#include "uml/profileApplication.h"

using namespace UML;

ProfileApplication::ProfileApplication() {

}

ProfileApplication::ProfileApplication(const ProfileApplication& profileApplication) {

}

ProfileApplication::~ProfileApplication() {

}

ElementType ProfileApplication::getElementType() const {
    return ElementType::PROFILE_APPLICATION;
}

bool ProfileApplication::isSubClassOf(ElementType eType) const {
    bool ret = DirectedRelationship::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PROFILE_APPLICATION;
    }

    return ret;
}