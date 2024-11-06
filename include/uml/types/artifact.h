#pragma once

#include "classifier.h"
#include "deployedArtifact.h"
#include "uml/managers/abstractManager.h"
#include "uml/managers/typeInfo.h"
#include "uml/set/orderedSet.h"
#include "uml/types/element.h"

namespace UML {

    class Manifestation;

    class Artifact : public Classifier, public DeployedArtifact {

        friend struct ElementInfo<Artifact>;

        protected:
            Set<Artifact, Artifact> m_nestedArtifacts = Set<Artifact, Artifact>(this);
            OrderedSet<Property, Artifact> m_ownedAttributes = OrderedSet<Property, Artifact>(this);
            OrderedSet<Operation, Artifact> m_ownedOperations = OrderedSet<Operation, Artifact>(this);
            Set<Manifestation, Artifact> m_manifestations =  Set<Manifestation, Artifact>(this);
        public:
            Artifact(std::size_t elementType, AbstractManager& manager);
            OrderedSet<Property, Artifact>& getOwnedAttributes();
            OrderedSet<Operation, Artifact>& getOwnedOperations();
            Set<Artifact, Artifact>& getNestedArtifacts();
            Set<Manifestation, Artifact>& getManifestations();
            typedef TypeInfo<std::tuple<Classifier, DeployedArtifact>, Artifact> Info;
    };

    template <>
    struct ElementInfo<Artifact> : public DefaultInfo {
        static const bool abstract = false;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "Artifact"; }
        static SetList sets(Artifact& el) {
            return SetList {
                makeSetPair("nestedArtifacts", el.m_nestedArtifacts),
                makeSetPair("ownedAttributes", el.m_ownedAttributes),
                makeSetPair("ownedOperations", el.m_ownedOperations),
                makeSetPair("manifestations", el.m_manifestations)
            };
        }
    };

    class NestedArtifactException : public std::exception {
        public:
            virtual const char* what() const throw() {
                return "Cannot add artifact to it's own nested artifacts";
            };
    };
}
