#include "uml/uml-stable.h"

namespace UML {

ReadOnlySingleton<Element, Element>& Element::getOwnerSingleton() {
    return m_owner;
}

Element::Element(std::size_t elementType, AbstractManager& manager) : AbstractElement(elementType, manager) {
    m_owner.opposite(&Element::getOwnedElements);
    m_owner.setComposition(CompositionType::ANTI_COMPOSITE);
    m_ownedElements.opposite(&Element::getOwnerSingleton);
    m_ownedElements.setComposition(CompositionType::COMPOSITE);
    m_ownedComments.subsets(m_ownedElements);
}

ElementPtr Element::getOwner() const {
    return m_owner.get();
}

ReadOnlySet<Element, Element>& Element::getOwnedElements() {
    return m_ownedElements;
}

Set<InstanceSpecification, Element>& Element::getAppliedStereotypes() {
    return m_appliedStereotypes;
}

Set<Comment, Element>& Element::getOwnedComments() {
    return m_ownedComments;
}

void Element::setOwner(ID id) {
    m_owner.nonOppositeAdd(m_manager.createPtr(id));
}

}
