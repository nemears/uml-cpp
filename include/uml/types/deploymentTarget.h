#pragma once

#include "namedElement.h"
#include "uml/set/set.h"

namespace UML {

    class Deployment;

    class DeploymentTarget : virtual public NamedElement {

        friend struct ElementInfo<DeploymentTarget>;

        protected:
            Set<Deployment, DeploymentTarget> m_deployments = Set<Deployment, DeploymentTarget>(this);
            DeploymentTarget(std::size_t elementType, AbstractManager& manager);
        public:
            Set<Deployment, DeploymentTarget>& getDeployments();
            typedef TypeInfo<std::tuple<NamedElement>, DeploymentTarget> Info;
    };

    template <>
    struct ElementInfo<DeploymentTarget> {
        static const bool abstract = true;
        inline static const std::string name {"DeploymentTarget"};
        static SetList sets(DeploymentTarget& el) {
            return SetList{
                makeSetPair("deployments", el.m_deployments)
            };
        }
    };
}
