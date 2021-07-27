#include "uml/dependency.h"

using namespace UML;

void Dependency::setManager(UmlManager* manager) {
    DirectedRelationship::setManager(manager);
}

void Dependency::reindexID(ID oldID, ID newID) {
    DirectedRelationship::reindexID(oldID, newID);
}

Dependency::Dependency() {

}

Dependency::Dependency(const Dependency& dependency) {

}

Dependency::~Dependency() {

}

ElementType Dependency::getElementType() const {
    return ElementType::DEPENDENCY;
}

bool Dependency::isSubClassOf(ElementType eType) const {
    bool ret = DirectedRelationship::isSubClassOf(eType);

    if (!ret) {
        ret = PackageableElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::DEPENDENCY;
    }

    return ret;
}