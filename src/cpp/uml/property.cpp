#include "uml/property.h"
#include "uml/primitiveType.h"
#include "uml/classifier.h"
#include "uml/dataType.h"
#include "uml/structuredClassifier.h"
#include "uml/class.h"
#include "uml/association.h"

using namespace std;
using namespace UML;

void Property::reindexID(ID oldID, ID newID) {
    if (!m_classifierID.isNull()) {
        if (!m_classifierPtr) {
            m_classifierPtr = &m_manager->get<Classifier>(m_classifierID);
        }
        m_classifierPtr->getAttributes().reindex(oldID, newID);
    }

    if (!m_dataTypeID.isNull()) {
        if (!m_dataTypePtr) {
            m_dataTypePtr = &m_manager->get<DataType>(m_dataTypeID);
        }
        m_dataTypePtr->getOwnedAttribute().reindex(oldID, newID);
    }

    if (!m_structuredClassifierID.isNull()) {
        if (!m_structuredClassifierPtr) {
            m_structuredClassifierPtr = &m_manager->get<StructuredClassifier>(m_structuredClassifierID);
        }
        if (m_structuredClassifierPtr->getOwnedAttributes().count(oldID)) {
            m_structuredClassifierPtr->getOwnedAttributes().reindex(oldID, newID);
        }
        if (m_structuredClassifierPtr->getRole().count(oldID)) {
            m_structuredClassifierPtr->getRole().reindex(oldID, newID);
        }
        if (m_structuredClassifierPtr->getParts().count(oldID)) {
            m_structuredClassifierPtr->getParts().reindex(oldID, newID);
        }
    }

    if (!m_associationID.isNull()) {
        if (!m_associationPtr) {
            m_associationPtr = &m_manager->get<Association>(m_associationID);
        }
        if (m_associationPtr->getMemberEnds().count(oldID)) {
            m_associationPtr->getMemberEnds().reindex(oldID, newID);
        }
    }

    if (!m_owningAssociationID.isNull()) {
        if (!m_owningAssociationPtr) {
            m_owningAssociationPtr = &m_manager->get<Association>(m_owningAssociationID);
        }
        if (m_owningAssociationPtr->getOwnedEnds().count(oldID)) {
            m_owningAssociationPtr->getOwnedEnds().reindex(oldID, newID);
        }
        if (m_owningAssociationPtr->getNavigableOwnedEnds().count(oldID)) {
            m_owningAssociationPtr->getNavigableOwnedEnds().reindex(oldID, newID);
        }
    }

    Feature::reindexID(oldID, newID);
}

void Property::reindexName(string oldName, string newName) {
    if (getClassifier()) {
        getClassifier()->getAttributes().reindex(m_id, oldName, newName);
    }

    if (getDataType()) {
        getDataType()->getOwnedAttribute().reindex(m_id, oldName, newName);
    }

    if (getStructuredClassifier()) {
        if (getStructuredClassifier()->getOwnedAttributes().count(m_id)) {
            getStructuredClassifier()->getOwnedAttributes().reindex(m_id, oldName, newName);
        }
        if (getStructuredClassifier()->getRole().count(m_id)) {
            getStructuredClassifier()->getRole().reindex(m_id, oldName, newName);
        }
        if (getStructuredClassifier()->getParts().count(m_id)) {
            getStructuredClassifier()->getParts().reindex(m_id, oldName, newName);
        }
    }

    if (getAssociation()) {
        if (getAssociation()->getMemberEnds().count(m_id)) {
            getAssociation()->getMemberEnds().reindex(m_id, oldName, newName);
        }
    }

    if (getOwningAssociation()) {
        if (getOwningAssociation()->getOwnedEnds().count(m_id)) {
            getOwningAssociation()->getOwnedEnds().reindex(m_id, oldName, newName);
        }
        if (getOwningAssociation()->getNavigableOwnedEnds().count(m_id)) {
            getOwningAssociation()->getNavigableOwnedEnds().reindex(m_id, oldName, newName);
        }
    }

    Feature::reindexName(oldName, newName);
}

