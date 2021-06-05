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

    if (m_association) {
        if (m_association->getMemberEnds().count(oldID)) {
            m_association->getMemberEnds().reindex(oldID, newID);
        }
    }

    if (m_owningAssociation) {
        if (m_owningAssociation->getOwnedEnds().count(oldID)) {
            m_owningAssociation->getOwnedEnds().reindex(oldID, newID);
        }
        if (m_owningAssociation->getNavigableOwnedEnds().count(oldID)) {
            m_owningAssociation->getNavigableOwnedEnds().reindex(oldID, newID);
        }
    }

    Feature::reindexID(oldID, newID);
}

// void Property::reindexName(string oldName, string newName) {
//     if (m_classifier) {
//         m_classifier->getAttributes().reindex(m_id, oldName, newName);
//     }

//     if (m_dataType) {
//         m_dataType->getOwnedAttribute().reindex(m_id, oldName, newName);
//     }

//     if (m_structuredClassifier) {
//         if (m_structuredClassifier->getOwnedAttributes().count(m_id)) {
//             m_structuredClassifier->getOwnedAttributes().reindex(m_id, oldName, newName);
//         }
//         if (m_structuredClassifier->getRole().count(m_id)) {
//             m_structuredClassifier->getRole().reindex(m_id, oldName, newName);
//         }
//         if (m_structuredClassifier->getParts().count(m_id)) {
//             m_structuredClassifier->getParts().reindex(m_id, oldName, newName);
//         }
//     }

//     if (m_association) {
//         if (m_association->getMemberEnds().count(m_id)) {
//             m_association->getMemberEnds().reindex(m_id, oldName, newName);
//         }
//     }

//     if (m_owningAssociation) {
//         if (m_owningAssociation->getOwnedEnds().count(m_id)) {
//             m_owningAssociation->getOwnedEnds().reindex(m_id, oldName, newName);
//         }
//         if (m_owningAssociation->getNavigableOwnedEnds().count(m_id)) {
//             m_owningAssociation->getNavigableOwnedEnds().reindex(m_id, oldName, newName);
//         }
//     }

//     Feature::reindexName(oldName, newName);
// }

Property::Property() {
    m_aggregation = AggregationKind::NONE;
    m_composite = false;
    m_defaultValuePtr = 0;
    m_classifierPtr = 0;
    m_dataTypePtr = 0;
    m_structuredClassifierPtr = 0;
    m_class = 0;
    m_association = 0;
    m_owningAssociation = 0;
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
    m_class = prop.m_class;
    m_structuredClassifierPtr = prop.m_structuredClassifierPtr;
    m_association = prop.m_association;
    m_owningAssociation = prop.m_owningAssociation;
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
    if (!m_structuredClassifierID.isNull()) {
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

    // if (m_structuredClassifier) {
    //     if (m_structuredClassifier != classifier) {
    //         if (m_structuredClassifier->getOwnedAttributes().count(m_id)) {
    //             m_structuredClassifier->getOwnedAttributes().remove(*this);
    //         }
    //     }
    // }
    // m_structuredClassifier = classifier;
    // if (m_structuredClassifier) {
    //     if (!m_structuredClassifier->getOwnedAttributes().count(m_id)) {
    //         m_structuredClassifier->getOwnedAttributes().add(*this);
    //     }
    // }
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
    if (!m_dataTypeID.isNull()) {
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

    // if (m_dataType) {
    //     if (m_dataType->getOwnedAttribute().count(m_id)) {
    //         m_dataType->getOwnedAttribute().remove(*this);
    //     }
    // }
    // m_dataType = dataType;
    // if (m_dataType) {
    //     if (!m_dataType->getOwnedAttribute().count(m_id)) {
    //         m_dataType->getOwnedAttribute().add(*this);
    //     }
    // }
}

Class* Property::getClass() {
    return m_class;
}

void Property::setClass(Class* clazz) {
    if (m_class) {
        if (m_class != clazz) {
            if (m_class->getOwnedAttributes().count(m_id)) {
                m_class->getOwnedAttributes().remove(*this);
            }
        }
    }
    m_class = clazz;
    if (m_class) {
        if (!m_class->getOwnedAttributes().count(m_id)) {
            m_class->getOwnedAttributes().add(*this);
        }
    }
}

Association* Property::getAssociation() {
    return m_association;
}

void Property::setAssociation(Association* association) {
    if (m_association) {
        if (m_association != association) {
            if (m_association->getMemberEnds().count(m_id)) {
                m_association->getMemberEnds().remove(*this);
            }
        }
    }
    m_association = association;
    if (m_association) {
        if (!m_association->getMemberEnds().count(m_id)) {
            m_association->getMemberEnds().add(*this);
        }
    }
}

Association* Property::getOwningAssociation() {
    return m_owningAssociation;
}

void Property::setOwningAssociation(Association* association) {
    if (m_owningAssociation) {
        if (m_owningAssociation != association) {
            if (m_owningAssociation->getNavigableOwnedEnds().count(m_id)) {
                m_owningAssociation->getNavigableOwnedEnds().remove(*this);
            }
            if (m_owningAssociation) {
                if (m_owningAssociation->getOwnedEnds().count(m_id)) {
                    m_owningAssociation->getOwnedEnds().remove(*this);
                }
            }
        }
    }

    m_owningAssociation = association;

    if (m_owningAssociation) {
        if (!m_owningAssociation->getOwnedEnds().count(m_id)) {
            m_owningAssociation->getOwnedEnds().add(*this);
        }
    }
}

void Property::setType(Type* type) {
    if (m_association) {
        if (!m_typeID.isNull()) {
            if (!m_typePtr) {
                m_typePtr = &m_manager->get<Type>(m_typeID);
            }
            if (m_typePtr != type) {
                if (m_association->getEndType().count(type->getID())) {
                    m_association->getEndType().remove(*type);
                }
            }
        }
    }
    TypedElement::setType(type);
    if (m_association) {
        if (!m_typeID.isNull()) {
            if (!m_association->getEndType().count(type->getID())) {
                m_association->getEndType().add(*type);
            }
        }
    }
}

ElementType Property::getElementType() const {
    return ElementType::PROPERTY;
}

bool Property::isSubClassOf(ElementType eType) {
    bool ret = StructuralFeature::isSubClassOf(eType);

    if (!ret) {
        ret = ConnectableElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::PROPERTY;
    }

    return ret;
}