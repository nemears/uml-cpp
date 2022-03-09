#include "uml/property.h"
#include "uml/package.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

void Property::AddEndTypeFunctor::operator()(Element& el) const {
    if (m_el.as<Property>().getAssociation()) {
        m_el.as<Property>().getAssociation()->getEndType().add(el.as<Type>());
        el.setReference(m_el.as<Property>().getAssociation().ptr());
    }
}

void Property::RemoveEndTypeFunctor::operator()(Element& el) const {
    if (m_el.as<Property>().getAssociation()) {
        if (m_el.as<Property>().getAssociation()->getEndType().contains(el.getID())) {
            m_el.as<Property>().getAssociation()->getEndType().remove(el.getID());
            el.removeReference(m_el.as<Property>().getAssociation().id());
        }
    }
}

void Property::AddRedefinitionContextFunctor::operator()(Element& el) const {
    Property& me = m_el.as<Property>();
    if (me.getFeaturingClassifier() && !me.m_redefinitionContext.contains(me.getFeaturingClassifier().id())) {
        me.m_redefinitionContext.nonOppositeAdd(*me.getFeaturingClassifier());
    }
    el.setReference(&m_el);
}

void Property::RemoveRedefinitionContextFunctor::operator()(Element& el) const {
    Property& me = m_el.as<Property>();
    if (me.m_redefinedElement.empty() && me.getFeaturingClassifier() && !me.m_redefinitionContext.empty()) {
        me.m_redefinitionContext.nonOppositeRemove(me.getFeaturingClassifier().id());
    }
    el.removeReference(m_el.getID());
}

void Property::referencingReleased(ID id) {
    StructuralFeature::referencingReleased(id);
    ConnectableElement::referencingReleased(id);
    m_association.release(id);
}

void Property::referenceReindexed(ID oldID, ID newID) {
    StructuralFeature::referenceReindexed(oldID, newID);
    ConnectableElement::referenceReindexed(oldID, newID);
    m_association.reindex(oldID, newID);
}

void Property::reindexName(std::string oldName, std::string newName) {
    StructuralFeature::reindexName(oldName, newName);
    ConnectableElement::reindexName(oldName, newName);
    m_association.reindexName(oldName, newName);
}

void Property::restoreReference(Element* el) {
    StructuralFeature::restoreReference(el);
    ConnectableElement::restoreReference(el);
    if (m_redefinedProperties.contains(el->getID())) {
        el->setReference(this);
        if (m_featuringClassifier.get() && !m_redefinitionContext.contains(m_featuringClassifier.get().id())) {
            m_redefinitionContext.addReadOnly(m_featuringClassifier.get().id());
        }
    }
}

void Property::referenceErased(ID id) {
    StructuralFeature::referenceErased(id);
    DeploymentTarget::referenceErased(id);
    m_association.eraseElement(id);
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

Set<Interface, Property>& Property::getInterfaceSingleton() {
    return m_interface;
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
    m_interface.subsets(m_namespace);
    m_interface.subsets(m_featuringClassifier);
    m_interface.opposite(&Interface::getOwnedAttributesSet);
    m_interface.m_signature = &Property::getInterfaceSingleton;
    m_type.m_addFunctors.insert(new AddEndTypeFunctor(this));
    m_type.m_removeFunctors.insert(new RemoveEndTypeFunctor(this));
    m_redefinedProperties.subsets(m_redefinedElement);
    m_redefinedProperties.m_signature = &Property::getRedefinedProperties;
    m_redefinedProperties.m_addFunctors.insert(new AddRedefinitionContextFunctor(this));
    m_redefinedProperties.m_removeFunctors.insert(new RemoveRedefinitionContextFunctor(this));
}

Property::Property() : Element(ElementType::PROPERTY) {
    init();
}

Property::~Property() {
    mountAndRelease();
}

AggregationKind Property::getAggregation() {
    return m_aggregation;
}

bool Property::isComposite() {
    return m_composite;
}

void Property::setComposite(bool composite) {
    if (!composite && m_composite) {
        if (m_featuringClassifier.get() && m_featuringClassifier.get()->isSubClassOf(ElementType::STRUCTURED_CLASSIFIER)) {
            m_featuringClassifier.get()->as<StructuredClassifier>().m_parts.removeFromJustThisSet(m_id);
        }
    }
    m_composite = composite;
    if (m_composite) {
        if (m_featuringClassifier.get() && m_featuringClassifier.get()->isSubClassOf(ElementType::STRUCTURED_CLASSIFIER)) {
            m_featuringClassifier.get()->as<StructuredClassifier>().m_parts.nonOppositeAdd(*this);
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

ValueSpecificationPtr Property::getDefaultValue() const {
    return m_defaultValue.get();
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

DataTypePtr Property::getDataType() {
    return m_dataType.get();
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

ClassPtr Property::getClass() const {
    return m_class.get();
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

AssociationPtr Property::getAssociation() const {
    return m_association.get();
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

AssociationPtr Property::getOwningAssociation() const {
    return m_owningAssociation.get();
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

InterfacePtr Property::getInterface() const {
    return m_interface.get();
}

void Property::setInterface(Interface* interface) {
    m_interface.set(interface);
}

void Property::setInterface(Interface& interface) {
    m_interface.set(interface);
}

void Property::setInterface(ID id) {
    m_interface.set(id);
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