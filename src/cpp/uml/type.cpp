#include "uml/type.h"
#include "uml/package.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/interface.h"
#include "uml/deployment.h"


using namespace UML;

Type::Type() : Element(ElementType::TYPE) {

}

Type::~Type() {

}

bool Type::isSubClassOf(ElementType eType) const {
    bool ret = PackageableElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TYPE;
    }

    return ret;
}