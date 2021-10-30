#include "uml/model.h"

using namespace UML;

Model::Model() : Element(ElementType::MODEL) {}

Model::Model(const Model& rhs) : Element(rhs, ElementType::MODEL) {}

bool Model::isSubClassOf(ElementType eType) const {
    bool ret = Package::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::MODEL;
    }

    return ret;
}