#include "uml/uml-stable.h"

using namespace UML;

DeployedArtifact::DeployedArtifact() : Element(ElementType::DEPLOYED_ARTIFACT) {
    
}

DeployedArtifact::~DeployedArtifact() {

}

bool DeployedArtifact::is(ElementType eType) const {
    bool ret = NamedElement::is(eType);

    if (!ret) {
        ret = eType == ElementType::DEPLOYED_ARTIFACT;
    }

    return ret;
}
