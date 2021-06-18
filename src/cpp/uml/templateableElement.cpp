#include "uml/templateableElement.h"

using namespace UML;

TemplateableElement::~TemplateableElement() {

}

ElementType TemplateableElement::getElementType() const {
    return ElementType::TEMPLATEABLE_ELEMENT;
}

bool TemplateableElement::isSubClassOf(ElementType eType) {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TEMPLATEABLE_ELEMENT;
    }

    return ret;
}