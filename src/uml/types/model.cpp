#include "uml/uml-stable.h"

using namespace UML;

Model::Model(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    Package(elementType, manager)
{}

std::string Model::getViewpoint() const {
    return m_viewpoint;
}

void Model::setViewpoint(const std::string& viewpoint) {
    m_viewpoint = viewpoint;
}
