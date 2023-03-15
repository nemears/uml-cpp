#ifndef _UML_DEPLOYMENT_TARGET_H_
#define _UML_DEPLOYMENT_TARGET_H_

#include "namedElement.h"
#include "set/set.h"

namespace UML {

    class Deployment;

    class DeploymentTarget : virtual public NamedElement {

        template <typename AccessPolicy, typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            CustomSet<Deployment, DeploymentTarget> m_deployments = CustomSet<Deployment, DeploymentTarget>(this);
            DeploymentTarget();
        public:
            virtual ~DeploymentTarget();
            Set<Deployment, DeploymentTarget>& getDeployments();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DEPLOYMENT_TARGET;
            };
    };
}

#endif