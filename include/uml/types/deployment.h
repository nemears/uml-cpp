#pragma once

#include "dependency.h"

namespace UML {

    class DeployedArtifact;
    class DeploymentTarget;

    typedef UmlPtr<DeploymentTarget> DeploymentTargetPtr;

    class Deployment : public Dependency {

        friend class DeploymentTarget;
        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        private:
            Set<DeployedArtifact, Deployment> m_deployedArtifacts = Set<DeployedArtifact, Deployment>(this);
            Singleton<DeploymentTarget, Deployment> m_location = Singleton<DeploymentTarget, Deployment>(this);
            Singleton<DeploymentTarget, Deployment>& getLocationSingleton();
            Deployment();
        public:
            virtual ~Deployment();
            Set<DeployedArtifact, Deployment>& getDeployedArtifacts();
            DeploymentTargetPtr getLocation() const;
            void setLocation(DeploymentTarget& location);
            void setLocation(DeploymentTargetPtr location);
            void setLocation(ID id);
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DEPLOYMENT;
            };
    };
}
