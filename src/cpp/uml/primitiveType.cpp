#include "uml/primitiveType.h"
#include "uml/uml-stable.h"

using namespace UML;

PrimitiveType::PrimitiveType() : Element(ElementType::PRIMITIVE_TYPE) {}

PrimitiveType::PrimitiveType(const PrimitiveType& rhs) : Element(rhs, ElementType::PRIMITIVE_TYPE) {
    init();
    Element::copy(rhs);
    NamedElement::copy(rhs);
    Namespace::copy(rhs);
    ParameterableElement::copy(rhs);
    PackageableElement::copy(rhs);
    TemplateableElement::copy(rhs);
    Classifier::copy(rhs);
    DataType::copy(rhs);
}

bool PrimitiveType::isSubClassOf(ElementType eType) const {
    bool ret = DataType::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PRIMITIVE_TYPE;
    }

    return ret;
}