#include "uml/profile.h"

using namespace UML;

Profile::Profile() {

}

Profile::Profile(const Profile& profile) {

}

Profile::~Profile() {

}

ElementType Profile::getElementType() const {
    return ElementType::PROFILE;
}

bool Profile::isSubClassOf(ElementType eType) const {
    bool ret = Package::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PROFILE;
    }

    return ret;
}