#ifndef ARTIFACT_H
#define ARTIFACT_H

#include "classifier.h"
#include "deployedArtifact.h"
#include "orderedSet.h"

namespace UML {

    class Manifestation;
    namespace Parsers {
        class ArtifactSetArtifact;
    }

    class Artifact : public Classifier, public DeployedArtifact {

        friend class UmlManager;
        friend class Parsers::ArtifactSetArtifact;

        protected:
            Set<Artifact, Artifact> m_nestedArtifacts = Set<Artifact, Artifact>(this);
            OrderedSet<Property, Artifact> m_ownedAttributes = OrderedSet<Property, Artifact>(this);
            OrderedSet<Operation, Artifact> m_ownedOperations = OrderedSet<Operation, Artifact>(this);
            Set<Manifestation, Artifact> m_manifestations =  Set<Manifestation, Artifact>(this);
            Set<Property, Artifact>& getOwnedAttributesSet();
            Set<Operation, Artifact>& getOwnedOperationsSet();
            void init();
            void copy(const Artifact& rhs);
            Artifact();
        public:
            Artifact(const Artifact& rhs);
            virtual ~Artifact();
            OrderedSet<Property, Artifact>& getOwnedAttributes();
            OrderedSet<Operation, Artifact>& getOwnedOperations();
            Set<Artifact, Artifact>& getNestedArtifacts();
            Set<Manifestation, Artifact>& getManifestations();
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