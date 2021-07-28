#include "uml/deployedArtifact.h"

using namespace UML;

DeployedArtifact::DeployedArtifact() {

}

DeployedArtifact::DeployedArtifact(const DeployedArtifact& deployedArtifact) {

}

DeployedArtifact::~DeployedArtifact() {

}

ElementType DeployedArtifact::getElementType() const {
    return ElementType::DEPLOYED_ARTIFACT;
}

bool DeployedArtifact::isSubClassOf(ElementType eType) const {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::DEPLOYED_ARTIFACT;
    }

    return ret;
}