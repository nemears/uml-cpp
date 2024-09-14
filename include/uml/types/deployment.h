#pragma once

#include "dependency.h"
#include "uml/managers/abstractManager.h"
#include "uml/types/element.h"

namespace UML {

    class DeployedArtifact;
    class DeploymentTarget;

    typedef UmlPtr<DeploymentTarget> DeploymentTargetPtr;

    class Deployment : public Dependency {

        friend class DeploymentTarget;
        friend struct ElementInfo<Deployment>;

        protected:
            Set<DeployedArtifact, Deployment> m_deployedArtifacts = Set<DeployedArtifact, Deployment>(this);
            Singleton<DeploymentTarget, Deployment> m_location = Singleton<DeploymentTarget, Deployment>(this);
            Singleton<DeploymentTarget, Deployment>& getLocationSingleton();
        public:
            Deployment(std::size_t elementType, AbstractManager& manager);
            Set<DeployedArtifact, Deployment>& getDeployedArtifacts();
            DeploymentTargetPtr getLocation() const;
            void setLocation(DeploymentTarget& location);
            void setLocation(DeploymentTargetPtr location);
            void setLocation(ID id);
            typedef TypeInfo<std::tuple<Dependency>, Deployment> Info;
    };

    template <>
    struct ElementInfo<Deployment> : public DefaultInfo {
        static const bool abstract = false;
        inline static const std::string name {"Deployment"};
        static SetList sets(Deployment& el) {
            return SetList {
                makeSetPair("deployedArtifacts", el.m_deployedArtifacts),
                makeSetPair("location", el.m_location)
            };
        }
    };
}
