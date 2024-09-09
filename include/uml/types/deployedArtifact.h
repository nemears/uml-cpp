#pragma once

#include "namedElement.h"
#include "uml/managers/abstractManager.h"

namespace UML {
    class DeployedArtifact : virtual public NamedElement {
        protected:
            DeployedArtifact(std::size_t elementType, AbstractManager& manager);
        public:
            typedef TypeInfo<std::tuple<NamedElement>, DeployedArtifact> Info;
    };

    template <>
    struct ElementInfo<DeployedArtifact> {
        static const bool abstract = true;
        inline static const std::string name {"DeployedArtifact"};
        static SetList sets(__attribute__((unused)) DeployedArtifact& el) {
            return SetList {};
        }
    };
}
