#include "uml/operation.h"
#include "uml/class.h"
#include "uml/dataType.h"

using namespace std;
using namespace UML;

void Operation::reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) {
    if (m_class) {
        m_class->getOperations().reindex(oldID, newID);
    }
    if (m_dataType) {
        m_dataType->getOwnedOperation().reindex(oldID, newID);
    }
    Feature::reindexID(oldID, newID);
}

void Operation::reindexName(string oldName, string newName) {
    if (m_class) {
        m_class->getOperations().reindex(m_id, oldName, newName);
    }
    if (m_dataType) {
        m_dataType->getOwnedOperation().reindex(m_id, oldName, newName);
    }
    Feature::reindexName(oldName, newName);
}

Operation::Operation() {
    m_type = 0;
    m_class = 0;
    m_dataType = 0;
}

Type* Operation::getType() {
    return m_type;
}

void Operation::setType(Type* type) {
    m_type = type;
}

Class* Operation::getClass() {
    return m_class;
}

void Operation::setClass(Class* clazz) {
    if (m_class) {
        if(m_class->getOperations().count(m_id)) {
            m_class->getOperations().remove(*this);
        }
    }
    m_class = clazz;
    if (m_class) {
        if (!m_class->getOperations().count(m_id)) {
            m_class->getOperations().add(*this);
        }
        if (m_featuringClassifier != clazz) {
            setFeaturingClassifier(clazz);
        }
    }
}

DataType* Operation::getDataType() {
    return m_dataType;
}

void Operation::setDataType(DataType* dataType) {
    if (m_dataType) {
        if (m_dataType->getOwnedOperation().count(m_id)) {
            m_dataType->getOwnedOperation().remove(*this);
        }
    }
    m_dataType = dataType;
    if (m_dataType) {
        if (!m_dataType->getOwnedOperation().count(m_id)) {
            m_dataType->getOwnedOperation().add(*this);
        }
    }
}

ElementType Operation::getElementType() const {
    return ElementType::OPERATION;
}

bool Operation::isSubClassOf(ElementType eType) {
    bool ret = BehavioralFeature::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::OPERATION;
    }

    return ret;
}