void Property::setManager(UmlManager* manager) {
    NamedElement::setManager(manager);
    RedefinableElement::setManager(manager);
}

Property::Property() {
    m_aggregation = AggregationKind::NONE;
    m_composite = false;
    m_defaultValuePtr = 0;
    m_classifierPtr = 0;
    m_dataTypePtr = 0;
    m_structuredClassifierPtr = 0;
    m_classPtr = 0;
    m_associationPtr = 0;
    m_owningAssociationPtr = 0;
}

// TODO remove?
Property::Property(const Property& prop) : StructuralFeature(prop), TypedElement(prop), NamedElement(prop), Element(prop) {
    m_aggregation = prop.m_aggregation;
    m_composite = prop.m_composite;
    m_defaultValueID = prop.m_defaultValueID;
    m_defaultValuePtr = prop.m_defaultValuePtr;
    m_classifierID = prop.m_classifierID;
    m_classifierPtr = prop.m_classifierPtr;
    m_dataTypeID = prop.m_dataTypeID;
    m_dataTypePtr = prop.m_dataTypePtr;
    m_classPtr = prop.m_classPtr;
    m_classID = prop.m_classID;
    m_structuredClassifierPtr = prop.m_structuredClassifierPtr;
    m_associationID = prop.m_associationID;
    m_associationPtr = prop.m_associationPtr;
    m_owningAssociationID = prop.m_owningAssociationID;
    m_owningAssociationPtr = prop.m_owningAssociationPtr;
}

AggregationKind Property::getAggregation() {
    return m_aggregation;
}

bool Property::isComposite() {
    return m_composite;
}

void Property::setComposite(bool composite) {
    if (!composite && m_composite) {
        if (!m_structuredClassifierID.isNull()) {
            if (!m_structuredClassifierPtr) {
                m_structuredClassifierPtr = &m_manager->get<StructuredClassifier>(m_structuredClassifierID);
            }
            if (m_structuredClassifierPtr->getParts().count(m_id)) {
                m_structuredClassifierPtr->getParts().remove(*this);
            }
        }
    }
    m_composite = composite;
    if (m_composite) {
        if (!m_structuredClassifierID.isNull()) {
            if (!m_structuredClassifierPtr) {
                m_structuredClassifierPtr = &m_manager->get<StructuredClassifier>(m_structuredClassifierID);
            }
            if (!m_structuredClassifierPtr->getParts().count(m_id)) {
                m_structuredClassifierPtr->getParts().add(*this);
            }
        }
    }
}

void Property::setAggregation(AggregationKind aggregation) {
    if (aggregation != AggregationKind::COMPOSITE) {
        if (isComposite()) {
            setComposite(false);
        }
    }
    m_aggregation = aggregation;
    if (m_aggregation == AggregationKind::COMPOSITE) {
        if (!isComposite()) {
            setComposite(true);
        }
    }
}

ValueSpecification* Property::getDefaultValue() {
    if (!m_defaultValueID.isNull()) {
        if (!m_defaultValuePtr) {
            m_defaultValuePtr = &m_manager->get<ValueSpecification>(m_defaultValueID);
        }
        return m_defaultValuePtr;
    }
    return 0;
}

void Property::setDefaultValue(ValueSpecification* val) {
    if (!m_defaultValueID.isNull()) {
        if (m_ownedElements->count(m_defaultValueID)) {
            if (!m_defaultValuePtr) {
                m_defaultValuePtr = &m_manager->get<ValueSpecification>(m_defaultValueID);
            }
            m_ownedElements->internalRemove(*m_defaultValuePtr);
        }
        if (m_defaultValueID != val->getID()) {
            m_defaultValueID = ID::nullID();
            m_defaultValuePtr = 0;
        }
    }
    
    if (val) {
        m_defaultValueID = val->getID();
    }

    if (!m_manager) {
        m_defaultValuePtr = val;
    }

    if (val) {
        if (!m_ownedElements->count(m_defaultValueID)) {
            m_ownedElements->internalAdd(*val);
        }
    }
}

