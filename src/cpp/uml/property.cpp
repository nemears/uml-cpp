#include "uml/property.h"
#include "uml/primitiveType.h"
#include "uml/classifier.h"
#include "uml/dataType.h"
#include "uml/structuredClassifier.h"
#include "uml/class.h"
#include "uml/association.h"
#include "uml/artifact.h"

using namespace std;
using namespace UML;

void Property::RemoveDefaultValueProcedure::operator()(ValueSpecification* el) const {
    m_me->getOwnedElements().internalRemove(*el);
}

void Property::AddDefaultValueProcedure::operator()(ValueSpecification* el) const {
    if (!m_me->getOwnedElements().count(el->getID())) {
        m_me->getOwnedElements().internalAdd(*el);
    }
    if (el->getOwnerID() != m_me->getID()) {
        el->setOwner(m_me);
    }
}

void Property::AddDefaultValueProcedure::operator()(ID id) const {
    if (!m_me->getOwnedElements().count(id)) {
        m_me->getOwnedElements().addByID(id);
    }
}

void Property::RemoveStructuredClassifierProcedure::operator()(StructuredClassifier* el) const {
    if (el->getOwnedAttributes().count(m_me->getID())) {
        el->getOwnedAttributes().remove(*m_me);
    }
}

void Property::AddStructuredClassifierProcedure::operator()(StructuredClassifier* el) const {
    if (!el->getOwnedAttributes().count(m_me->getID())) {
        el->getOwnedAttributes().add(*m_me);
    }
    if (m_me->getClassifierID() != el->getID()) {
        m_me->setClassifier(el);
    }
}

void Property::AddStructuredClassifierProcedure::operator()(ID id) const {
    if (m_me->getClassifierID() != id) {
        m_me->m_classifier.setByID(id);
    }
}

void Property::RemoveClassifierProcedure::operator()(Classifier* el) const {
    if (el->getAttributes().count(m_me->getID())) {
        el->getAttributes().remove(*m_me);
    }
}

void Property::AddClassifierProcedure::operator()(Classifier* el) const {
    if (!el->getAttributes().count(m_me->getID())) {
        el->getAttributes().add(*m_me);
    }
    if (!m_me->getRedefinedProperties().empty()) {
        if (!m_me->getRedefinitionContext().count(el->getID())) {
            m_me->getRedefinitionContext().add(*el);
        }
    }
    if (m_me->getFeaturingClassifierID() != el->getID()) {
        m_me->setFeaturingClassifier(el);
    }
}

void Property::AddClassifierProcedure::operator()(ID id) const {
    if (m_me->getFeaturingClassifierID() != id) {
        m_me->m_featuringClassifier.setByID(id);
    }
    if (!m_me->getRedefinedProperties().empty()) {
        if (!m_me->getRedefinitionContext().count(id)) {
            m_me->getRedefinitionContext().addByID(id);
        }
    }
}

void Property::RemoveDataTypeProcedure::operator()(DataType* el) const {
    if (el->getOwnedAttribute().count(m_me->getID())) {
        el->getOwnedAttribute().remove(*m_me);
    }
}

void Property::AddDataTypeProcedure::operator()(DataType* el) const {
    if (!el->getOwnedAttribute().count(m_me->getID())) {
        el->getOwnedAttribute().add(*m_me);
    }
    if (m_me->getClassifierID() != el->getID()) {
        m_me->setClassifier(el);
    }
}

void Property::AddDataTypeProcedure::operator()(ID id) const {
    if (m_me->getClassifierID() != id) {
        m_me->m_classifier.setByID(id);
    }
}

void Property::RemoveClassProcedure::operator()(Class* el) const {
    if (el->getOwnedAttributes().count(m_me->getID())) {
        el->getOwnedAttributes().remove(*m_me);
    }
}

void Property::AddClassProcedure::operator()(Class* el) const {
    if (!el->getOwnedAttributes().count(m_me->getID())) {
        el->getOwnedAttributes().add(*m_me);
    }
    if (m_me->getStructuredClassifierID() != el->getID()) {
        m_me->setStructuredClassifier(el);
    }
}

void Property::AddClassProcedure::operator()(ID id) const {
    if (m_me->getStructuredClassifierID() != id) {
        m_me->m_structuredClassifier.setByID(id);
    }
}

void Property::RemoveAssociationProcedure::operator()(Association* el) const {
    if (el->getMemberEnds().count(m_me->getID())) {
        el->getMemberEnds().remove(*m_me);
    }
}

