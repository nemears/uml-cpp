#ifndef DEPLOYMENT_H
#define DEPLOYMENT_H

#include "dependency.h"

namespace UML {

    class DeployedArtifact;
    class DeploymentTarget;
    namespace Parsers {
        class SetLocation;
    }

    class Deployment : public Dependency {

        friend class UmlManager;
        friend class Parsers::SetLocation;
        friend class DeploymentTarget;

        private:
            Set<DeployedArtifact, Deployment> m_deployedArtifacts = Set<DeployedArtifact, Deployment>(this);
            Singleton<DeploymentTarget, Deployment> m_location = Singleton<DeploymentTarget, Deployment>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            void restoreReferences() override;
            Set<DeploymentTarget, Deployment>& getLocationSingleton();
            void init();
            void copy(const Deployment& rhs);
            Deployment();
        public:
            Deployment(const Deployment& rhs);
            virtual ~Deployment();
            Set<DeployedArtifact, Deployment>& getDeployedArtifacts();
            DeploymentTarget* getLocation();
            DeploymentTarget& getLocationRef();
            ID getLocationID() const;
            bool hasLocation() const;
            void setLocation(DeploymentTarget& location);
            void setLocation(DeploymentTarget* location);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DEPLOYMENT;
            };
    };
}

#endif