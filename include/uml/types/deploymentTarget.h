#pragma once

#include "namedElement.h"
#include "uml/set/set.h"

namespace UML {

    class Deployment;

    class DeploymentTarget : virtual public NamedElement {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            Set<Deployment, DeploymentTarget> m_deployments = Set<Deployment, DeploymentTarget>(this);
            DeploymentTarget();
        public:
            virtual ~DeploymentTarget();
            Set<Deployment, DeploymentTarget>& getDeployments();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DEPLOYMENT_TARGET;
            };
    };
}