Classifier* Property::getClassifier() {
    if (!m_classifierID.isNull()) {
        if (!m_classifierPtr) {
            m_classifierPtr = &m_manager->get<Classifier>(m_classifierID);
        }
        return m_classifierPtr;
    }
    return 0;
}

void Property::setClassifier(Classifier* classifier) {
    if (!m_classifierID.isNull()) {
        if (!m_classifierPtr) {
            m_classifierPtr = &m_manager->get<Classifier>(m_classifierID);
        }
        if (m_classifierPtr->getAttributes().count(m_id)) {
            m_classifierPtr->getAttributes().remove(*this);
        }
        m_classifierPtr = 0;
        m_classifierID = ID::nullID();
    }

    if (classifier) {
        m_classifierID = classifier->getID();
    }

    if (!m_manager) {
        m_classifierPtr = classifier;
    }

    if (classifier) {
        if (!classifier->getAttributes().count(m_id)) {
            classifier->getAttributes().add(*this);
        }
    }

    // if (m_classifier) {
    //     if (m_classifier->getAttributes().count(m_id)) {
    //         m_classifier->getAttributes().remove(*this);
    //     }
    // }
    // m_classifier = classifier;
    // if (m_classifier) {
    //     if (!m_classifier->getAttributes().count(m_id)) {
    //         m_classifier->getAttributes().add(*this);
    //     }
    // }
}

StructuredClassifier* Property::getStructuredClassifier() {
    if (!m_structuredClassifierID.isNull()) {
        if (!m_structuredClassifierPtr) {
            m_structuredClassifierPtr = &m_manager->get<StructuredClassifier>(m_structuredClassifierID);
        }
        return m_structuredClassifierPtr;
    }
    return 0;
}

void Property::setStructuredClassifier(StructuredClassifier* classifier) {
    if (!isSameOrNull(m_structuredClassifierID, classifier)) {
        if (!m_structuredClassifierPtr) {
            m_structuredClassifierPtr = &m_manager->get<StructuredClassifier>(m_structuredClassifierID);
        }
        if (m_structuredClassifierPtr->getOwnedAttributes().count(m_id)) {
            m_structuredClassifierPtr->getOwnedAttributes().remove(*this);
        }
        m_structuredClassifierPtr = 0;
        m_structuredClassifierID = ID::nullID();
    }

    if (classifier) {
        m_structuredClassifierID = classifier->getID();
    }

    if (!m_manager) {
        m_structuredClassifierPtr = classifier;
    }

    if (classifier) {
        if (!classifier->getOwnedAttributes().count(m_id)) {
            classifier->getOwnedAttributes().add(*this);
        }
    }
}

DataType* Property::getDataType() {
    if (!m_dataTypeID.isNull()) {
        if (!m_dataTypePtr) {
            m_dataTypePtr = &m_manager->get<DataType>(m_dataTypeID);
        }
        return m_dataTypePtr;
    }
    return 0;
}

void Property::setDataType(DataType* dataType) {
    if (!isSameOrNull(m_dataTypeID, dataType)) {
        if (!m_dataTypePtr) {
            m_dataTypePtr = &m_manager->get<DataType>(m_dataTypeID);
        }
        if (m_dataTypePtr->getOwnedAttribute().count(m_id)) {
            m_dataTypePtr->getOwnedAttribute().remove(*this);
        }
        m_dataTypePtr = 0;
        m_dataTypeID = ID::nullID();
    }

    if (dataType) {
        m_dataTypeID = dataType->getID();
    }

    if (!m_manager) {
        m_dataTypePtr = dataType;
    }

    if (dataType) {
        if (!dataType->getOwnedAttribute().count(m_id)) {
            dataType->getOwnedAttribute().add(*this);
        }
    }
}

Class* Property::getClass() {
    if (!m_classID.isNull()) {
        if (!m_classPtr) {
            m_classPtr = &m_manager->get<Class>(m_classID);
        }
        return m_classPtr;
    }
    return 0;
}

