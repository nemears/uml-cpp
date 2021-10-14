#ifndef DEPLOYMENT_TARGET_H
#define DEPLOYMENT_TARGET_H

#include "namedElement.h"
#include "sequence.h"

namespace UML {

    class Deployment;

    class DeploymentTarget : virtual public NamedElement {

        friend class UmlManager;

        protected:
            Sequence<Deployment> m_deployments = Sequence<Deployment>(this);
            class AddDeploymentFunctor : public TemplateAbstractSequenceFunctor<Deployment,DeploymentTarget> {
                public:
                    AddDeploymentFunctor(DeploymentTarget* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Deployment& el) const override;
            };
            class RemoveDeploymentFunctor : public TemplateAbstractSequenceFunctor<Deployment,DeploymentTarget> {
                public:
                    RemoveDeploymentFunctor(DeploymentTarget* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Deployment& el) const override;
            };
            void setManager(UmlManager* manager) override;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            DeploymentTarget();
        public:
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