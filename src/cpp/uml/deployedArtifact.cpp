#include "uml/deployedArtifact.h"
#include "uml/profile.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"

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