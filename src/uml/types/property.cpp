#include "uml/types/property.h"
#include "uml/types/package.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
#include "uml/types/extension.h"
#include "uml/types/extensionEnd.h"
#include "uml/umlPtr.h"

using namespace UML;

void Property::SetPropertyTypePolicy::apply(Type& el, Property& me) {
    if (!me.getAssociation()) {
        return;
    }
    [[maybe_unused]] SetLock assocLock = me.lockEl(*me.getAssociation());
    me.getAssociation()->m_endTypes.innerAdd(el);
    me.getAssociation()->setReference(&el);
    if (!me.getAssociation()->isSubClassOf(ElementType::EXTENSION)) {
        return;
    }
    Extension& extension = me.getAssociation()->as<Extension>();
    if (extension.getMetaClass().id() == el.getID()) {
        return;
    }
    extension.m_metaClass.set(el.as<Class>());
}

void Property::RemovePropertyTypePolicy::apply(Type& el, Property& me) {
    if (!me.getAssociation()) {
        return;
    }
    if (me.getAssociation()->getEndTypes().contains(el.getID())) {
        [[maybe_unused]] SetLock assocLock = me.lockEl(*me.getAssociation());
        me.getAssociation()->m_endTypes.innerRemove(el.getID());
        me.getAssociation()->removeReference(el.getID());
    }
    if (!me.getAssociation()->isSubClassOf(ElementType::EXTENSION)) {
        return;
    }
    Extension& extension = me.getAssociation()->as<Extension>();
    if (extension.getMetaClass().id() == el.getID()) {
        extension.m_metaClass.set(0);
    }
}

void Property::AddRedefinedPropertyPolicy::apply(Property& el, Property& me) {
    if (me.getFeaturingClassifier() && !me.m_redefinitionContext.contains(me.getFeaturingClassifier().id())) {
        [[maybe_unused]] SetLock lock = me.lockEl(*me.getFeaturingClassifier());
        me.m_redefinitionContext.innerAdd(*me.getFeaturingClassifier());
    }
}

void Property::RemoveRedefinedPropertyPolicy::apply(Property& el, Property& me) {
    if (me.m_redefinedElement.empty() && me.getFeaturingClassifier() && !me.m_redefinitionContext.empty()) {
        [[maybe_unused]] SetLock lock = me.lockEl(*me.getFeaturingClassifier());
        me.m_redefinitionContext.innerRemove(me.getFeaturingClassifier().id());
    }
}

void Property::referenceReindexed(ID newID) {
    StructuralFeature::referenceReindexed(newID);
    ConnectableElement::referenceReindexed(newID);
    m_association.reindex(newID);
}

void Property::restoreReference(Element* el) {
    StructuralFeature::restoreReference(el);
    ConnectableElement::restoreReference(el);
    if (m_redefinedProperties.contains(el->getID())) {
        el->setReference(this);
        if (m_featuringClassifier.get() && !m_redefinitionContext.contains(m_featuringClassifier.get().id())) {
            [[maybe_unused]] SetLock lock = lockEl(*m_featuringClassifier.get());
            m_redefinitionContext.innerAdd(m_featuringClassifier.get().id());
        }
    }
}

void Property::restoreReferences() {
    StructuralFeature::restoreReferences();
    if (m_namespace->get() && !m_featuringClassifier.get()) {
        if (m_namespace->get()->isSubClassOf(ElementType::CLASSIFIER)) {
            Classifier& clazz = m_namespace->get()->as<Classifier>();
            if (clazz.getAttributes().contains(m_id)) {
                m_featuringClassifier.innerAdd(clazz);
            }
        }
    }
}

void Property::referenceErased(ID id) {
    StructuralFeature::referenceErased(id);
    DeploymentTarget::referenceErased(id);
    m_association.eraseElement(id);
}

TypedSet<ValueSpecification, Property>& Property::getDefaultValueSingleton() {
    return m_defaultValue;
}

TypedSet<Class, Property>& Property::getClassSingleton() {
    return m_class;
}

TypedSet<DataType, Property>& Property::getDataTypeSingleton() {
    return m_dataType;
}

TypedSet<Association, Property>& Property::getAssociationSingleton() {
    return m_association;
}

TypedSet<Association, Property>& Property::getOwningAssociationSingleton() {
    return m_owningAssociation;
}

TypedSet<Interface, Property>& Property::getInterfaceSingleton() {
    return m_interface;
}

Property::Property() : Element(ElementType::PROPERTY) {
    m_defaultValue.subsets(*m_ownedElements);
    m_class.subsets(*m_namespace);
    m_class.subsets(m_featuringClassifier);
    m_class.opposite(&Class::getOwnedAttributes);
    m_dataType.subsets(*m_namespace);
    m_dataType.subsets(m_featuringClassifier);
    m_dataType.opposite(&DataType::getOwnedAttributesSet);
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
        if (m_featuringClassifier.get() && m_featuringClassifier.get()->isSubClassOf(ElementType::STRUCTURED_CLASSIFIER)) {
            // TODO make this happen
            m_featuringClassifier.get()->as<StructuredClassifier>().m_parts.removeFromJustThisSet(m_id); // TODO test
        }
    }
    m_composite = composite;
    if (m_composite) {
        if (m_featuringClassifier.get() && m_featuringClassifier.get()->isSubClassOf(ElementType::STRUCTURED_CLASSIFIER)) {
            m_featuringClassifier.get()->as<StructuredClassifier>().m_parts.innerAdd(*this);
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