void Property::AddAssociationProcedure::operator()(Association* el) const {
    if (!el->getMemberEnds().count(m_me->getID())) {
        el->getMemberEnds().add(*m_me);
    }
    if (!m_me->getMemberNamespace().count(el->getID())) {
        m_me->getMemberNamespace().add(*el);
    }
}

void Property::AddAssociationProcedure::operator()(ID id) const {
    if (!m_me->getMemberNamespace().count(id)) {
        m_me->getMemberNamespace().addByID(id);
    }
}

void Property::RemoveOwningAssociationProcedure::operator()(Association* el) const {
    if (el->getOwnedEnds().count(m_me->getID())) {
        el->getOwnedEnds().remove(*m_me);
    }
    if (el->getNavigableOwnedEnds().count(m_me->getID())) {
        el->getNavigableOwnedEnds().remove(*m_me);
    }
}

void Property::AddOwningAssociationProcedure::operator()(Association* el) const {
    if (!el->getOwnedEnds().count(m_me->getID())) {
        el->getOwnedEnds().add(*m_me);
    }
    if (m_me->getAssociationID() != el->getID()) {
        m_me->m_association.set(el);
    }
    if (m_me->getFeaturingClassifierID() != el->getID()) {
        m_me->m_featuringClassifier.set(el);
    }
}

void Property::AddOwningAssociationProcedure::operator()(ID id) const {
    if (m_me->getAssociationID() != id) {
        m_me->m_association.setByID(id);
    }
    if (m_me->getFeaturingClassifierID() != id) {
        m_me->m_featuringClassifier.setByID(id);
    }
}

void Property::RemoveArtifactProcedure::operator()(Artifact* el) const {
    if (el->getOwnedAttributes().count(m_me->getID())) {
        el->getOwnedAttributes().remove(*m_me);
    }
}

void Property::AddArtifactProcedure::operator()(Artifact* el) const {
    if (!el->getOwnedAttributes().count(m_me->getID())) {
        el->getOwnedAttributes().add(*m_me);
    }
    if (m_me->getClassifierID() != el->getID()) {
        m_me->setClassifier(el);
    }
}

void Property::AddArtifactProcedure::operator()(ID id) const {
    if (m_me->getClassifierID() != id) {
        m_me->m_classifier.setByID(id);
    }
}

void Property::RemoveTypeProcedure::operator()(Type* el) const {
    if (m_me->m_association.has()) {
        if (m_me->m_association.get()->getEndType().count(m_me->m_type.get()->getID())) {
            m_me->m_association.get()->getEndType().remove(m_me->m_type.getRef());
        }
    }
}

