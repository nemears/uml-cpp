#ifndef _UML_ARTIFACT_H_
#define _UML_ARTIFACT_H_

#include "classifier.h"
#include "deployedArtifact.h"
#include "orderedSet.h"

namespace UML {

    class Manifestation;

    class Artifact : public Classifier, public DeployedArtifact {

        friend class UmlManager;

        protected:
            Set<Artifact, Artifact> m_nestedArtifacts = Set<Artifact, Artifact>(this);
            OrderedSet<Property, Artifact> m_ownedAttributes = OrderedSet<Property, Artifact>(this);
            OrderedSet<Operation, Artifact> m_ownedOperations = OrderedSet<Operation, Artifact>(this);
            Set<Manifestation, Artifact> m_manifestations =  Set<Manifestation, Artifact>(this);
            Set<Property, Artifact>& getOwnedAttributesSet();
            Set<Operation, Artifact>& getOwnedOperationsSet();
            void init();
            Artifact();
        public:
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