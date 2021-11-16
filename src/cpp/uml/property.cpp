#include "uml/property.h"
#include "uml/classifier.h"

using namespace UML;

void Property::referencingReleased(ID id) {
    StructuralFeature::referencingReleased(id);
    // m_defaultValue.release(id);
    m_classifier.release(id);
    // m_structuredClassifier.release(id);
    // m_dataType.release(id);
    // m_class.release(id);
    // m_artifact.release(id);
    // m_association.release(id);
    // m_owningAssociation.release(id);
    // m_redefinedProperties.elementReleased(id, &Property::getRedefinedProperties);
}

void Property::referenceReindexed(ID oldID, ID newID) {
    StructuralFeature::referenceReindexed(oldID, newID);
    // m_defaultValue.reindex(oldID, newID);
    m_classifier.reindex(oldID, newID);
    // m_structuredClassifier.reindex(oldID, newID);
    // m_dataType.reindex(oldID, newID);
    // m_class.reindex(oldID, newID);
    // m_artifact.reindex(oldID, newID);
    // m_association.reindex(oldID, newID);
    // m_owningAssociation.reindex(oldID, newID);
    // m_redefinedProperties.reindex(oldID, newID, &Property::getRedefinedProperties);
}

void Property::restoreReferences() {
    StructuralFeature::restoreReferences();
    // m_defaultValue.restoreReference();
    // m_classifier.restoreReference();
    // m_structuredClassifier.restoreReference();
    // m_dataType.restoreReference();
    // m_class.restoreReference();
    // m_association.restoreReference();
    // m_owningAssociation.restoreReference();
    // m_artifact.restoreReference();
    // m_redefinedProperties.restoreReferences();
}

void Property::restoreReference(Element* el) {
    StructuralFeature::restoreReference(el);
    // if (m_redefinedProperties.count(el->getID())) {
    //     el->setReference(this);
    // }
}

void Property::referenceErased(ID id) {
    StructuralFeature::referenceErased(id);
    // DeploymentTarget::referenceErased(id);
    // m_defaultValue.elementErased(id);
    m_classifier.eraseElement(id);
    // m_structuredClassifier.elementErased(id);
    // m_dataType.elementErased(id);
    // m_class.elementErased(id);
    // m_association.elementErased(id);
    // m_owningAssociation.elementErased(id);
    // m_artifact.elementErased(id);
    // m_redefinedProperties.elementErased(id);
}

Set<Classifier, Property>& Property::getClassifierSingleton() {
    return m_classifier;
}

void Property::init() {
    m_classifier.subsets(m_featuringClassifier);
    m_classifier.opposite(&Classifier::getAttributes);
    m_classifier.m_signature = &Property::getClassifierSingleton;
}

void Property::copy(const Property& rhs) {

}

Property::Property() : Element(ElementType::PROPERTY) {
    init();
}

Property::Property(const Property& rhs) : Element(rhs, ElementType::PROPERTY) {
    // TODO
}

AggregationKind Property::getAggregation() {
    return m_aggregation;
}

bool Property::isComposite() {
    return m_composite;
}

void Property::setComposite(bool composite) {
    // if (!composite && m_composite) {
    //     if (m_structuredClassifier.has()) {
    //         if (m_structuredClassifier.get()->getParts().count(m_id)) {
    //             m_structuredClassifier.get()->getParts().remove(*this);
    //         }
    //     }
    // }
    m_composite = composite;
    // if (m_composite) {
    //     if (m_structuredClassifier.has()) {
    //         if (!m_structuredClassifier.get()->getParts().count(m_id)) {
    //             m_structuredClassifier.get()->getParts().add(*this);
    //         }
    //     }
    // }
    updateCopiesScalar(composite, &Property::m_composite);
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
    updateCopiesScalar(aggregation, &Property::m_aggregation);
}

// ValueSpecification* Property::getDefaultValue() {
//     return m_defaultValue.get();
// }

// ValueSpecification& Property::getDefaultValueRef() {
//     return m_defaultValue.getRef();
// }

// ID Property::getDefaultValueID() const {
//     return m_defaultValue.id();
// }

// bool Property::hasDefaultValue() const {
//     return m_defaultValue.has();
// }

// void Property::setDefaultValue(ValueSpecification* val) {
//     m_defaultValue.set(val);
// }

