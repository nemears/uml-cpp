#ifndef ARTIFACT_H
#define ARTIFACT_H

#include "classifier.h"
#include "deployedArtifact.h"

namespace UML {
    class Artifact : public Classifier, public DeployedArtifact {
        public:
            Artifact();
            Artifact(const Artifact& artifact);
            virtual ~Artifact();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ARTIFACT;
            };
    };
}

#endif