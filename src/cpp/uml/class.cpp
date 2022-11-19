#include "uml/class.h"
#include "uml/operation.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/package.h"
#include "uml/dataType.h"
#include "uml/behavior.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"

using namespace UML;

// Set<Property, Class>& Class::getOwnedAttributesSet() {
//     return m_classOwnedAttrubutes;
// }

// Set<Operation, Class>& Class::getOwnedOperationsSet() {
//     return m_ownedOperations;
// }

// Set<Classifier, Class>& Class::getNestedClassifiersSet() {
//     return m_nestedClassifiers;
// }

void Class::init() {
    m_classOwnedAttrubutes.redefines(m_ownedAttributes);
    m_classOwnedAttrubutes.opposite(&Property::getClassSingleton);
    m_ownedOperations.subsets(m_features);
    m_ownedOperations.subsets(m_ownedMembers);
    m_ownedOperations.opposite(&Operation::getClassSingleton);
    m_nestedClassifiers.subsets(m_ownedMembers);
    m_ownedReceptions.subsets(m_features);
    m_ownedReceptions.subsets(m_ownedMembers);
}

Class::Class() : Element(ElementType::CLASS) {
    init();
}

Class::~Class() {
    mountAndRelease();
}

OrderedSet<Property, Class>& Class::getOwnedAttributes() {
    return m_classOwnedAttrubutes;
}

OrderedSet<Operation, Class>& Class::getOwnedOperations() {
    return m_ownedOperations;
}

OrderedSet<Classifier, Class>& Class::getNestedClassifiers() {
    return m_nestedClassifiers;
}

Set<Reception, Class>& Class::getOwnedReceptions() {
    return m_ownedReceptions;
}

bool Class::isSubClassOf(ElementType eType) const {
    bool ret = EncapsulatedClassifier::isSubClassOf(eType);

    if (!ret) {
        ret = BehavioredClassifier::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::CLASS;
    }
    
    return ret;
}