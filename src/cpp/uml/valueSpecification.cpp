#include "uml/valueSpecification.h"
#include "uml/uml-stable.h"

using namespace UML;

void ValueSpecification::referencingReleased(ID id) {
    PackageableElement::referencingReleased(id);
    TypedElement::referencingReleased(id);
}

void ValueSpecification::referenceReindexed(ID oldID, ID newID) {
    PackageableElement::referenceReindexed(oldID, newID);
    // TODO type
}

void ValueSpecification::reindexName(std::string oldName, std::string newName) {
    PackageableElement::reindexName(oldName, newName);
    TypedElement::reindexName(oldName, newName);
}

void ValueSpecification::restoreReference(Element* el) {
    PackageableElement::restoreReference(el);
    // TODO type
}

void ValueSpecification::referenceErased(ID id) {
    TypedElement::referenceErased(id);
    PackageableElement::referenceErased(id);
}

ValueSpecification::ValueSpecification() : Element(ElementType::VALUE_SPECIFICATION) {

}

ValueSpecification::ValueSpecification(const ValueSpecification& rhs) : Element(ElementType::VALUE_SPECIFICATION) {
    // abstract
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