#include "uml/managers/abstractManager.h"
#include "uml/uml-stable.h"
#include <regex>

namespace UML {

bool isValidID(std::string strn) {
    return std::regex_match (strn, std::regex("(?:[A-Za-z0-9_&]{28})"));
}

void Element::restoreReferences() {
    // nothing
}

void Element::referenceErased(ID id) {
    eraseFromSet(id, *m_owner);
    eraseFromSet(id, *m_ownedElements);
    eraseFromSet(id, *m_ownedComments);
    eraseFromSet(id, *m_appliedStereotypes);
}

ReadOnlySingleton<Element, Element>& Element::getOwnerSingleton() {
    return *m_owner;
}

void Element::eraseFromSet(ID id, AbstractSet& set) {
    set.weakRemove(m_manager->createPtr(id));
}

Element::Element(std::size_t elementType, AbstractManager& manager) : BaseElement<UmlTypes>(elementType, manager) {
    m_node = 0;
    m_id = ID::randomID();
    m_owner.opposite(&Element::getOwnedElements);
    m_ownedElements.opposite(&Element::getOwnerSingleton);
    m_ownedComments.subsets(m_ownedElements);
}

void Element::setID(std::string id) {
    if (UML::isValidID(id)) {
        setID(ID::fromString(id));
    } else {
        throw InvalidID_Exception();
    }
}



ElementPtr Element::getOwner() const {
    return m_owner->get();
}

void Element::setOwner(ElementPtr owner) {
    m_owner->innerAdd(owner);
}

ReadOnlySet<Element, Element>& Element::getOwnedElements() {
    return *m_ownedElements;
}

Set<InstanceSpecification, Element>& Element::getAppliedStereotypes() {
    return *m_appliedStereotypes;
}

Set<Comment, Element>& Element::getOwnedComments() {
    return *m_ownedComments;
}

void Element::setOwner(ID id) {
    m_owner->nonOppositeAdd(m_manager->createPtr(id));
}

}
