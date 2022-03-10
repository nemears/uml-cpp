#include "uml/model.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"

using namespace UML;

Model::Model() : Element(ElementType::MODEL) {
    
}

Model::~Model() {

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