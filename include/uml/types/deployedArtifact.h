#pragma once

#include "namedElement.h"

namespace UML {
    class DeployedArtifact : virtual public NamedElement {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            DeployedArtifact();
        public:
            virtual ~DeployedArtifact();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DEPLOYED_ARTIFACT;
            };
    };
}
