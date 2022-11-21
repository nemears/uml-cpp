#include "uml/interface.h"
#include "uml/operation.h"
#include "uml/manifestation.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"

using namespace UML;

void Interface::init() {
    m_ownedAttributes.subsets(m_ownedMembers);
    m_ownedAttributes.subsets(m_attributes);
    m_ownedAttributes.opposite(&Property::getInterfaceSingleton);
    m_ownedOperations.subsets(m_features);
    m_ownedOperations.subsets(m_ownedMembers);
    m_ownedOperations.opposite(&Operation::getInterfaceSingleton);
    m_nestedClassifiers.subsets(m_ownedMembers);
}

Interface::Interface() : Element(ElementType::INTERFACE_UML) {
    init();
}

Interface::~Interface() {
    mountAndRelease();
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