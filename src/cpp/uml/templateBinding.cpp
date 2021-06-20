#include "uml/templateBinding.h"

using namespace UML;

TemplateBinding::TemplateBinding() {

}

TemplateBinding::TemplateBinding(const TemplateBinding& bind) {

}

TemplateBinding::~TemplateBinding() {

}

ElementType TemplateBinding::getElementType() const {
    return ElementType::TEMPLATE_BINDING;
}

bool TemplateBinding::isSubClassOf(ElementType eType) const {
    bool ret = DirectedRelationship::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TEMPLATE_BINDING;
    }

    return ret;
}