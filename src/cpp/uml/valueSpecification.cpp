#include "uml/valueSpecification.h"
#include "uml/uml-stable.h"

using namespace UML;

void ValueSpecification::reindexName(std::string oldName, std::string newName) {
    // if (getOwner()) {
    //     if (getOwner()->isSubClassOf(ElementType::SLOT)) {
    //         dynamic_cast<Slot*>(getOwner())->getValues().reindex(m_id, oldName, newName);
    //     } else if (getOwner()->isSubClassOf(ElementType::EXPRESSION)) {
    //         dynamic_cast<Expression*>(getOwner())->getOperands().reindex(m_id, oldName, newName);
    //     }
    // }

    NamedElement::reindexName(oldName, newName);
}

void ValueSpecification::referencingReleased(ID id) {
    PackageableElement::referencingReleased(id);
    TypedElement::referencingReleased(id);
}

void ValueSpecification::referenceReindexed(ID oldID, ID newID) {
    PackageableElement::referenceReindexed(oldID, newID);
    // TODO type
}

void ValueSpecification::restoreReference(Element* el) {
    PackageableElement::restoreReference(el);
    // TODO type
}

void ValueSpecification::restoreReferences() {
    TypedElement::restoreReferences();
    PackageableElement::restoreReferences();
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