#ifndef DEPLOYED_ARTIFACT_H
#define DEPLOYED_ARTIFACT_H

#include "namedElement.h"

namespace UML {
    class DeployedArtifact : virtual public NamedElement {

        friend class UmlManager;

        protected:
            DeployedArtifact();
        public:
            DeployedArtifact(const DeployedArtifact& deployedArtifact);
            virtual ~DeployedArtifact();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DEPLOYED_ARTIFACT;
            };
    };
}

#endif