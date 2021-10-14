#include "uml/operation.h"
#include "uml/class.h"
#include "uml/dataType.h"
#include "uml/artifact.h"

using namespace std;
using namespace UML;

void Operation::RemoveTypeProcedure::operator()(Type* el) const {
    el->removeReference(m_me->getID());
}

void Operation::AddTypeProcedure::operator()(Type* el) const {
    el->setReference(m_me);
}

void Operation::RemoveClassProcedure::operator()(Class* el) const {
    if (el->getOwnedOperations().count(m_me->getID())) {
        el->getOwnedOperations().remove(*m_me);
    }
    if (m_me->m_featuringClassifier.id() == el->getID()) {
        m_me->m_featuringClassifier.set(0);
    }
}

void Operation::AddClassProcedure::operator()(Class* el) const {
    if (!el->getOwnedOperations().count(m_me->getID())) {
        el->getOwnedOperations().add(*m_me);
    }
    if (m_me->m_featuringClassifier.id() != el->getID()) {
        m_me->m_featuringClassifier.set(el);
    }
}

void Operation::AddClassProcedure::operator()(ID id) const {
    if (m_me->getFeaturingClassifierID() != id) {
        m_me->m_featuringClassifier.setByID(id);
    }
}

void Operation::RemoveDataTypeProcedure::operator()(DataType* el) const {
    if (el->getOwnedOperation().count(m_me->getID())) {
        el->getOwnedOperation().remove(*m_me);
    }
    if (m_me->m_featuringClassifier.id() == el->getID()) {
        m_me->m_featuringClassifier.set(0);
    }
}

void Operation::AddDataTypeProcedure::operator()(DataType* el) const {
    if (!el->getOwnedOperation().count(m_me->getID())) {
        el->getOwnedOperation().add(*m_me);
    }
    if (m_me->m_featuringClassifier.id() != el->getID()) {
        m_me->m_featuringClassifier.set(el);
    }
}

void Operation::AddDataTypeProcedure::operator()(ID id) const {
    if (m_me->m_featuringClassifier.id() != id) {
        m_me->m_featuringClassifier.setByID(id);
    }
}

void Operation::RemoveArtifactProcedure::operator()(Artifact* el) const {
    if (el->getOwnedOperations().count(m_me->getID())) {
        el->getOwnedOperations().remove(*m_me);
    }
    if (m_me->m_featuringClassifier.id() == el->getID()) {
        m_me->m_featuringClassifier.set(0);
    }
}

void Operation::AddArtifactProcedure::operator()(Artifact* el) const {
    if (!el->getOwnedOperations().count(m_me->getID())) {
        el->getOwnedOperations().add(*m_me);
    }
    if (m_me->m_featuringClassifier.id() != el->getID()) {
        m_me->m_featuringClassifier.set(el);
    }
}

void Operation::AddArtifactProcedure::operator()(ID id) const {
    if (m_me->getFeaturingClassifierID() != id) {
        m_me->m_featuringClassifier.setByID(id);
    }
}

void Operation::reindexName(string oldName, string newName) {
    if (getClass()) {
        getClass()->getOwnedOperations().reindex(m_id, oldName, newName);
    }
    if (getDataType()) {
        getDataType()->getOwnedOperation().reindex(m_id, oldName, newName);
    }
    Feature::reindexName(oldName, newName);
}

void Operation::referencingReleased(ID id) {
    BehavioralFeature::referencingReleased(id);
    m_type.release(id);
    m_class.release(id);
    m_dataType.release(id);
    m_artifact.release(id);
}

void Operation::referenceReindexed(ID oldID, ID newID) {
    BehavioralFeature::referenceReindexed(oldID, newID);
    m_type.reindex(oldID, newID);
    m_class.reindex(oldID, newID);
    m_dataType.reindex(oldID, newID);
    m_artifact.reindex(oldID, newID);
}

void Operation::restoreReferences() {
    BehavioralFeature::restoreReferences();
    m_type.restoreReference();
    m_class.restoreReference();
    m_dataType.restoreReference();
    m_artifact.restoreReference();
}

void Operation::referenceErased(ID id) {
    BehavioralFeature::referenceErased(id);
    m_type.elementErased(id);
    m_class.elementErased(id);
    m_dataType.elementErased(id);
    m_artifact.elementErased(id);
}