// void Property::setDefaultValue(ValueSpecification& val) {
//     m_defaultValue.set(val);
// }

Classifier* Property::getClassifier() {
    return m_classifier.get();
}

Classifier& Property::getClassifierRef() {
    return m_classifier.getRef();
}

ID Property::getClassifierID() const {
    return m_classifier.id();
}

bool Property::hasClassifier() const {
    return m_classifier.has();
}

void Property::setClassifier(Classifier* classifier) {
    m_classifier.set(classifier);
}

void Property::setClassifier(Classifier& classifier) {
    m_classifier.set(classifier);
}

// StructuredClassifier* Property::getStructuredClassifier() {
//     return m_structuredClassifier.get();
// }

// StructuredClassifier& Property::getStructuredClassifierRef() {
//     return m_structuredClassifier.getRef();
// }

// ID Property::getStructuredClassifierID() const {
//     return m_structuredClassifier.id();
// }

// bool Property::hasStructuredClassifier() const {
//     return m_structuredClassifier.has();
// }

// void Property::setStructuredClassifier(StructuredClassifier* classifier) {
//     m_structuredClassifier.set(classifier);
// }

// void Property::setStructuredClassifier(StructuredClassifier& classifier) {
//     m_structuredClassifier.set(classifier);
// }

// DataType* Property::getDataType() {
//     return m_dataType.get();
// }

// DataType& Property::getDataTypeRef() {
//     return m_dataType.getRef();
// }

// ID Property::getDataTypeID() const {
//     return m_dataType.id();
// }

// bool Property::hasDataType() const {
//     return m_dataType.has();
// }

// void Property::setDataType(DataType* dataType) {
//     m_dataType.set(dataType);
// }

// void Property::setDataType(DataType& dataType) {
//     m_dataType.set(dataType);
// }

// Class* Property::getClass() {
//     return m_class.get();
// }

// Class& Property::getClassRef() {
//     return m_class.getRef();
// }

// ID Property::getClassID() const {
//     return m_class.id();
// }

// bool Property::hasClass() const {
//     return m_class.has();
// }

// void Property::setClass(Class* clazz) {
//     m_class.set(clazz);
// }

// void Property::setClass(Class& clazz) {
//     m_class.set(clazz);
// }

// Association* Property::getAssociation() {
//     return m_association.get();
// }

// Association& Property::getAssociationRef() {
//     return m_association.getRef();
// }

// ID Property::getAssociationID() const {
//     return m_association.id();
// }

// bool Property::hasAssociation() const {
//     return m_association.has();
// }

// void Property::setAssociation(Association* association) {
//     m_association.set(association);
// }

// void Property::setAssociation(Association& association) {
//     m_association.set(association);
// }

// Association* Property::getOwningAssociation() {
//     return m_owningAssociation.get();
// }

// Association& Property::getOwningAssociationRef() {
//     return m_owningAssociation.getRef();
// }

// ID Property::getOwningAssociationID() const {
//     return m_owningAssociation.id();
// }

// bool Property::hasOwningAssociation() const {
//     return m_owningAssociation.has();
// }

// void Property::setOwningAssociation(Association* association) {
//     m_owningAssociation.set(association);
// }

// void Property::setOwningAssociation(Association& association) {
//     m_owningAssociation.set(association);
// }

// Artifact* Property::getArtifact() {
//     return m_artifact.get();
// }

// Artifact& Property::getArtifactRef() {
//     return m_artifact.getRef();
// }

// ID Property::getArtifactID() const {
//     return m_artifact.id();
// }

// bool Property::hasArtifact() const {
//     return m_artifact.has();
// }

// void Property::setArtifact(Artifact* artifact) {
//     m_artifact.set(artifact);
// }

// void Property::setArtifact(Artifact& artifact) {
//     m_artifact.set(artifact);
// }

// Sequence<Property>& Property::getRedefinedProperties() {
//     return m_redefinedProperties;
// }

bool Property::isSubClassOf(ElementType eType) const {
    bool ret = StructuralFeature::isSubClassOf(eType);

    // if (!ret) {
    //     ret = ConnectableElement::isSubClassOf(eType);
    // }

    // if (!ret) {
    //     ret = DeploymentTarget::isSubClassOf(eType);
    // }

    if (!ret) {
        ret = eType == ElementType::PROPERTY;
    }

    return ret;
}