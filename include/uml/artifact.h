#ifndef ARTIFACT_H
#define ARTIFACT_H

#include "classifier.h"
#include "deployedArtifact.h"

namespace UML {
    class Artifact : public Classifier, public DeployedArtifact {
        friend class UmlManager;
        protected:
            Sequence<Property> m_ownedAttributes;
            Sequence<Operation> m_ownedOperations;
            void setManager(UmlManager* manager) override;
            class AddOwnedAttributeFunctor : public AbstractSequenceFunctor {
                public:
                    AddOwnedAttributeFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveOwnedAttributeFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveOwnedAttributeFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class AddOwnedOperationFunctor : public AbstractSequenceFunctor {
                public:
                    AddOwnedOperationFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveOwnedOperationFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveOwnedOperationFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
        public:
            Artifact();
            Artifact(const Artifact& artifact);
            virtual ~Artifact();
            Sequence<Property>& getOwnedAttributes();
            Sequence<Operation>& getOwnedOperations();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ARTIFACT;
            };
    };
}

#endif