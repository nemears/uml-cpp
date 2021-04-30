#include "uml/property.h"
#include "uml/primitiveType.h"
#include "uml/classifier.h"
#include "uml/dataType.h"
#include "uml/structuredClassifier.h"
#include "uml/association.h"

using namespace std;
using namespace UML;

void Property::reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) {
    if (m_classifier) {
        m_classifier->getAttributes().reindex(oldID, newID);
    }

    if (m_dataType) {
        m_dataType->getOwnedAttribute().reindex(oldID, newID);
    }

    Feature::reindexID(oldID, newID);
}

void Property::reindexName(string oldName, string newName) {
    if (m_classifier) {
        m_classifier->getAttributes().reindex(m_id, oldName, newName);
    }

    if (m_dataType) {
        m_dataType->getOwnedAttribute().reindex(m_id, oldName, newName);
    }

    Feature::reindexName(oldName, newName);
}

void Property::setDefaultValue(ValueSpecification* val) {
    defaultValue = val;
}

Property::Property() {
    defaultValue = 0;
    m_classifier = 0;
    m_dataType = 0;
    m_structuredClassifier = 0;
    m_association = 0;
    m_owningAssociation = 0;
}

// TODO remove?
Property::Property(const Property& prop) : StructuralFeature(prop), TypedElement(prop), NamedElement(prop), Element(prop) {
    defaultValue = prop.defaultValue;
    m_classifier = prop.m_classifier;
    m_dataType = prop.m_dataType;
    m_structuredClassifier = prop.m_structuredClassifier;
    m_association = prop.m_association;
    m_owningAssociation = prop.m_owningAssociation;
}

ValueSpecification* Property::getDefaultValue() {
    return defaultValue;
}

Classifier* Property::getClassifier() {
    return m_classifier;
}

void Property::setClassifier(Classifier* classifier) {
    if (m_classifier) {
        if (m_classifier->getAttributes().count(m_id)) {
            m_classifier->getAttributes().remove(*this);
        }
    }
    m_classifier = classifier;
    if (m_classifier) {
        if (!m_classifier->getAttributes().count(m_id)) {
            m_classifier->getAttributes().add(*this);
        }
    }
}

StructuredClassifier* Property::getStructuredClassifier() {
    return m_structuredClassifier;
}

void Property::setStructuredClassifier(StructuredClassifier* classifier) {
    if (m_structuredClassifier) {
        if (m_structuredClassifier != classifier) {
            if (m_structuredClassifier->getOwnedAttributes().count(m_id)) {
                m_structuredClassifier->getOwnedAttributes().remove(*this);
            }
        }
    }
    m_structuredClassifier = classifier;
    if (m_structuredClassifier) {
        if (!m_structuredClassifier->getOwnedAttributes().count(m_id)) {
            m_structuredClassifier->getOwnedAttributes().add(*this);
        }
    }
}

DataType* Property::getDataType() {
    return m_dataType;
}

void Property::setDataType(DataType* dataType) {
    if (m_dataType) {
        if (m_dataType->getOwnedAttribute().count(m_id)) {
            m_dataType->getOwnedAttribute().remove(*this);
        }
    }
    m_dataType = dataType;
    if (m_dataType) {
        if (!m_dataType->getOwnedAttribute().count(m_id)) {
            m_dataType->getOwnedAttribute().add(*this);
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
            if (m_owningAssociation->getOwnedEnds().count(m_id)) {
                m_owningAssociation->getOwnedEnds().remove(*this);
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