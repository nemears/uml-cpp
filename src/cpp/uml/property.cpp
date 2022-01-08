#include "uml/property.h"
#include "uml/class.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/uml-stable.h"

using namespace UML;

void Property::AddEndTypeFunctor::operator()(Element& el) const {
    if (m_el.as<Property>().hasAssociation()) {
        m_el.as<Property>().getAssociationRef().getEndType().add(el.as<Type>());
        el.setReference(m_el.as<Property>().getAssociation());
    }
}

void Property::RemoveEndTypeFunctor::operator()(Element& el) const {
    if (m_el.as<Property>().hasAssociation()) {
        if (m_el.as<Property>().getAssociationRef().getEndType().contains(el.getID())) {
            m_el.as<Property>().getAssociationRef().getEndType().remove(el.getID());
            el.removeReference(m_el.as<Property>().getAssociationID());
        }
    }
}

void Property::AddRedefinitionContextFunctor::operator()(Element& el) const {
    Property& me = m_el.as<Property>();
    if (me.hasFeaturingClassifier() && !me.m_redefinitionContext.contains(me.getFeaturingClassifierID())) {
        me.m_redefinitionContext.nonOppositeAdd(me.getFeaturingClassifierRef());
    }
    el.setReference(&m_el);
}

void Property::RemoveRedefinitionContextFunctor::operator()(Element& el) const {
    Property& me = m_el.as<Property>();
    if (me.m_redefinedElement.empty() && me.hasFeaturingClassifier() && !me.m_redefinitionContext.empty()) {
        me.m_redefinitionContext.nonOppositeRemove(me.getFeaturingClassifierID());
    }
    el.removeReference(m_el.getID());
}

void Property::referencingReleased(ID id) {
    StructuralFeature::referencingReleased(id);
    m_defaultValue.release(id);
    m_dataType.release(id);
    m_class.release(id);
    // m_artifact.release(id);
    m_association.release(id);
    m_owningAssociation.release(id);
    m_redefinedProperties.release(id);
}

void Property::referenceReindexed(ID oldID, ID newID) {
    StructuralFeature::referenceReindexed(oldID, newID);
    m_defaultValue.reindex(oldID, newID);
    m_dataType.reindex(oldID, newID);
    m_class.reindex(oldID, newID);
    // m_artifact.reindex(oldID, newID);
    m_association.reindex(oldID, newID);
    m_owningAssociation.reindex(oldID, newID);
    m_redefinedProperties.reindex(oldID, newID);
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
    m_defaultValue.eraseElement(id);
    m_dataType.eraseElement(id);
    m_class.eraseElement(id);
    m_association.eraseElement(id);
    m_owningAssociation.eraseElement(id);
    // m_artifact.elementErased(id);
    m_redefinedProperties.eraseElement(id);
}

Set<ValueSpecification, Property>& Property::getDefaultValueSingleton() {
    return m_defaultValue;
}

Set<Class, Property>& Property::getClassSingleton() {
    return m_class;
}

Set<DataType, Property>& Property::getDataTypeSingleton() {
    return m_dataType;
}

Set<Association, Property>& Property::getAssociationSingleton() {
    return m_association;
}

Set<Association, Property>& Property::getOwningAssociationSingleton() {
    return m_owningAssociation;
}

void Property::init() {
    m_defaultValue.subsets(*m_ownedElements);
    m_defaultValue.m_signature = &Property::getDefaultValueSingleton;
    m_class.subsets(m_namespace);
    m_class.subsets(m_featuringClassifier);
    m_class.opposite(&Class::getOwnedAttributesSet);
    m_class.m_signature = &Property::getClassSingleton;
    m_dataType.subsets(m_namespace);
    m_dataType.subsets(m_featuringClassifier);
    m_dataType.opposite(&DataType::getOwnedAttributesSet);
    m_dataType.m_signature = &Property::getDataTypeSingleton;
    m_association.opposite(&Association::getMemberEndsSet);
    m_association.m_signature = &Property::getAssociationSingleton;
    m_owningAssociation.subsets(m_namespace);
    m_owningAssociation.subsets(m_featuringClassifier);
    m_owningAssociation.subsets(m_association);
    m_owningAssociation.opposite(&Association::getOwnedEndsSet);
    m_owningAssociation.m_signature = &Property::getOwningAssociationSingleton;
    m_type.m_addFunctors.insert(new AddEndTypeFunctor(this));
    m_type.m_removeFunctors.insert(new RemoveEndTypeFunctor(this));
    m_redefinedProperties.subsets(m_redefinedElement);
    m_redefinedProperties.m_signature = &Property::getRedefinedProperties;
    m_redefinedProperties.m_addFunctors.insert(new AddRedefinitionContextFunctor(this));
    m_redefinedProperties.m_removeFunctors.insert(new RemoveRedefinitionContextFunctor(this));
}

void Property::copy(const Property& rhs) {
    m_defaultValue = rhs.m_defaultValue;
    m_class = rhs.m_class;
    m_dataType =  rhs.m_dataType;
    m_composite = rhs.m_composite;
    m_aggregation = rhs.m_aggregation;
    m_redefinedProperties = rhs.m_redefinedProperties;
}

Property::Property() : Element(ElementType::PROPERTY) {
    init();
}

Property::Property(const Property& rhs) : Element(rhs, ElementType::PROPERTY) {
    init();
    Element::copy(rhs);
    NamedElement::copy(rhs);
    TypedElement::copy(rhs);
    MultiplicityElement::copy(rhs);
    RedefinableElement::copy(rhs);
    Feature::copy(rhs);
    DeploymentTarget::copy(rhs);
    copy(rhs);
}

Property::~Property() {

}

AggregationKind Property::getAggregation() {
    return m_aggregation;
}

bool Property::isComposite() {
    return m_composite;
}

void Property::setComposite(bool composite) {
    if (!composite && m_composite) {
        if (m_featuringClassifier.has() && m_featuringClassifier.getRef().isSubClassOf(ElementType::STRUCTURED_CLASSIFIER)) {
            m_featuringClassifier.getRef().as<StructuredClassifier>().m_parts.removeFromJustThisSet(m_id);
        }
    }
    m_composite = composite;
    if (m_composite) {
        if (m_featuringClassifier.has() && m_featuringClassifier.getRef().isSubClassOf(ElementType::STRUCTURED_CLASSIFIER)) {
            m_featuringClassifier.getRef().as<StructuredClassifier>().m_parts.nonOppositeAdd(*this);
        }
    }
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

void Property::setDefaultValue(ID id) {
    m_defaultValue.set(id);
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

void Property::setDataType(ID id) {
    m_dataType.set(id);
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

void Property::setClass(ID id) {
    m_class.set(id);
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

void Property::setAssociation(ID id) {
    m_association.set(id);
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

void Property::setOwningAssociation(ID id) {
    m_owningAssociation.set(id);
}

Set<Property, Property>& Property::getRedefinedProperties() {
    return m_redefinedProperties;
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