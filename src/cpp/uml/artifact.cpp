#include "uml/artifact.h"

using namespace UML;

Artifact::Artifact() {

}

Artifact::Artifact(const Artifact& artifact) {

}

Artifact::~Artifact() {

}

ElementType Artifact::getElementType() const {
    return ElementType::ARTIFACT;
}

bool Artifact::isSubClassOf(ElementType eType) const {
    bool ret = Classifier::isSubClassOf(eType);

    if (!ret) {
        ret = DeployedArtifact::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::ARTIFACT;
    }

    return ret;
}