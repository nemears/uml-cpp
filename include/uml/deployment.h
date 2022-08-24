#ifndef _UML_DEPLOYMENT_H_
#define _UML_DEPLOYMENT_H_

#include "dependency.h"
#include "deployedArtifact.h"
#include "deploymentTarget.h"

namespace UML {

    class DeployedArtifact;
    class DeploymentTarget;

    typedef UmlPtr<DeploymentTarget> DeploymentTargetPtr;

    class Deployment : public Dependency {

        friend class DeploymentTarget;
        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        private:
            Set<DeployedArtifact, Deployment> m_deployedArtifacts = Set<DeployedArtifact, Deployment>(this);
            Singleton<DeploymentTarget, Deployment> m_location = Singleton<DeploymentTarget, Deployment>(this);
            Set<DeploymentTarget, Deployment>& getLocationSingleton();
            void init();
            Deployment();
        public:
            virtual ~Deployment();
            Set<DeployedArtifact, Deployment>& getDeployedArtifacts();
            DeploymentTargetPtr getLocation() const;
            void setLocation(DeploymentTarget& location);
            void setLocation(DeploymentTarget* location);
            void setLocation(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DEPLOYMENT;
            };
    };
}

#endif