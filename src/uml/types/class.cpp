#include "uml/types/class.h"
#include "uml/types/operation.h"
#include "uml/types/property.h"
#include "uml/types/generalization.h"
#include "uml/types/package.h"
#include "uml/types/dataType.h"
#include "uml/types/behavior.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"

using namespace UML;

void Class::restoreReferences() {
    EncapsulatedClassifier::restoreReferences();
    for (auto& reception : m_ownedReceptions) {
        if (reception.getFeaturingClassifier().id() == ID::nullID() && reception.getNamespace().id() == m_id) {
            reception.m_featuringClassifier.innerAdd(*this);
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