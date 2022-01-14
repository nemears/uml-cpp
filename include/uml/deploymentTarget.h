#ifndef DEPLOYMENT_TARGET_H
#define DEPLOYMENT_TARGET_H

#include "namedElement.h"

namespace UML {

    class Deployment;

    class DeploymentTarget : virtual public NamedElement {

        friend class UmlManager;

        protected:
            Set<Deployment, DeploymentTarget> m_deployments = Set<Deployment, DeploymentTarget>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            void init();
            void copy(const DeploymentTarget& rhs);
            DeploymentTarget();
        public:
            DeploymentTarget(const DeploymentTarget& rhs);
            virtual ~DeploymentTarget();
            Set<Deployment, DeploymentTarget>& getDeployments();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DEPLOYMENT_TARGET;
            };
    };
}

#endif