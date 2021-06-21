#include "uml/templateParameterSubstitution.h"

using namespace UML;

TemplateParameterSubstitution::TemplateParameterSubstitution() {

}

TemplateParameterSubstitution::TemplateParameterSubstitution(const TemplateParameterSubstitution& temp) {

}

TemplateParameterSubstitution::~TemplateParameterSubstitution() {

}

ElementType TemplateParameterSubstitution::getElementType() const {
    return ElementType::TEMPLATE_PARAMETER_SUBSTITUTION;
}

bool TemplateParameterSubstitution::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = ElementType::TEMPLATE_PARAMETER_SUBSTITUTION;
    }

    return ret;
}