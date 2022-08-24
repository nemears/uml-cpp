#ifndef _UML_DEPLOYMENT_TARGET_H_
#define _UML_DEPLOYMENT_TARGET_H_

#include "namedElement.h"

namespace UML {

    class Deployment;

    class DeploymentTarget : virtual public NamedElement {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            Set<Deployment, DeploymentTarget> m_deployments = Set<Deployment, DeploymentTarget>(this);
            void init();
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