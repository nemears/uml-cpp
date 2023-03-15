#ifndef _UML_DEPLOYED_ARTIFACT_H_
#define _UML_DEPLOYED_ARTIFACT_H_

#include "namedElement.h"

namespace UML {
    class DeployedArtifact : virtual public NamedElement {

        template <typename AccessPolicy, typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            DeployedArtifact();
        public:
            virtual ~DeployedArtifact();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DEPLOYED_ARTIFACT;
            };
    };
}

#endif