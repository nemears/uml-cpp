#ifndef DEPLOYMENT_H
#define DEPLOYMENT_H

#include "dependency.h"

namespace UML {

    class DeployedArtifact;
    class DeploymentTarget;

    class Deployment : public Dependency {

        friend class UmlManager;

        private:
            Sequence<DeployedArtifact> m_deployedArtifacts = Sequence<DeployedArtifact>(this);
            Singleton<DeploymentTarget, Deployment> m_location = Singleton<DeploymentTarget, Deployment>(this);
            class RemoveLocationProcedure : public AbstractSingletonProcedure<DeploymentTarget, Deployment> {
                public:
                    RemoveLocationProcedure(Deployment* me) : AbstractSingletonProcedure<DeploymentTarget, Deployment>(me) {};
                    void operator()(DeploymentTarget* el) const override;
            };
            class AddLocationProcedure : public AbstractSingletonProcedure<DeploymentTarget, Deployment> {
                public:
                    AddLocationProcedure(Deployment* me) : AbstractSingletonProcedure<DeploymentTarget, Deployment>(me) {};
                    void operator()(DeploymentTarget* el) const override;
            };
            class AddDeployedArtifactFunctor : public TemplateAbstractSequenceFunctor<DeployedArtifact,Deployment> {
                public:
                    AddDeployedArtifactFunctor(Deployment* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(DeployedArtifact& el) const override;
            };
            class RemoveDeployedArtifactFunctor : public TemplateAbstractSequenceFunctor<DeployedArtifact,Deployment> {
                public:
                    RemoveDeployedArtifactFunctor(Deployment* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(DeployedArtifact& el) const override;
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            void restoreReferences() override;
            Deployment();
        public:
            Deployment(const Deployment& deployment);
            virtual ~Deployment();
            Sequence<DeployedArtifact>& getDeployedArtifact();
            DeploymentTarget* getLocation();
            DeploymentTarget& getLocationRef();
            ID getLocationID() const;
            bool hasLocation() const;
            void setLocation(DeploymentTarget& location);
            void setLocation(DeploymentTarget* location);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DEPLOYMENT;
            };
    };
}

#endif