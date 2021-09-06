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

void Property::RemoveDefaultValueProcedure::operator()(ValueSpecification* el) const {
    m_me->getOwnedElements().internalRemove(*el);
}

void Property::AddDefaultValueProcedure::operator()(ValueSpecification* el) const {
    if (!m_me->getOwnedElements().count(el->getID())) {
        m_me->getOwnedElements().internalAdd(*el);
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
}

void Property::RemoveOwningAssociationProcedure::operator()(Association* el) const {
    if (el->getOwnedEnds().count(m_me->getID())) {
        el->getOwnedEnds().remove(*m_me);
    }
}

void Property::AddOwningAssociationProcedure::operator()(Association* el) const {
    if (!el->getOwnedEnds().count(m_me->getID())) {
        el->getOwnedEnds().add(*m_me);
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
}

void Property::reindexID(ID oldID, ID newID) {
    if (!m_classifier.has()) {
        m_classifier.get()->getAttributes().reindex(oldID, newID);
    }

    if (!m_dataType.has()) {
        m_dataType.get()->getOwnedAttribute().reindex(oldID, newID);
    }

    if (!m_structuredClassifier.has()) {
        if (m_structuredClassifier.get()->getOwnedAttributes().count(oldID)) {
            m_structuredClassifier.get()->getOwnedAttributes().reindex(oldID, newID);
        }
        if (m_structuredClassifier.get()->getRole().count(oldID)) {
            m_structuredClassifier.get()->getRole().reindex(oldID, newID);
        }
        if (m_structuredClassifier.get()->getParts().count(oldID)) {
            m_structuredClassifier.get()->getParts().reindex(oldID, newID);
        }
    }

    if (!m_association.has()) {
        if (m_association.get()->getMemberEnds().count(oldID)) {
            m_association.get()->getMemberEnds().reindex(oldID, newID);
        }
    }

    if (!m_owningAssociation.has()) {
        if (m_owningAssociation.get()->getOwnedEnds().count(oldID)) {
            m_owningAssociation.get()->getOwnedEnds().reindex(oldID, newID);
        }
        if (m_owningAssociation.get()->getNavigableOwnedEnds().count(oldID)) {
            m_owningAssociation.get()->getNavigableOwnedEnds().reindex(oldID, newID);
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

void Property::AddRedefinedPropertyFunctor::operator()(Property& el) const {
    if (!m_el->m_redefinedElement.count(el.getID())) {
        m_el->m_redefinedElement.add(el);
    }

    if (!m_el->m_classifier.has()) {
        if (!m_el->m_redefinitionContext.count(m_el->m_classifier.id())) {
            m_el->m_redefinitionContext.add(*m_el->getClassifier());
        }
    }

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
    updateCopiedSequenceRemovedFrom(el, &Property::getRedefinedProperties);
}

void Property::setManager(UmlManager* manager) {
    NamedElement::setManager(manager);
    RedefinableElement::setManager(manager);
    m_redefinedProperties.m_manager = manager;
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
    m_redefinedProperties.addProcedures.clear();
    m_redefinedProperties.addChecks.clear();
    m_redefinedProperties.removeProcedures.clear();
    m_redefinedProperties.addProcedures.push_back(new AddRedefinedPropertyFunctor(this));
    m_redefinedProperties.addChecks.push_back(new CheckRedefinedPropertyFunctor(this));
    m_redefinedProperties.removeProcedures.push_back(new RemoveRedefinedPropertyFunctor(this));
    m_redefinedProperties.m_el = this;
}

AggregationKind Property::getAggregation() {
    return m_aggregation;
}

bool Property::isComposite() {
    return m_composite;
}

void Property::setComposite(bool composite) {
    if (!composite && m_composite) {
        if (!m_structuredClassifier.has()) {
            if (m_structuredClassifier.get()->getParts().count(m_id)) {
                m_structuredClassifier.get()->getParts().remove(*this);
            }
        }
    }
    m_composite = composite;
    if (m_composite) {
        if (!m_structuredClassifier.has()) {
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

void Property::setDefaultValue(ValueSpecification* val) {
    m_defaultValue.set(val);
}

Classifier* Property::getClassifier() {
    return m_classifier.get();
}

void Property::setClassifier(Classifier* classifier) {
    m_classifier.set(classifier);
}

StructuredClassifier* Property::getStructuredClassifier() {
    return m_structuredClassifier.get();
}

void Property::setStructuredClassifier(StructuredClassifier* classifier) {
    m_structuredClassifier.set(classifier);
}

DataType* Property::getDataType() {
    return m_dataType.get();
}

void Property::setDataType(DataType* dataType) {
    m_dataType.set(dataType);
}

Class* Property::getClass() {
    return m_class.get();
}

void Property::setClass(Class* clazz) {
    m_class.set(clazz);
}

Association* Property::getAssociation() {
    return m_association.get();
}

void Property::setAssociation(Association* association) {
    m_association.set(association);
}

Association* Property::getOwningAssociation() {
    return m_owningAssociation.get();
}

void Property::setOwningAssociation(Association* association) {
    m_owningAssociation.set(association);
}

Artifact* Property::getArtifact() {
    return m_artifact.get();
}

void Property::setArtifact(Artifact* artifact) {
    m_artifact.set(artifact);
}

Sequence<Property>& Property::getRedefinedProperties() {
    return m_redefinedProperties;
}

void Property::setType(Type* type) {
    if (!m_association.has()) {
        if (!m_typeID.isNull()) {
            if (!m_typePtr) {
                m_typePtr = &m_manager->get<Type>(m_typeID);
            }
            if (m_typePtr != type) {
                if (m_association.get()->getEndType().count(m_typePtr->getID())) {
                    m_association.get()->getEndType().remove(*m_typePtr);
                }
            }
        }
    }
    TypedElement::setType(type);
    if (!m_association.has()) {
        if (type) {
            if (!m_association.get()->getEndType().count(type->getID())) {
                m_association.get()->getEndType().add(*type);
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

void Property::restoreReleased(ID id, Element* released) {
    StructuralFeature::restoreReleased(id, released);
}

void Property::referencingReleased(ID id) {
    StructuralFeature::referencingReleased(id);
    if (m_defaultValue.id() == id) {
        m_defaultValue.release();
    }
    if (m_classifier.id() == id) {
        m_classifier.release();
    }
    if (m_structuredClassifier.id() == id) {
        m_structuredClassifier.release();
    }
    if (m_dataType.id() == id) {
        m_dataType.release();
    }
    if (m_class.id() == id) {
        m_class.release();
    }
    if (m_artifact.id() == id) {
        m_artifact.release();
    }
    if (m_association.id() == id) {
        m_association.release();
    }
    if (m_owningAssociation.id() == id) {
        m_owningAssociation.release();
    }
    m_redefinedProperties.elementReleased(id);
}