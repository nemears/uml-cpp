#include "uml/uml-stable.h"

using namespace UML;

Model::Model() : Element(ElementType::MODEL) {
    
}

Model::~Model() {

}

std::string Model::getViewpoint() const {
    return m_viewpoint;
}

void Model::setViewpoint(const std::string& viewpoint) {
    m_viewpoint = viewpoint;
}

bool Model::is(ElementType eType) const {
    bool ret = Package::is(eType);

    if (!ret) {
        ret = eType == ElementType::MODEL;
    }

    return ret;
}
