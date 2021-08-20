#ifndef ARTIFACT_H
#define ARTIFACT_H

#include "classifier.h"
#include "deployedArtifact.h"

namespace UML {

    class Manifestation;

    class Artifact : public Classifier, public DeployedArtifact {
        friend class UmlManager;
        protected:
            Sequence<Artifact> m_nestedArtifacts = Sequence<Artifact>(this);
            Sequence<Property> m_ownedAttributes = Sequence<Property>(this);
            Sequence<Operation> m_ownedOperations = Sequence<Operation>(this);
            Sequence<Manifestation> m_manifestations =  Sequence<Manifestation>(this);
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
            class CheckNestedArtifactFunctor : public AbstractSequenceFunctor {
                public:
                    CheckNestedArtifactFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class AddNestedArtifactFunctor : public AbstractSequenceFunctor {
                public:
                    AddNestedArtifactFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveNestedArtifactFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveNestedArtifactFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class AddManifestationFunctor : public AbstractSequenceFunctor {
                public:
                    AddManifestationFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveManifestationFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveManifestationFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
        public:
            Artifact();
            Artifact(const Artifact& artifact);
            virtual ~Artifact();
            Sequence<Property>& getOwnedAttributes();
            Sequence<Operation>& getOwnedOperations();
            Sequence<Artifact>& getNestedArtifacts();
            Sequence<Manifestation>& getManifestations();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ARTIFACT;
            };
    };

    class NestedArtifactException : public std::exception {
        public:
            virtual const char* what() const throw() {
                return "Cannot add artifact to it's own nested artifacts";
            };
    };
}

#endif