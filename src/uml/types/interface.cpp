#include "uml/types/interface.h"
#include "uml/types/operation.h"
#include "uml/types/manifestation.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/deployment.h"

using namespace UML;

Interface::Interface() : Element(ElementType::INTERFACE_UML) {
    m_ownedAttributes.subsets(m_ownedMembers);
    m_ownedAttributes.subsets(m_attributes);
    m_ownedAttributes.opposite(&Property::getInterfaceSingleton);
    m_ownedOperations.subsets(m_features);
    m_ownedOperations.subsets(m_ownedMembers);
    m_ownedOperations.opposite(&Operation::getInterfaceSingleton);
    m_nestedClassifiers.subsets(m_ownedMembers);
}

Interface::~Interface() {
    
}

OrderedSet<Property, Interface>& Interface::getOwnedAttributes() {
    return m_ownedAttributes;
}

OrderedSet<Operation, Interface>& Interface::getOwnedOperations() {
    return m_ownedOperations;
}

OrderedSet<Classifier, Interface>& Interface::getNestedClassifiers() {
    return m_nestedClassifiers;
}

bool Interface::isSubClassOf(ElementType eType) const {
    bool ret = Classifier::isSubClassOf(eType);
    
    if (!ret) {
        ret = eType == ElementType::INTERFACE_UML;
    }

    return ret;
}