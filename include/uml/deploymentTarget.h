#ifndef DEPLOYMENT_TARGET_H
#define DEPLOYMENT_TARGET_H

#include "namedElement.h"
#include "sequence.h"

namespace UML {

    class Deployment;

    class DeploymentTarget : virtual public NamedElement {
        friend class UmlManager;
        private:
            Sequence<Deployment> m_deployments;
            class AddDeploymentFunctor : public AbstractSequenceFunctor {
                public:
                    AddDeploymentFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveDeploymentFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveDeploymentFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            void setManager(UmlManager* manager) override;
        public:
            DeploymentTarget();
            DeploymentTarget(const DeploymentTarget& deploymentTarget);
            virtual ~DeploymentTarget();
            Sequence<Deployment>& getDeployments();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DEPLOYMENT_TARGET;
            };
    };
}

#endif