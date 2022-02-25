#include "uml/model.h"
#include "uml/uml-stable.h"

using namespace UML;

Model::Model() : Element(ElementType::MODEL) {}

Model::Model(const Model& rhs) : Element(rhs, ElementType::MODEL) {
    init();
    Element::copy(rhs);
    NamedElement::copy(rhs);
    Namespace::copy(rhs);
    ParameterableElement::copy(rhs);
    PackageableElement::copy(rhs);
    TemplateableElement::copy(rhs);
    Package::copy(rhs);
    if (!m_copiedElementFlag) {
        delete &rhs;
    }
}

std::string Model::getViewpoint() const {
    return m_viewpoint;
}

void Model::setViewpoint(std::string viewpoint) {
    m_viewpoint = viewpoint;
}

bool Model::isSubClassOf(ElementType eType) const {
    bool ret = Package::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::MODEL;
    }

    return ret;
}