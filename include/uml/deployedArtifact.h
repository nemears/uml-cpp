#ifndef DEPLOYED_ARTIFACT_H
#define DEPLOYED_ARTIFACT_H

#include "namedElement.h"

namespace UML {
    class DeployedArtifact : public NamedElement {
        public:
            DeployedArtifact();
            DeployedArtifact(const DeployedArtifact& deployedArtifact);
            virtual ~DeployedArtifact();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DEPLOYED_ARTIFACT;
            };
    };
}

#endif