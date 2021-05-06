#include "uml/packageMerge.h"

using namespace UML;

ElementType PackageMerge::getElementType() const {
    return ElementType::PACKAGE_MERGE;
}

bool PackageMerge::isSubClassOf(ElementType eType) {
    bool ret = DirectedRelationship::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PACKAGE_MERGE;
    }

    return ret;
}