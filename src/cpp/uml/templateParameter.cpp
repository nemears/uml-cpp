#include "uml/templateParameter.h"

using namespace UML;

TemplateParameter::TemplateParameter() {

}

TemplateParameter::~TemplateParameter() {

}

ElementType TemplateParameter::getElementType() const {
    return ElementType::TEMPLATE_PARAMETER;
}

bool TemplateParameter::isSubClassOf(ElementType eType) {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TEMPLATE_PARAMETER;
    }

    return ret;
}