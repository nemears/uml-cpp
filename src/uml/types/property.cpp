#include "uml/types/structuredClassifier.h"
#include "uml/uml-stable.h"

using namespace UML;

void Property::PropertyTypePolicy::elementAdded(Type& el, Property& me) {
    if (!me.getAssociation()) {
        return;
    }
    me.getAssociation()->m_endTypes.innerAdd(ElementPtr(&el));
    if (!me.getAssociation()->is(ElementType::EXTENSION)) {
        return;
    }
    Extension& extension = me.getAssociation()->as<Extension>();
    if (extension.getMetaClass().id() == el.getID()) {
        return;
    }
    extension.m_metaClass.set(el.as<Class>());
}

void Property::PropertyTypePolicy::elementRemoved(Type& el, Property& me) {
    if (!me.getAssociation()) {
        return;
    }
    if (me.getAssociation()->getEndTypes().contains(el.getID())) {
        me.getAssociation()->m_endTypes.innerRemove(ElementPtr(&el));
    }
    if (!me.getAssociation()->is(ElementType::EXTENSION)) {
        return;
    }
    Extension& extension = me.getAssociation()->as<Extension>();
    if (extension.getMetaClass().id() == el.getID()) {
        extension.m_metaClass.set(0);
    }
}

void Property::RedefinedPropertyPolicy::elementAdded(Property& el, Property& me) {
    if (me.getFeaturingClassifier() && !me.m_redefinitionContext.contains(me.getFeaturingClassifier().id())) {
        me.m_redefinitionContext.innerAdd(me.getFeaturingClassifier());
    }
}

void Property::RedefinedPropertyPolicy::elementRemoved(Property& el, Property& me) {
    if (me.m_redefinedElement.empty() && me.getFeaturingClassifier() && !me.m_redefinitionContext.empty()) {
        me.m_redefinitionContext.innerRemove(me.getFeaturingClassifier());
    }
}

void Property::restoreReferences() {
    StructuralFeature::restoreReferences();
    if (m_namespace->get() && !m_featuringClassifier.get()) {
        if (m_namespace->get()->is(ElementType::CLASSIFIER)) {
            Classifier& clazz = m_namespace->get()->as<Classifier>();
            if (clazz.getAttributes().contains(m_id)) {
                m_featuringClassifier.innerAdd(ElementPtr(&clazz));
            }
        }
    }
    if (!m_redefinedProperties.empty()) {
        if (m_featuringClassifier.get() && !m_redefinitionContext.contains(m_featuringClassifier.get().id())) {
            m_redefinitionContext.innerAdd(m_featuringClassifier.get());
        }
    }
}

void Property::referenceErased(ID id) {
    StructuralFeature::referenceErased(id);
    DeploymentTarget::referenceErased(id);
    eraseFromSet(id, m_association);
    eraseFromSet(id, m_subsettedProperties);
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

Property::Property() : Element(ElementType::PROPERTY) {
    m_defaultValue.subsets(*m_ownedElements);
    m_class.subsets(*m_namespace);
    m_class.subsets(m_featuringClassifier);
    m_class.opposite(&Class::getOwnedAttributes);
    m_dataType.subsets(*m_namespace);
    m_dataType.subsets(m_featuringClassifier);
    m_dataType.opposite(&DataType::getOwnedAttributes);
    m_association.opposite(&Association::getMemberEnds);
    m_owningAssociation.subsets(*m_namespace);
    m_owningAssociation.subsets(m_featuringClassifier);
    m_owningAssociation.subsets(m_association);
    m_owningAssociation.opposite(&Association::getOwnedEnds);
    m_interface.subsets(*m_namespace);
    m_interface.subsets(m_featuringClassifier);
    m_interface.opposite(&Interface::getOwnedAttributes);
    m_redefinedProperties.subsets(m_redefinedElement);
    m_propertyType.redefines(m_type);
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
        if (m_featuringClassifier.get() && m_featuringClassifier.get()->is(ElementType::STRUCTURED_CLASSIFIER)) {
            // TODO test that removeFromJustThisSet function is safe
            //m_featuringClassifier.get()->as<StructuredClassifier>().m_parts.removeFromJustThisSet(m_id); // TODO test
            // remove and add again instead of removeFromJustThisSet (simpler)
            m_featuringClassifier.get()->as<StructuredClassifier>().m_parts.innerRemove(ElementPtr(this));
            m_featuringClassifier.get()->as<StructuredClassifier>().m_ownedAttributes.add(this);
        }
    }
    m_composite = composite;
    if (m_composite) {
        if (m_featuringClassifier.get() && m_featuringClassifier.get()->is(ElementType::STRUCTURED_CLASSIFIER)) {
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

void Property::setDefaultValue(ValueSpecification* val) {
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

void Property::setInterface(Interface* interface_uml) {
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

bool Property::is(ElementType eType) const {
    bool ret = StructuralFeature::is(eType);

    if (!ret) {
        ret = ConnectableElement::is(eType);
    }

    if (!ret) {
        ret = DeploymentTarget::is(eType);
    }

    if (!ret) {
        ret = eType == ElementType::PROPERTY;
    }

    return ret;
}
