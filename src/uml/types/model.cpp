#include "uml/types/model.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/deployment.h"

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

bool Model::isSubClassOf(ElementType eType) const {
    bool ret = Package::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::MODEL;
    }

    return ret;
}