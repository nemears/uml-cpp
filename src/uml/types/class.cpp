#include "uml/uml-stable.h"

using namespace UML;

void Class::restoreReferences() {
    EncapsulatedClassifier::restoreReferences();
    for (auto& reception : m_ownedReceptions) {
        if (reception.getFeaturingClassifier().id() == ID::nullID() && reception.getNamespace().id() == m_id) {
            reception.m_featuringClassifier.innerAdd(this);
        }
    }
}

Class::Class() : Element(ElementType::CLASS) {
    m_classOwnedAttrubutes.redefines(m_ownedAttributes);
    m_classOwnedAttrubutes.opposite(&Property::getClassSingleton);
    m_ownedOperations.subsets(m_features);
    m_ownedOperations.subsets(m_ownedMembers);
    m_ownedOperations.opposite(&Operation::getClassSingleton);
    m_nestedClassifiers.subsets(m_ownedMembers);
    m_ownedReceptions.subsets(m_features);
    m_ownedReceptions.subsets(m_ownedMembers);
}

Class::~Class() {

}

IndexableOrderedSet<Property, Class>& Class::getOwnedAttributes() {
    return m_classOwnedAttrubutes;
}

IndexableOrderedSet<Operation, Class>& Class::getOwnedOperations() {
    return m_ownedOperations;
}

IndexableOrderedSet<Classifier, Class>& Class::getNestedClassifiers() {
    return m_nestedClassifiers;
}

IndexableSet<Reception, Class>& Class::getOwnedReceptions() {
    return m_ownedReceptions;
}

bool Class::is(ElementType eType) const {
    bool ret = EncapsulatedClassifier::is(eType);

    if (!ret) {
        ret = BehavioredClassifier::is(eType);
    }

    if (!ret) {
        ret = eType == ElementType::CLASS;
    }
    
    return ret;
}
