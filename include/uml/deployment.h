#ifndef DEPLOYMENT_H
#define DEPLOYMENT_H

#include "dependency.h"

namespace UML {

    class DeployedArtifact;
    class DeploymentTarget;

    class Deployment : public Dependency {
        friend class UmlManager;
        private:
            Sequence<DeployedArtifact> m_deployedArtifacts;
            ID m_locationID;
            DeploymentTarget* m_locationPtr;
            class AddDeployedArtifactFunctor : public AbstractSequenceFunctor {
                public:
                    AddDeployedArtifactFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveDeployedArtifactFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveDeployedArtifactFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            void setManager(UmlManager* manager) override;
        public:
            Deployment();
            Deployment(const Deployment& deployment);
            virtual ~Deployment();
            Sequence<DeployedArtifact>& getDeployedArtifact();
            DeploymentTarget* getLocation();
            void setLocation(DeploymentTarget* location);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DEPLOYMENT;
            };
    };
}

#endif