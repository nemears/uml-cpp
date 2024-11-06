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
    struct ElementInfo<DeployedArtifact> : public DefaultInfo {
        static const bool abstract = true;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "DeployedArtifact"; }
        static SetList sets(__attribute__((unused)) DeployedArtifact& el) {
            return SetList {};
        }
    };
}
