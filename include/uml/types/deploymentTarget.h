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
    struct ElementInfo<DeploymentTarget> : public DefaultInfo {
        static const bool abstract = true;
        static constexpr std::string_view name {"DeploymentTarget"};
        static SetList sets(DeploymentTarget& el) {
            return SetList{
                makeSetPair("deployments", el.m_deployments)
            };
        }
    };
}
