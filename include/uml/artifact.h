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
            class AddOwnedAttributeFunctor : public TemplateAbstractSequenceFunctor<Property,Artifact> {
                public:
                    AddOwnedAttributeFunctor(Artifact* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Property& el) const override;
            };
            class RemoveOwnedAttributeFunctor : public TemplateAbstractSequenceFunctor<Property,Artifact> {
                public:
                    RemoveOwnedAttributeFunctor(Artifact* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Property& el) const override;
            };
            class AddOwnedOperationFunctor : public TemplateAbstractSequenceFunctor<Operation,Artifact> {
                public:
                    AddOwnedOperationFunctor(Artifact* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Operation& el) const override;
            };
            class RemoveOwnedOperationFunctor : public TemplateAbstractSequenceFunctor<Operation,Artifact> {
                public:
                    RemoveOwnedOperationFunctor(Artifact* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Operation& el) const override;
            };
            class CheckNestedArtifactFunctor : public TemplateAbstractSequenceFunctor<Artifact,Artifact> {
                public:
                    CheckNestedArtifactFunctor(Artifact* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Artifact& el) const override;
            };
            class AddNestedArtifactFunctor : public TemplateAbstractSequenceFunctor<Artifact,Artifact> {
                public:
                    AddNestedArtifactFunctor(Artifact* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Artifact& el) const override;
            };
            class RemoveNestedArtifactFunctor : public TemplateAbstractSequenceFunctor<Artifact,Artifact> {
                public:
                    RemoveNestedArtifactFunctor(Artifact* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Artifact& el) const override;
            };
            class AddManifestationFunctor : public TemplateAbstractSequenceFunctor<Manifestation,Artifact> {
                public:
                    AddManifestationFunctor(Artifact* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Manifestation& el) const override;
            };
            class RemoveManifestationFunctor : public TemplateAbstractSequenceFunctor<Manifestation,Artifact> {
                public:
                    RemoveManifestationFunctor(Artifact* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Manifestation& el) const override;
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