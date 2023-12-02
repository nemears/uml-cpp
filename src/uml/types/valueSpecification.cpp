#include "uml/types/valueSpecification.h"
#include "uml/types/package.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"


using namespace UML;

void ValueSpecification::referenceErased(ID id) {
    TypedElement::referenceErased(id);
    PackageableElement::referenceErased(id);
}

ValueSpecification::ValueSpecification() : Element(ElementType::VALUE_SPECIFICATION) {

}

bool ValueSpecification::isSubClassOf(ElementType eType) const {
    bool ret = TypedElement::isSubClassOf(eType);

    if (!ret) {
        ret = PackageableElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::VALUE_SPECIFICATION;
    }

    return ret;
}