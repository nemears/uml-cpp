#include "uml/types/deployedArtifact.h"
#include "uml/types/profile.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/deployment.h"

using namespace UML;

DeployedArtifact::DeployedArtifact() : Element(ElementType::DEPLOYED_ARTIFACT) {
    
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