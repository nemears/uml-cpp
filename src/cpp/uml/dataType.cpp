#include "uml/dataType.h"
#include "uml/operation.h"
#include "uml/property.h"

using namespace UML;

void DataType::setManager(UmlManager* manager) {
    Classifier::setManager(manager);
    m_ownedAttribute.m_manager = manager;
    m_ownedOperation.m_manager = manager;
}

void DataType::AddOwnedAttributeFunctor::operator()(Property& el) const {
    if (!m_el->getAttributes().count(el.getID())) {
        m_el->getAttributes().add(el);
    }

    if (!m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().add(el);
    }

    if (el.getDataType() != m_el) {
        el.setDataType(m_el);
    }
    updateCopiedSequenceAddedTo(el, &DataType::getOwnedAttribute);
}

void DataType::AddOwnedAttributeFunctor::operator()(ID id) const {
    if (!m_el->getAttributes().count(id)) {
        m_el->getAttributes().addByID(id);
    }
    
    if (!m_el->getOwnedMembers().count(id)) {
        m_el->getOwnedMembers().addByID(id);
    }
}

void DataType::RemoveOwnedAttributeFunctor::operator()(Property& el) const {
    if (m_el->getAttributes().count(el.getID())) {
        m_el->getAttributes().remove(el);
    }

    if (m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().remove(el);
    }

    if (el.getDataType() == m_el) {
        el.setDataType(0);
    }
    updateCopiedSequenceRemovedFrom(el, &DataType::getOwnedAttribute);
}

void DataType::AddOwnedOperationFunctor::operator()(Operation& el) const {
    if (!m_el->getFeatures().count(el.getID())) {
        m_el->getFeatures().add(el);
    }

    if (!m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().add(el);
    }

    if (el.getDataType() != m_el) {
        el.setDataType(m_el);
    }

    if (!el.getRedefinitionContext().count(m_el->getID())) {
        el.getRedefinitionContext().add(*m_el);
    }
    updateCopiedSequenceAddedTo(el, &DataType::getOwnedOperation);
}

void DataType::AddOwnedOperationFunctor::operator()(ID id) const {
    if (!m_el->getFeatures().count(id)) {
        m_el->getFeatures().addByID(id);
    }
    if (!m_el->getOwnedMembers().count(id)) {
        m_el->getOwnedMembers().addByID(id);
    }
}

void DataType::RemoveOwnedOperationFunctor::operator()(Operation& el) const {
    if (m_el->getFeatures().count(el.getID())) {
        m_el->getFeatures().remove(el);
    }

    if (m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().remove(el);
    }

    if (el.getDataType() == m_el) {
        el.setDataType(0);
    }

    if (el.getRedefinitionContext().count(m_el->getID())) {
        el.getRedefinitionContext().remove(*m_el);
    }
    updateCopiedSequenceRemovedFrom(el, &DataType::getOwnedOperation);
}

void DataType::referenceReindexed(ID oldID, ID newID) {
    Classifier::referenceReindexed(oldID, newID);
    if (m_ownedAttribute.count(oldID)) {
        m_ownedAttribute.reindex(oldID, newID, &DataType::getOwnedAttribute);
    }
    if (m_ownedOperation.count(oldID)) {
        m_ownedOperation.reindex(oldID, newID, &DataType::getOwnedOperation);
    }
}

void DataType::referencingReleased(ID id) {
    Classifier::referencingReleased(id);
    if (m_ownedAttribute.count(id)) {
        m_ownedAttribute.elementReleased(id, &DataType::getOwnedAttribute);
    }
    if (m_ownedOperation.count(id)) {
        m_ownedOperation.elementReleased(id, &DataType::getOwnedOperation);
    }
}

DataType::DataType() {
    m_ownedAttribute.addProcedures.push_back(new AddOwnedAttributeFunctor(this));
    m_ownedAttribute.removeProcedures.push_back(new RemoveOwnedAttributeFunctor(this));
    m_ownedOperation.addProcedures.push_back(new AddOwnedOperationFunctor(this));
    m_ownedOperation.removeProcedures.push_back(new RemoveOwnedOperationFunctor(this));
}

DataType::~DataType() {
    
}

DataType::DataType(const DataType& el) : Classifier(el), PackageableElement(el), NamedElement(el), Element(el){
    m_ownedAttribute = el.m_ownedAttribute;
    m_ownedAttribute.m_el = this;
    m_ownedAttribute.addProcedures.clear();
    m_ownedAttribute.addProcedures.push_back(new AddOwnedAttributeFunctor(this));
    m_ownedAttribute.removeProcedures.clear();
    m_ownedAttribute.removeProcedures.push_back(new RemoveOwnedAttributeFunctor(this));
    m_ownedOperation = el.m_ownedOperation;
    m_ownedOperation.m_el = this;
    m_ownedOperation.addProcedures.clear();
    m_ownedOperation.addProcedures.push_back(new AddOwnedOperationFunctor(this));
    m_ownedOperation.removeProcedures.clear();
    m_ownedOperation.removeProcedures.push_back(new RemoveOwnedOperationFunctor(this));
}

Sequence<Property>& DataType::getOwnedAttribute() {
    return m_ownedAttribute;
}

Sequence<Operation>& DataType::getOwnedOperation() {
    return m_ownedOperation;
}

ElementType DataType::getElementType() const {
    return ElementType::DATA_TYPE;
}

bool DataType::isSubClassOf(ElementType eType) const {
    bool ret = Classifier::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::DATA_TYPE;
    }

    return ret;
}