void Property::setClass(Class* clazz) {
    if (!isSameOrNull(m_classID, clazz)) {
        if (!m_classPtr) {
            m_classPtr = &m_manager->get<Class>(m_classID);
        }
        if (m_classPtr->getOwnedAttributes().count(m_id)) {
            m_classPtr->getOwnedAttributes().remove(*this);
        }
        m_classPtr = 0;
        m_classID = ID::nullID();
    }

    if (clazz) {
        m_classID = clazz->getID();
    }

    if (!m_manager) {
        m_classPtr = clazz;
    }

    if (clazz) {
        if (!clazz->getOwnedAttributes().count(m_id)) {
            clazz->getOwnedAttributes().add(*this);
        }
    }
}

Association* Property::getAssociation() {
    if (!m_associationID.isNull()) {
        if (!m_associationPtr) {
            m_associationPtr = &m_manager->get<Association>(m_associationID);
        }
        return m_associationPtr;
    }
    return 0;
}

void Property::setAssociation(Association* association) {
    if (!isSameOrNull(m_associationID, association)) {
        if (!m_associationPtr) {
            m_associationPtr = &m_manager->get<Association>(m_associationID);
        }
        if (m_associationPtr->getMemberEnds().count(m_id)) {
            m_associationPtr->getMemberEnds().remove(*this);
        }
        m_associationPtr = 0;
        m_associationID = ID::nullID();
    }

    if (association) {
        m_associationID = association->getID();
    }

    if (!m_manager) {
        m_associationPtr = association;
    }

    if (association) {
        if (!association->getMemberEnds().count(m_id)) {
            association->getMemberEnds().add(*this);
        }
    }
}

Association* Property::getOwningAssociation() {
    if (!m_owningAssociationID.isNull()) {
        if (!m_owningAssociationPtr) {
            m_owningAssociationPtr = &m_manager->get<Association>(m_owningAssociationID);
        }
        return m_owningAssociationPtr;
    }
    return 0;
}

void Property::setOwningAssociation(Association* association) {
    if (!isSameOrNull(m_owningAssociationID, association)) {
        if (!m_owningAssociationPtr) {
            m_owningAssociationPtr = &m_manager->get<Association>(m_owningAssociationID);
        }
        if (m_owningAssociationPtr->getNavigableOwnedEnds().count(m_id)) {
            m_owningAssociationPtr->getNavigableOwnedEnds().remove(*this);
        }
        if (!m_owningAssociationID.isNull()) {
            if (!m_owningAssociationPtr) {
                m_owningAssociationPtr = &m_manager->get<Association>(m_owningAssociationID);
            }
            if (m_owningAssociationPtr->getOwnedEnds().count(m_id)) {
                m_owningAssociationPtr->getOwnedEnds().remove(*this);
            }
        }
        m_owningAssociationPtr = 0;
        m_owningAssociationID = ID::nullID();
    }

    if (association) {
        m_owningAssociationID = association->getID();
    }

    if (!m_manager) {
        m_owningAssociationPtr = association;
    }

    if (association) {
        if (!association->getOwnedEnds().count(m_id)) {
            association->getOwnedEnds().add(*this);
        }
    }
}

void Property::setType(Type* type) {
    if (!m_associationID.isNull()) {
        if (!m_associationPtr) {
            m_associationPtr = &m_manager->get<Association>(m_associationID);
        }
        if (!m_typeID.isNull()) {
            if (!m_typePtr) {
                m_typePtr = &m_manager->get<Type>(m_typeID);
            }
            if (m_typePtr != type) {
                if (m_associationPtr->getEndType().count(m_typePtr->getID())) {
                    m_associationPtr->getEndType().remove(*m_typePtr);
                }
            }
        }
    }
    TypedElement::setType(type);
    if (!m_associationID.isNull()) {
        if (!m_associationPtr) {
            m_associationPtr = &m_manager->get<Association>(m_associationID);
        }
        if (type) {
            if (!m_associationPtr->getEndType().count(type->getID())) {
                m_associationPtr->getEndType().add(*type);
            }
        }
    }
}

ElementType Property::getElementType() const {
    return ElementType::PROPERTY;
}

bool Property::isSubClassOf(ElementType eType) const {
    bool ret = StructuralFeature::isSubClassOf(eType);

    if (!ret) {
        ret = ConnectableElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::PROPERTY;
    }

    return ret;
}