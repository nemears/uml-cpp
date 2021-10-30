#include "uml/deployedArtifact.h"

using namespace UML;

DeployedArtifact::DeployedArtifact() : Element(ElementType::DEPLOYED_ARTIFACT) {
    
}

DeployedArtifact::DeployedArtifact(const DeployedArtifact& deployedArtifact) : Element(deployedArtifact, ElementType::DEPLOYED_ARTIFACT) {

}

DeployedArtifact::~DeployedArtifact() {

}

bool DeployedArtifact::isSubClassOf(ElementType eType) const {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::DEPLOYED_ARTIFACT;
    }

    return ret;
}