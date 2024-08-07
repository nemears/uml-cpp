#include "uml/uml-stable.h"

using namespace UML;

void ValueSpecification::referenceErased(ID id) {
    TypedElement::referenceErased(id);
    PackageableElement::referenceErased(id);
}

ValueSpecification::ValueSpecification() : Element(ElementType::VALUE_SPECIFICATION) {

}

bool ValueSpecification::is(ElementType eType) const {
    bool ret = TypedElement::is(eType);

    if (!ret) {
        ret = PackageableElement::is(eType);
    }

    if (!ret) {
        ret = eType == ElementType::VALUE_SPECIFICATION;
    }

    return ret;
}