void Property::AddTypeProcedure::operator()(Type* el) const {
    if (m_me->m_association.has()) {
        if (el) {
            if (!m_me->m_association.get()->getEndType().count(el->getID())) {
                m_me->m_association.get()->getEndType().add(*el);
            }
        }
    }
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

void Property::AddRedefinedPropertyFunctor::operator()(Property& el) const {
    if (!m_el->m_redefinedElement.count(el.getID())) {
        m_el->m_redefinedElement.add(el);
    }

    if (m_el->m_classifier.has()) {
        if (!m_el->m_redefinitionContext.count(m_el->m_classifier.id())) {
            m_el->m_redefinitionContext.add(*m_el->getClassifier());
        }
    }
    el.setReference(m_el);
    updateCopiedSequenceAddedTo(el, &Property::getRedefinedProperties);
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

void Property::CheckRedefinedPropertyFunctor::operator()(Property& el) const {
    // if (!m_el->as<Property>().m_classifierID.isNull()) {
    //     if (!findProperty(*m_el->as<Property>().getClassifier(), el.getID())) {
    //         throw ImproperRedefinitionException();
    //     }
    // } else {
    //     throw ImproperRedefinitionException();
    // }
}

void Property::RemoveRedefinedPropertyFunctor::operator()(Property& el) const {
    if (m_el->m_redefinedElement.count(el.getID())) {
        m_el->m_redefinedElement.remove(el);
    }

    if (!m_el->m_classifier.has()) {
        if (m_el->m_redefinitionContext.count(m_el->m_classifier.id()) && m_el->m_redefinedProperties.empty()) {
            m_el->m_redefinitionContext.add(*m_el->getClassifier());
        }
    }
    el.removeReference(m_el->getID());
    updateCopiedSequenceRemovedFrom(el, &Property::getRedefinedProperties);
}

void Property::setManager(UmlManager* manager) {
    NamedElement::setManager(manager);
    RedefinableElement::setManager(manager);
    m_redefinedProperties.m_manager = manager;
}

void Property::referencingReleased(ID id) {
    StructuralFeature::referencingReleased(id);
    m_defaultValue.release(id);
    m_classifier.release(id);
    m_structuredClassifier.release(id);
    m_dataType.release(id);
    m_class.release(id);
    m_artifact.release(id);
    m_association.release(id);
    m_owningAssociation.release(id);
    m_redefinedProperties.elementReleased(id, &Property::getRedefinedProperties);
}

void Property::referenceReindexed(ID oldID, ID newID) {
    StructuralFeature::referenceReindexed(oldID, newID);
    m_defaultValue.reindex(oldID, newID);
    m_classifier.reindex(oldID, newID);
    m_structuredClassifier.reindex(oldID, newID);
    m_dataType.reindex(oldID, newID);
    m_class.reindex(oldID, newID);
    m_artifact.reindex(oldID, newID);
    m_association.reindex(oldID, newID);
    m_owningAssociation.reindex(oldID, newID);
    m_redefinedProperties.reindex(oldID, newID, &Property::getRedefinedProperties);
}

void Property::restoreReferences() {
    StructuralFeature::restoreReferences();
    m_defaultValue.restoreReference();
    m_classifier.restoreReference();
    m_structuredClassifier.restoreReference();
    m_dataType.restoreReference();
    m_class.restoreReference();
    m_association.restoreReference();
    m_owningAssociation.restoreReference();
    m_artifact.restoreReference();
    m_redefinedProperties.restoreReferences();
}

void Property::restoreReference(Element* el) {
    StructuralFeature::restoreReference(el);
    if (m_redefinedProperties.count(el->getID())) {
        el->setReference(this);
    }
}

void Property::referenceErased(ID id) {
    StructuralFeature::referenceErased(id);
    DeploymentTarget::referenceErased(id);
    m_defaultValue.elementErased(id);
    m_classifier.elementErased(id);
    m_structuredClassifier.elementErased(id);
    m_dataType.elementErased(id);
    m_class.elementErased(id);
    m_association.elementErased(id);
    m_owningAssociation.elementErased(id);
    m_artifact.elementErased(id);
    m_redefinedProperties.elementErased(id);
}

Property::Property() {
    m_aggregation = AggregationKind::NONE;
    m_composite = false;
    m_defaultValue.m_signature = &Property::m_defaultValue;
    m_defaultValue.m_removeProcedures.push_back(new RemoveDefaultValueProcedure(this));
    m_defaultValue.m_addProcedures.push_back(new AddDefaultValueProcedure(this));
    m_classifier.m_signature = &Property::m_classifier;
    m_classifier.m_removeProcedures.push_back(new RemoveClassifierProcedure(this));
    m_classifier.m_addProcedures.push_back(new AddClassifierProcedure(this));
    m_dataType.m_signature = &Property::m_dataType;
    m_dataType.m_removeProcedures.push_back(new RemoveDataTypeProcedure(this));
    m_dataType.m_addProcedures.push_back(new AddDataTypeProcedure(this));
    m_structuredClassifier.m_signature = &Property::m_structuredClassifier;
    m_structuredClassifier.m_removeProcedures.push_back(new RemoveStructuredClassifierProcedure(this));
    m_structuredClassifier.m_addProcedures.push_back(new AddStructuredClassifierProcedure(this));
    m_class.m_signature = &Property::m_class;
    m_class.m_removeProcedures.push_back(new RemoveClassProcedure(this));
    m_class.m_addProcedures.push_back(new AddClassProcedure(this));
    m_association.m_signature = &Property::m_association;
    m_association.m_removeProcedures.push_back(new RemoveAssociationProcedure(this));
    m_association.m_addProcedures.push_back(new AddAssociationProcedure(this));
    m_owningAssociation.m_signature = &Property::m_owningAssociation;
    m_owningAssociation.m_removeProcedures.push_back(new RemoveOwningAssociationProcedure(this));
    m_owningAssociation.m_addProcedures.push_back(new AddOwningAssociationProcedure(this));
    m_artifact.m_signature = &Property::m_artifact;
    m_artifact.m_removeProcedures.push_back(new RemoveArtifactProcedure(this));
    m_artifact.m_addProcedures.push_back(new AddArtifactProcedure(this));
    m_redefinedProperties.addProcedures.push_back(new AddRedefinedPropertyFunctor(this));
    m_redefinedProperties.addChecks.push_back(new CheckRedefinedPropertyFunctor(this));
    m_redefinedProperties.removeProcedures.push_back(new RemoveRedefinedPropertyFunctor(this));
    m_type.m_addProcedures.push_back(new AddTypeProcedure(this));
    m_type.m_removeProcedures.push_back(new RemoveTypeProcedure(this));
}

Property::Property(const Property& prop) : StructuralFeature(prop), TypedElement(prop), RedefinableElement(prop), NamedElement(prop), Element(prop) {
    m_aggregation = prop.m_aggregation;
    m_composite = prop.m_composite;
    m_defaultValue = prop.m_defaultValue;
    m_defaultValue.m_me = this;
    m_defaultValue.m_removeProcedures.clear();
    m_defaultValue.m_addProcedures.clear();
    m_defaultValue.m_removeProcedures.push_back(new RemoveDefaultValueProcedure(this));
    m_defaultValue.m_addProcedures.push_back(new AddDefaultValueProcedure(this));
    m_classifier = prop.m_classifier;
    m_classifier.m_me = this;
    m_classifier.m_removeProcedures.clear();
    m_classifier.m_addProcedures.clear();
    m_classifier.m_removeProcedures.push_back(new RemoveClassifierProcedure(this));
    m_classifier.m_addProcedures.push_back(new AddClassifierProcedure(this));
    m_dataType = prop.m_dataType;
    m_dataType.m_me = this;
    m_dataType.m_removeProcedures.clear();
    m_dataType.m_addProcedures.clear();
    m_dataType.m_removeProcedures.push_back(new RemoveDataTypeProcedure(this));
    m_dataType.m_addProcedures.push_back(new AddDataTypeProcedure(this));
    m_class = prop.m_class;
    m_class.m_me = this;
    m_class.m_removeProcedures.clear();
    m_class.m_addProcedures.clear();
    m_class.m_removeProcedures.push_back(new RemoveClassProcedure(this));
    m_class.m_addProcedures.push_back(new AddClassProcedure(this));
    m_structuredClassifier = prop.m_structuredClassifier;
    m_structuredClassifier.m_me = this;
    m_structuredClassifier.m_removeProcedures.clear();
    m_structuredClassifier.m_addProcedures.clear();
    m_structuredClassifier.m_removeProcedures.clear();
    m_structuredClassifier.m_removeProcedures.push_back(new RemoveStructuredClassifierProcedure(this));
    m_structuredClassifier.m_addProcedures.push_back(new AddStructuredClassifierProcedure(this));
    m_association = prop.m_association;
    m_association.m_me = this;
    m_association.m_removeProcedures.clear();
    m_association.m_addProcedures.clear();
    m_association.m_removeProcedures.push_back(new RemoveAssociationProcedure(this));
    m_association.m_addProcedures.push_back(new AddAssociationProcedure(this));
    m_owningAssociation = prop.m_owningAssociation;
    m_owningAssociation.m_me = this;
    m_owningAssociation.m_removeProcedures.clear();
    m_owningAssociation.m_addProcedures.clear();
    m_owningAssociation.m_removeProcedures.push_back(new RemoveOwningAssociationProcedure(this));
    m_owningAssociation.m_addProcedures.push_back(new AddOwningAssociationProcedure(this));
    m_artifact = prop.m_artifact;
    m_artifact.m_me = this;
    m_artifact.m_removeProcedures.clear();
    m_artifact.m_addProcedures.clear();
    m_artifact.m_removeProcedures.push_back(new RemoveArtifactProcedure(this));
    m_artifact.m_addProcedures.push_back(new AddArtifactProcedure(this));
    m_redefinedProperties = prop.m_redefinedProperties;
    m_redefinedProperties.m_el = this;
    m_redefinedProperties.addProcedures.clear();
    m_redefinedProperties.addChecks.clear();
    m_redefinedProperties.removeProcedures.clear();
    m_redefinedProperties.addProcedures.push_back(new AddRedefinedPropertyFunctor(this));
    m_redefinedProperties.addChecks.push_back(new CheckRedefinedPropertyFunctor(this));
    m_redefinedProperties.removeProcedures.push_back(new RemoveRedefinedPropertyFunctor(this));
    m_redefinedProperties.m_el = this;
    m_type.m_addProcedures.push_back(new AddTypeProcedure(this));
    m_type.m_removeProcedures.push_back(new RemoveTypeProcedure(this));
}

AggregationKind Property::getAggregation() {
    return m_aggregation;
}

bool Property::isComposite() {
    return m_composite;
}

void Property::setComposite(bool composite) {
    if (!composite && m_composite) {
        if (m_structuredClassifier.has()) {
            if (m_structuredClassifier.get()->getParts().count(m_id)) {
                m_structuredClassifier.get()->getParts().remove(*this);
            }
        }
    }
    m_composite = composite;
    if (m_composite) {
        if (m_structuredClassifier.has()) {
            if (!m_structuredClassifier.get()->getParts().count(m_id)) {
                m_structuredClassifier.get()->getParts().add(*this);
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
    return m_defaultValue.get();
}

ValueSpecification& Property::getDefaultValueRef() {
    return m_defaultValue.getRef();
}

ID Property::getDefaultValueID() const {
    return m_defaultValue.id();
}

bool Property::hasDefaultValue() const {
    return m_defaultValue.has();
}

void Property::setDefaultValue(ValueSpecification* val) {
    m_defaultValue.set(val);
}

void Property::setDefaultValue(ValueSpecification& val) {
    m_defaultValue.set(val);
}

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

StructuredClassifier* Property::getStructuredClassifier() {
    return m_structuredClassifier.get();
}

StructuredClassifier& Property::getStructuredClassifierRef() {
    return m_structuredClassifier.getRef();
}

ID Property::getStructuredClassifierID() const {
    return m_structuredClassifier.id();
}

bool Property::hasStructuredClassifier() const {
    return m_structuredClassifier.has();
}

void Property::setStructuredClassifier(StructuredClassifier* classifier) {
    m_structuredClassifier.set(classifier);
}

void Property::setStructuredClassifier(StructuredClassifier& classifier) {
    m_structuredClassifier.set(classifier);
}

DataType* Property::getDataType() {
    return m_dataType.get();
}

DataType& Property::getDataTypeRef() {
    return m_dataType.getRef();
}

ID Property::getDataTypeID() const {
    return m_dataType.id();
}

bool Property::hasDataType() const {
    return m_dataType.has();
}

void Property::setDataType(DataType* dataType) {
    m_dataType.set(dataType);
}

void Property::setDataType(DataType& dataType) {
    m_dataType.set(dataType);
}

Class* Property::getClass() {
    return m_class.get();
}

Class& Property::getClassRef() {
    return m_class.getRef();
}

ID Property::getClassID() const {
    return m_class.id();
}

bool Property::hasClass() const {
    return m_class.has();
}

void Property::setClass(Class* clazz) {
    m_class.set(clazz);
}

void Property::setClass(Class& clazz) {
    m_class.set(clazz);
}

Association* Property::getAssociation() {
    return m_association.get();
}

Association& Property::getAssociationRef() {
    return m_association.getRef();
}

ID Property::getAssociationID() const {
    return m_association.id();
}

bool Property::hasAssociation() const {
    return m_association.has();
}

void Property::setAssociation(Association* association) {
    m_association.set(association);
}

void Property::setAssociation(Association& association) {
    m_association.set(association);
}

Association* Property::getOwningAssociation() {
    return m_owningAssociation.get();
}

Association& Property::getOwningAssociationRef() {
    return m_owningAssociation.getRef();
}

ID Property::getOwningAssociationID() const {
    return m_owningAssociation.id();
}

bool Property::hasOwningAssociation() const {
    return m_owningAssociation.has();
}

void Property::setOwningAssociation(Association* association) {
    m_owningAssociation.set(association);
}

void Property::setOwningAssociation(Association& association) {
    m_owningAssociation.set(association);
}

Artifact* Property::getArtifact() {
    return m_artifact.get();
}

Artifact& Property::getArtifactRef() {
    return m_artifact.getRef();
}

ID Property::getArtifactID() const {
    return m_artifact.id();
}

bool Property::hasArtifact() const {
    return m_artifact.has();
}

void Property::setArtifact(Artifact* artifact) {
    m_artifact.set(artifact);
}

void Property::setArtifact(Artifact& artifact) {
    m_artifact.set(artifact);
}

Sequence<Property>& Property::getRedefinedProperties() {
    return m_redefinedProperties;
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