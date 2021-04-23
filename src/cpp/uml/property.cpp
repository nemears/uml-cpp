#include "uml/property.h"
#include "uml/primitiveType.h"
#include "uml/classifier.h"

using namespace std;
using namespace UML;

void Property::reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) {
    if (m_classifier) {
        m_classifier->getAttributes().reindex(oldID, newID);
    }
    NamedElement::reindexID(oldID, newID);
}

void Property::reindexName(string oldName, string newName) {
    if (m_classifier) {
        m_classifier->getAttributes().reindex(m_id, oldName, newName);
    }
    NamedElement::reindexName(oldName, newName);
}

void Property::setDefaultValue(ValueSpecification* val) {

    // TODO add checks to see if defaultValue corresponds with type
    if (type == NULL) {
        throw invalidValueException;
    }

    // TODO maybe delete these checks don't know if relevant
    if (type->isSubClassOf(ElementType::PRIMITIVE_TYPE)) {
        Type* valType = val->getType();
        if (!(valType->isSubClassOf(ElementType::PRIMITIVE_TYPE))) {
            throw invalidValueException;
        }
        if (((PrimitiveType*)val->getType())->getPrimitiveType() != ((PrimitiveType*) type)->getPrimitiveType()) {
            throw invalidValueException;
        }
    }

    // TODO check that val type is same as property type if not primitive

    defaultValue = val;
}

Property::Property(const Property& prop) : StructuralFeature(prop), NamedElement(prop), Element(prop) {
    defaultValue = prop.defaultValue;
    m_classifier = prop.m_classifier;
}

ValueSpecification* Property::getDefaultValue() {
    return defaultValue;
}

Classifier* Property::getClassifier() {
    return m_classifier;
}

void Property::setClassifier(Classifier* classifier) {
    if (m_classifier) {
        if (m_classifier->getAttributes().count(m_id)) {
            m_classifier->getAttributes().remove(*this);
        }
    }
    m_classifier = classifier;
    if (m_classifier) {
        if (!m_classifier->getAttributes().count(m_id)) {
            m_classifier->getAttributes().add(*this);
        }
    }
}

ElementType Property::getElementType() {
    return ElementType::PROPERTY;
}

bool Property::isSubClassOf(ElementType eType) {
    bool ret = StructuralFeature::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PROPERTY;
    }

    return ret;
}