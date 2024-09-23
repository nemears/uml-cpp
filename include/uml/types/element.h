#pragma once

#include <exception>
#include "uml/managers/typeInfo.h"
#include "uml/managers/baseElement.h"
#include "uml/set/set.h"
#include "uml/set/singleton.h"
#include "uml/set/doNothingPolicy.h"
#include <vector>

namespace YAML {
    class Node;
}

namespace UML {

    // forward declare all types from the base UML api
    class Abstraction;
    class Artifact;
    class Association;
    class Behavior;
    class BehavioralFeature;
    class BehavioredClassifier;
    class Class;
    class Classifier;
    class ClassifierTemplateParameter;
    class Comment;
    class ConnectableElement;
    class Connector;
    class ConnectorEnd;
    class Constraint;
    class DataType;
    class Dependency;
    class DeployedArtifact;
    class Deployment;
    class DeploymentTarget;
    class DirectedRelationship;
    class Element;
    class ElementImport;
    class EncapsulatedClassifier;
    class Enumeration;
    class EnumerationLiteral;
    class Expression;
    class Extension;
    class ExtensionEnd;
    class Feature;
    class Generalization;
    class GeneralizationSet;
    class InstanceSpecification;
    class InstanceValue;
    class Interface;
    class InterfaceRealization;
    class LiteralBool;
    class LiteralInt;
    class LiteralNull;
    class LiteralReal;
    class LiteralSpecification;
    class LiteralString;
    class LiteralUnlimitedNatural;
    class Manifestation;
    class Model;
    class MultiplicityElement;
    class NamedElement;
    class Namespace;
    class OpaqueBehavior;
    class Operation;
    class Package;
    class PackageImport;
    class PackageMerge;
    class PackageableElement;
    class Parameter;
    class ParameterSet;
    class ParameterableElement;
    class Port;
    class PrimitiveType;
    class Profile;
    class ProfileApplication;
    class Property;
    class Realization;
    class Reception;
    class RedefinableElement;
    class RedefinableTemplateSignature;
    class Relationship;
    class Signal;
    class Slot;
    class Stereotype;
    class StructuralFeature;
    class StructuredClassifier;
    class TemplateBinding;
    class TemplateParameter;
    class TemplateParameterSubstitution;
    class TemplateSignature;
    class TemplateableElement;
    class Type;
    class TypedElement;
    class Usage;
    class ValueSpecification;

    // put all types in a typelist (tuple)
    typedef std::tuple<
        Abstraction,
        Artifact,
        Association,
        Behavior,
        BehavioralFeature,
        BehavioredClassifier,
        Class,
        Classifier,
        ClassifierTemplateParameter,
        Comment,
        ConnectableElement,
        Connector,
        ConnectorEnd,
        Constraint,
        DataType,
        Dependency,
        DeployedArtifact,
        Deployment,
        DeploymentTarget,
        DirectedRelationship,
        Element,
        ElementImport,
        EncapsulatedClassifier,
        Enumeration,
        EnumerationLiteral,
        Expression,
        Extension,
        ExtensionEnd,
        Feature,
        Generalization,
        GeneralizationSet,
        InstanceSpecification,
        InstanceValue,
        Interface,
        InterfaceRealization,
        LiteralBool,
        LiteralInt,
        LiteralNull,
        LiteralReal,
        LiteralSpecification,
        LiteralString,
        LiteralUnlimitedNatural,
        Manifestation,
        Model,
        MultiplicityElement,
        NamedElement,
        Namespace,
        OpaqueBehavior,
        Operation,
        Package,
        PackageImport,
        PackageMerge,
        PackageableElement,
        Parameter,
        ParameterSet,
        ParameterableElement,
        Port,
        PrimitiveType,
        Profile,
        ProfileApplication,
        Property,
        Realization,
        Reception,
        RedefinableElement,
        RedefinableTemplateSignature,
        Relationship,
        Signal,
        Slot,
        Stereotype,
        StructuralFeature,
        StructuredClassifier,
        TemplateBinding,
        TemplateParameter,
        TemplateParameterSubstitution,
        TemplateSignature,
        TemplateableElement,
        Type,
        TypedElement,
        Usage,
        ValueSpecification
    > UmlTypes;

    typedef UmlPtr<Element> ElementPtr;

    /**
     * Element is the base class of all UML classes
     * It has three main attributes
     * owner - the element that has ownership of this element
     * ownedElements - the elements that this element owns
     * id - the elements unique id for quick comparison and indexing
     **/
    class Element : public BaseElement<UmlTypes> {

        friend struct ElementInfo<Element>;

        protected:
            ReadOnlySingleton<Element, Element> m_owner = ReadOnlySingleton<Element, Element>(this);
            ReadOnlySet<Element, Element> m_ownedElements = ReadOnlySet<Element, Element>(this);
            Set<Comment, Element> m_ownedComments = Set<Comment, Element>(this);
            Set<InstanceSpecification, Element> m_appliedStereotypes = Set<InstanceSpecification, Element>(this);
            ReadOnlySingleton<Element, Element>& getOwnerSingleton();
            Element(std::size_t elementType, AbstractManager& manager);
        public:
            ElementPtr getOwner() const;
            ReadOnlySet<Element, Element>& getOwnedElements();
            Set<Comment, Element, DoNothingPolicy>& getOwnedComments();
            /**
             * TODO: I am keeping it simple for now, instance specification of stereotype to
             *       hold tags and operations, but I think it would be cool to dynamically map
             *       methods if we load the stereotype before runtime. Also would be cool to have
             *       stereotype tags as keyword in yaml config for disk storage (not necessarily useful though?)
             **/
            Set<InstanceSpecification, Element, DoNothingPolicy>& getAppliedStereotypes();

            typedef TypeInfo<std::tuple<>, Element> Info;
        
        private:
            void setOwner(ID id);
    };

    inline std::pair<std::string, AbstractSet*> makeSetPair(const char* name, AbstractSet& set) {
        return std::make_pair<std::string, AbstractSet*>(name, &set);
    }

    // template specialization for ElementInfo<Element> (required for all implementing types)
    template <>
    struct ElementInfo<Element> : public DefaultInfo {
        static constexpr std::string_view name {"Element"};
        static SetList sets(Element& el) {
            return std::vector {
                makeSetPair("owner", el.m_owner),
                makeSetPair("ownedElements", el.m_ownedElements),
                makeSetPair("ownedComments", el.m_ownedComments),
                makeSetPair("appliedStereotypes", el.m_appliedStereotypes)
            };
        }
    };
}
