#include "uml/templateSignature.h"

using namespace UML;

TemplateSignature::~TemplateSignature() {

}

ElementType TemplateSignature::getElementType() const {
    return ElementType::TEMPLATE_SIGNATURE;
}

bool TemplateSignature::isSubClassOf(ElementType eType) {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        return eType == ElementType::TEMPLATE_SIGNATURE;
    }

    return ret;
}