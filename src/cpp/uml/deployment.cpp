#include "uml/deployment.h"

using namespace UML;

Deployment::Deployment() {

}

Deployment::Deployment(const Deployment& deployment) {

}

Deployment::~Deployment() {

}

ElementType Deployment::getElementType() const {
    return ElementType::DEPLOYMENT;
}

bool Deployment::isSubClassOf(ElementType eType) const {
    bool ret = Dependency::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::DEPLOYMENT;
    }

    return ret;
}