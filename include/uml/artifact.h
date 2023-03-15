#ifndef _UML_ARTIFACT_H_
#define _UML_ARTIFACT_H_

#include "classifier.h"
#include "deployedArtifact.h"
#include "set/orderedSet.h"

namespace UML {

    class Manifestation;

    class Artifact : public Classifier, public DeployedArtifact {

        template <typename AccessPolicy, typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            CustomSet<Artifact, Artifact> m_nestedArtifacts = CustomSet<Artifact, Artifact>(this);
            CustomOrderedSet<Property, Artifact> m_ownedAttributes = CustomOrderedSet<Property, Artifact>(this);
            CustomOrderedSet<Operation, Artifact> m_ownedOperations = CustomOrderedSet<Operation, Artifact>(this);
            CustomSet<Manifestation, Artifact> m_manifestations =  CustomSet<Manifestation, Artifact>(this);
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