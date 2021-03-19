#include "uml/property.h"
#include "uml/primitiveType.h"
#include "uml/classifier.h"

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

    if (type->isPrimitive()) {
        Type* valType = val->getType();
        if (!(valType->isPrimitive())) {
            throw invalidValueException;
        }
        if (((PrimitiveType*)val->getType())->getPrimitiveType() != ((PrimitiveType*) type)->getPrimitiveType()) {
            throw invalidValueException;
        }
    }

    // TODO check that val type is same as property type if not primitive

    defaultValue = val;
}

ValueSpecification* Property::getDefaultValue() {
    return defaultValue;
}

Classifier* Property::getClassifer() {
    return m_classifier;
}

void Property::setClassifier(Classifier* classifier) {
    m_classifier = classifier;
}

ElementType Property::getElementType() {
    return ElementType::PROPERTY;
}