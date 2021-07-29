#include "uml/operation.h"
#include "uml/class.h"
#include "uml/dataType.h"
#include "uml/artifact.h"

using namespace std;
using namespace UML;

void Operation::reindexID(ID oldID, ID newID) {
    if (!m_classID.isNull()) {
        if (!m_classPtr) {
            m_classPtr = &m_manager->get<Class>(m_classID);
        }
        m_classPtr->getOperations().reindex(oldID, newID);
    }
    if (!m_dataTypeID.isNull()) {
        if (!m_dataTypePtr) {
            m_dataTypePtr = &m_manager->get<DataType>(m_dataTypeID);
        }
        m_dataTypePtr->getOwnedOperation().reindex(oldID, newID);
    }
    Feature::reindexID(oldID, newID);
}

void Operation::reindexName(string oldName, string newName) {
    if (getClass()) {
        getClass()->getOperations().reindex(m_id, oldName, newName);
    }
    if (getDataType()) {
        getDataType()->getOwnedOperation().reindex(m_id, oldName, newName);
    }
    Feature::reindexName(oldName, newName);
}

Operation::Operation() {
    m_typePtr = 0;
    m_classPtr = 0;
    m_dataTypePtr = 0;
    m_artifactPtr = 0;
}

Operation::Operation(const Operation& op) : BehavioralFeature(op), TemplateableElement(op), NamedElement(op), Element(op) {
    m_typeID = op.m_typeID;
    m_typePtr = op.m_typePtr;
    m_classID = op.m_classID;
    m_classPtr = op.m_classPtr;
    m_dataTypeID = op.m_dataTypeID;
    m_dataTypePtr = op.m_dataTypePtr;
    m_artifactID = op.m_artifactID;
}

Operation::~Operation() {

}

Type* Operation::getType() {
    if (!m_typeID.isNull()) {
        if (!m_typePtr) {
            m_typePtr = &m_manager->get<Type>(m_typeID);
        }
        return m_typePtr;
    }
    return 0;
}

void Operation::setType(Type* type) {
    if (type) {
        m_typeID = type->getID();
    }
    if (!m_manager) {
        m_typePtr = type;
    }
}

Class* Operation::getClass() {
    if (!m_classID.isNull()) {
        if (!m_classPtr) {
            m_classPtr = &m_manager->get<Class>(m_classID);
        }
        return m_classPtr;
    }
    return 0;
}

void Operation::setClass(Class* clazz) {
    if (!m_classID.isNull()) {
        if (!m_classPtr) {
            m_classPtr = &m_manager->get<Class>(m_classID);
        }
        if (m_classPtr->getOperations().count(m_id)) {
            m_classPtr->getOperations().remove(*this);
        }
        m_classPtr = 0;
        m_classID = ID::nullID();
    }

    if (clazz) {
        m_classID = clazz->getID();
    }

    if (!m_manager) {
        m_classPtr = clazz;
    }

    if (clazz) {
        if (!clazz->getOperations().count(m_id)) {
            clazz->getOperations().add(*this);
        }
        if (m_featuringClassifierID != clazz->getID()) {
            setFeaturingClassifier(clazz);
        }
    }
}

DataType* Operation::getDataType() {
    if (!m_dataTypeID.isNull()) {
        if (!m_dataTypePtr) {
            m_dataTypePtr = &m_manager->get<DataType>(m_dataTypeID);
        }
        return m_dataTypePtr;
    }
    return 0;
}

void Operation::setDataType(DataType* dataType) {
    if (!m_dataTypeID.isNull()) {
        if (!m_dataTypePtr) {
            m_dataTypePtr = &m_manager->get<DataType>(m_dataTypeID);
        }
        if (m_dataTypePtr->getOwnedOperation().count(m_id)) {
            m_dataTypePtr->getOwnedOperation().remove(*this);
        }
        m_dataTypeID = ID::nullID();
        m_dataTypePtr = 0;
    }

    if (dataType) {
        m_dataTypeID = dataType->getID();
    }

    if (!m_manager) {
        m_dataTypePtr = dataType;
    }

    if (dataType) {
        if (!dataType->getOwnedOperation().count(m_id)) {
            dataType->getOwnedOperation().add(*this);
        }
    }
}

Artifact* Operation::getArtifact() {
    if (!m_artifactID.isNull()) {
        if (!m_artifactPtr) {
            m_artifactPtr = &m_manager->get<Artifact>(m_artifactID);
        }
        return m_artifactPtr;
    }
    return 0;
}

void Operation::setArtifact(Artifact* artifact) {
    if (!isSameOrNull(m_artifactID, artifact)) {
        if (!m_artifactPtr) {
            m_artifactPtr = &m_manager->get<Artifact>(m_artifactID);
        }

        if (m_artifactPtr->getOwnedOperations().count(m_id)) {
            m_artifactPtr->getOwnedOperations().remove(*this);
        }

        m_artifactPtr = 0;
        m_artifactID = ID::nullID();
    }

    if (artifact) {
        m_artifactID = artifact->getID();
    }

    if (!m_manager) {
        m_artifactPtr = artifact;
    }

    if (artifact) {
        if (!artifact->getOwnedOperations().count(m_id)) {
            artifact->getOwnedOperations().add(*this);
        }
    }
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