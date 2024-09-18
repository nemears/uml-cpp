#include "uml/types/connectableElement.h"
#include "uml/types/deploymentTarget.h"
#include "uml/types/parameterableElement.h"
#include "uml/types/structuredClassifier.h"
#include "uml/types/typedElement.h"
#include "uml/uml-stable.h"

using namespace UML;

void Property::PropertyTypePolicy::elementAdded(Type& el, Property& me) {
    if (!me.getAssociation()) {
        return;
    }
    auto& endTypes = me.getAssociation()->m_endTypes;
    if (!endTypes.contains(el)) {
        me.addToReadonlySet(me.getAssociation()->m_endTypes, el);
    }
    if (!me.getAssociation()->is<Extension>()) {
        return;
    }
    Extension& extension = me.getAssociation()->as<Extension>();
    if (extension.getMetaClass().id() == el.getID()) {
        return;
    }
    me.addToReadonlySet(extension.m_metaClass, el);
}

void Property::PropertyTypePolicy::elementRemoved(Type& el, Property& me) {
    if (!me.getAssociation()) {
        return;
    }
    if (me.getAssociation()->getEndTypes().contains(el.getID())) {
        me.removeFromReadonlySet(me.getAssociation()->m_endTypes, el);
    }
    if (!me.getAssociation()->is<Extension>()) {
        return;
    }
    Extension& extension = me.getAssociation()->as<Extension>();
    if (extension.getMetaClass().id() == el.getID()) {
        me.removeFromReadonlySet(extension.m_metaClass, el);
    }
}

void Property::RedefinedPropertyPolicy::elementAdded(__attribute__((unused)) Property& el, Property& me) {
    if (me.getFeaturingClassifier() && !me.m_redefinitionContext.contains(me.getFeaturingClassifier().id())) {
        me.addToReadonlySet(me.m_redefinitionContext, *me.getFeaturingClassifier());
    }
}

void Property::RedefinedPropertyPolicy::elementRemoved(__attribute__((unused)) Property& el, Property& me) {
    if (me.m_redefinedElement.empty() && me.getFeaturingClassifier() && !me.m_redefinitionContext.empty()) {
        me.removeFromReadonlySet(me.m_redefinitionContext, *me.getFeaturingClassifier());
    }
}

Singleton<ValueSpecification, Property>& Property::getDefaultValueSingleton() {
    return m_defaultValue;
}

Singleton<Class, Property>& Property::getClassSingleton() {
    return m_class;
}

Singleton<DataType, Property>& Property::getDataTypeSingleton() {
    return m_dataType;
}

Singleton<Association, Property>& Property::getAssociationSingleton() {
    return m_association;
}

Singleton<Association, Property>& Property::getOwningAssociationSingleton() {
    return m_owningAssociation;
}

Singleton<Interface, Property>& Property::getInterfaceSingleton() {
    return m_interface;
}

Property::Property(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    TypedElement(elementType, manager),
    RedefinableElement(elementType, manager),
    ParameterableElement(elementType, manager),
    StructuralFeature(elementType, manager),
    ConnectableElement(elementType, manager),
    DeploymentTarget(elementType, manager)
{
    m_defaultValue.subsets(m_ownedElements);
    m_class.subsets(m_namespace);
    m_class.subsets(m_featuringClassifier);
    m_class.opposite(&Class::getOwnedAttributes);
    m_dataType.subsets(m_namespace);
    m_dataType.subsets(m_featuringClassifier);
    m_dataType.opposite(&DataType::getOwnedAttributes);
    m_association.opposite(&Association::getMemberEnds);
    m_owningAssociation.subsets(m_namespace);
    m_owningAssociation.subsets(m_featuringClassifier);
    m_owningAssociation.subsets(m_association);
    m_owningAssociation.opposite(&Association::getOwnedEnds);
    m_interface.subsets(m_namespace);
    m_interface.subsets(m_featuringClassifier);
    m_interface.opposite(&Interface::getOwnedAttributes);
    m_redefinedProperties.subsets(m_redefinedElement);
    m_propertyType.redefines(m_type);
}

AggregationKind Property::getAggregation() {
    return m_aggregation;
}

bool Property::isComposite() {
    return m_composite;
}

void Property::setComposite(bool composite) {
    if (!composite && m_composite) {
        if (m_featuringClassifier.get() && m_featuringClassifier.get()->is<StructuredClassifier>()) {
            // TODO test that removeFromJustThisSet function is safe
            //m_featuringClassifier.get()->as<StructuredClassifier>().m_parts.removeFromJustThisSet(m_id); // TODO test
            // remove and add again instead of removeFromJustThisSet (simpler)
            StructuredClassifierPtr featuringClassifier = m_featuringClassifier.get();
            featuringClassifier->m_parts.innerRemove(this);
            m_composite = composite;
            featuringClassifier->m_ownedAttributes.add(this);
        }
    }
    m_composite = composite;
    if (m_composite) {
        if (m_featuringClassifier.get() && m_featuringClassifier.get()->is<StructuredClassifier>()) {
            m_featuringClassifier.get()->as<StructuredClassifier>().m_parts.innerAdd(this);
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

void Property::setDefaultValue(ValueSpecificationPtr val) {
    m_defaultValue.set(val);
}

void Property::setDefaultValue(ValueSpecification& val) {
    m_defaultValue.set(val);
}

void Property::setDefaultValue(ID id) {
    m_defaultValue.set(id);
}

DataTypePtr Property::getDataType() const {
    return m_dataType.get();
}

void Property::setDataType(DataTypePtr dataType) {
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

void Property::setClass(ClassPtr clazz) {
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

void Property::setAssociation(AssociationPtr association) {
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

void Property::setOwningAssociation(AssociationPtr association) {
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

void Property::setInterface(InterfacePtr interface_uml) {
    m_interface.set(interface_uml);
}

void Property::setInterface(Interface& interface_uml) {
    m_interface.set(interface_uml);
}

void Property::setInterface(ID id) {
    m_interface.set(id);
}

Set<Property, Property, Property::RedefinedPropertyPolicy>& Property::getRedefinedProperties() {
    return m_redefinedProperties;
}

Set<Property, Property>& Property::getSubsettedProperties() {
    return m_subsettedProperties;
}
