#include "uml/property.h"
#include "uml/primitiveType.h"
#include "uml/classifier.h"
#include "uml/dataType.h"
#include "uml/structuredClassifier.h"
#include "uml/class.h"
#include "uml/association.h"
#include "uml/artifact.h"
#include "uml/universalFunctions.h"

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

void Property::AddRedefinedPropertyFunctor::operator()(Element& el) const {
    if (!m_el->as<Property>().m_redefinedElement.count(el.getID())) {
        m_el->as<Property>().m_redefinedElement.add(el.as<RedefinableElement>());
    }

    if (!m_el->as<Property>().m_classifierID.isNull()) {
        if (!m_el->as<Property>().m_redefinitionContext.count(m_el->as<Property>().m_classifierID)) {
            m_el->as<Property>().m_redefinitionContext.add(*m_el->as<Property>().getClassifier());
        }
        
    }
}

bool findProperty(Classifier& clazz, ID propID) {
    for (auto& general : clazz.getGenerals()) {
        if (!general.getAttributes().count(propID)) {
            bool ret = findProperty(general, propID);
            if (ret) {
                return ret;
            }
        }
        else {
            return true;
        }
    }
    return false;
}

void Property::CheckRedefinedPropertyFunctor::operator()(Element& el) const {
    // if (!m_el->as<Property>().m_classifierID.isNull()) {
    //     if (!findProperty(*m_el->as<Property>().getClassifier(), el.getID())) {
    //         throw ImproperRedefinitionException();
    //     }
    // } else {
    //     throw ImproperRedefinitionException();
    // }
}

void Property::RemoveRedefinedPropertyFunctor::operator()(Element& el) const {
    if (m_el->as<Property>().m_redefinedElement.count(el.getID())) {
        m_el->as<Property>().m_redefinedElement.remove(el.as<RedefinableElement>());
    }

    if (!m_el->as<Property>().m_classifierID.isNull()) {
        if (m_el->as<Property>().m_redefinitionContext.count(m_el->as<Property>().m_classifierID) && m_el->as<Property>().m_redefinedProperties.empty()) {
            m_el->as<Property>().m_redefinitionContext.add(*m_el->as<Property>().getClassifier());
        }
        
    }
}

void Property::setManager(UmlManager* manager) {
    NamedElement::setManager(manager);
    RedefinableElement::setManager(manager);
    m_redefinedProperties.m_manager = manager;
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
    m_artifactPtr = 0;
    m_redefinedProperties.addProcedures.push_back(new AddRedefinedPropertyFunctor(this));
    m_redefinedProperties.addChecks.push_back(new CheckRedefinedPropertyFunctor(this));
    m_redefinedProperties.removeProcedures.push_back(new RemoveRedefinedPropertyFunctor(this));
}

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
    m_artifactID = prop.m_artifactID;
    m_artifactPtr = prop.m_artifactPtr;
    m_redefinedProperties = prop.m_redefinedProperties;
    m_redefinedProperties.addProcedures.clear();
    m_redefinedProperties.addChecks.clear();
    m_redefinedProperties.removeProcedures.clear();
    m_redefinedProperties.addProcedures.push_back(new AddRedefinedPropertyFunctor(this));
    m_redefinedProperties.addChecks.push_back(new CheckRedefinedPropertyFunctor(this));
    m_redefinedProperties.removeProcedures.push_back(new RemoveRedefinedPropertyFunctor(this));
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
    return universalGet<ValueSpecification>(m_defaultValueID, m_defaultValuePtr, m_manager);
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
    return universalGet<Classifier>(m_classifierID, m_classifierPtr, m_manager);
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
}

StructuredClassifier* Property::getStructuredClassifier() {
    return universalGet<StructuredClassifier>(m_structuredClassifierID, m_structuredClassifierPtr, m_manager);
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
    return universalGet<DataType>(m_dataTypeID, m_dataTypePtr, m_manager);
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
    return universalGet<Class>(m_classID, m_classPtr, m_manager);
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
    return universalGet<Association>(m_associationID, m_associationPtr, m_manager);
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
    return universalGet<Association>(m_owningAssociationID, m_owningAssociationPtr, m_manager);
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

Artifact* Property::getArtifact() {
    return universalGet<Artifact>(m_artifactID, m_artifactPtr, m_manager);
}

void Property::setArtifact(Artifact* artifact) {
    if (!isSameOrNull(m_artifactID, artifact)) {
        if (!m_artifactPtr) {
            m_artifactPtr = &m_manager->get<Artifact>(m_artifactID);
        }

        if (m_artifactPtr->getOwnedAttributes().count(m_id)) {
            m_artifactPtr->getOwnedAttributes().remove(*this);
        }

        m_artifactPtr = 0;
        m_artifactID = ID::nullID();
    }

    if (artifact) {
        m_artifactID = artifact->getID();
    }

    if (!m_manager) {
        m_artifactPtr = artifact;
    }

    if (artifact) {
        if (!artifact->getOwnedAttributes().count(m_id)) {
            artifact->getOwnedAttributes().add(*this);
        }
    }
}

Sequence<Property>& Property::getRedefinedProperties() {
    return m_redefinedProperties;
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
        ret = DeploymentTarget::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::PROPERTY;
    }

    return ret;
}