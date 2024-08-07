#include "uml/uml-stable.h"

using namespace UML;

Profile::Profile() : Element(ElementType::PROFILE) {
}

Profile::~Profile() {
    
}

bool Profile::is(ElementType eType) const {
    bool ret = Package::is(eType);

    if (!ret) {
        ret = eType == ElementType::PROFILE;
    }

    return ret;
}