Operation::Operation() {
    m_type.m_signature = &Operation::m_type;
    m_type.m_removeProcedures.push_back(new RemoveTypeProcedure(this));
    m_type.m_addProcedures.push_back(new AddTypeProcedure(this));
    m_class.m_signature = &Operation::m_class;
    m_class.m_removeProcedures.push_back(new RemoveClassProcedure(this));
    m_class.m_addProcedures.push_back(new AddClassProcedure(this));
    m_dataType.m_signature = &Operation::m_dataType;
    m_dataType.m_removeProcedures.push_back(new RemoveDataTypeProcedure(this));
    m_dataType.m_addProcedures.push_back(new AddDataTypeProcedure(this));
    m_artifact.m_signature = &Operation::m_artifact;
    m_artifact.m_removeProcedures.push_back(new RemoveArtifactProcedure(this));
    m_artifact.m_addProcedures.push_back(new AddArtifactProcedure(this));
}

Operation::Operation(const Operation& op) : BehavioralFeature(op), TemplateableElement(op), NamedElement(op), Element(op) {
    m_type = op.m_type;
    m_type.m_me = this;
    m_type.m_removeProcedures.clear();
    m_type.m_addProcedures.clear();
    m_type.m_removeProcedures.push_back(new RemoveTypeProcedure(this));
    m_type.m_addProcedures.push_back(new AddTypeProcedure(this));
    m_class = op.m_class;
    m_class.m_me = this;
    m_class.m_removeProcedures.clear();
    m_class.m_addProcedures.clear();
    m_class.m_removeProcedures.push_back(new RemoveClassProcedure(this));
    m_class.m_addProcedures.push_back(new AddClassProcedure(this));
    m_dataType = op.m_dataType;
    m_dataType.m_me = this;
    m_dataType.m_removeProcedures.clear();
    m_dataType.m_addProcedures.clear();
    m_dataType.m_removeProcedures.push_back(new RemoveDataTypeProcedure(this));
    m_dataType.m_addProcedures.push_back(new AddDataTypeProcedure(this));
    m_artifact = op.m_artifact;
    m_artifact.m_me = this;
    m_artifact.m_removeProcedures.clear();
    m_artifact.m_addProcedures.clear();
    m_artifact.m_removeProcedures.push_back(new RemoveArtifactProcedure(this));
    m_artifact.m_addProcedures.push_back(new AddArtifactProcedure(this));
}

Operation::~Operation() {

}

Type* Operation::getType() {
    return m_type.get();
}

Type& Operation::getTypeRef() {
    return m_type.getRef();
}

bool Operation::hasType() const {
    return m_type.has();
}

void Operation::setType(Type* type) {
    m_type.set(type);
}

void Operation::setType(Type& type) {
    m_type.set(type);
}

Class* Operation::getClass() {
    return m_class.get();
}

Class& Operation::getClassRef() {
    return m_class.getRef();
}

ID Operation::getClassID() const {
    return m_class.id();
}

bool Operation::hasClass() const {
    return m_class.has();
}

void Operation::setClass(Class* clazz) {
    m_class.set(clazz);
}

void Operation::setClass(Class& clazz) {
    m_class.set(clazz);
}

DataType* Operation::getDataType() {
    return m_dataType.get();
}

DataType& Operation::getDataTypeRef() {
    return m_dataType.getRef();
}

ID Operation::getDataTypeID() const {
    return m_dataType.id();
}

bool Operation::hasDataType() const {
    return m_dataType.has();
}

void Operation::setDataType(DataType* dataType) {
    m_dataType.set(dataType);
}

void Operation::setDataType(DataType& dataType) {
    m_dataType.set(dataType);
}

Artifact* Operation::getArtifact() {
    return m_artifact.get();
}

Artifact& Operation::getArtifactRef() {
    return m_artifact.getRef();
}

ID Operation::getArtifactID() const {
    return m_artifact.id();
}

bool Operation::hasArtifact() const {
    return m_artifact.has();
}

void Operation::setArtifact(Artifact* artifact) {
    m_artifact.set(artifact);
}

void Operation::setArtifact(Artifact& artifact) {
    m_artifact.set(artifact);
}

ElementType Operation::getElementType() const {
    return ElementType::OPERATION;
}

bool Operation::isSubClassOf(ElementType eType) const {
    bool ret = BehavioralFeature::isSubClassOf(eType);

    if (!ret) {
        ret = TemplateableElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = ParameterableElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::OPERATION;
    }

    return ret;
}