#pragma once

#include <string>
#include <exception>
#include <memory>
#include <list>
#include "uml/managers/managerNode.h"
#include "uml/managers/typeInfo.h"
#include "uml/managers/baseElement.h"
#include "uml/set/doNothingPolicy.h"

namespace YAML {
    class Node;
}

namespace UML {

    // Helper function to assess possible ids
    bool isValidID(std::string strn);
    class AbstractSet;
    template <class T, class U, class DataTypePolicy, class ApiPolicy = DoNothingPolicy>
    class PrivateSet;
    template <class T>
    class SetDataPolicy;
    template <class T, class U, class ApiPolicy = DoNothingPolicy>
    using ReadOnlySet = PrivateSet<T, U, SetDataPolicy<T>, ApiPolicy>;
    template <class T, class U, class ApiPolicy = DoNothingPolicy>
    class Set;
    template<class T>
    class SingletonDataPolicy;
    template <class T, class U, class ApiPolicy = DoNothingPolicy>
    class Singleton;
    template <class T, class U, class ApiPolicy = DoNothingPolicy>
    using ReadOnlySingleton = PrivateSet<T, U, SingletonDataPolicy<T>, ApiPolicy>;
    template <class T> class UmlPtr;
    typedef UmlPtr<Element> ElementPtr;
    class EmitterData;
    class ParserData;
    class AbstractManager;
    struct NodeReference;
                      //
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

    /**
     * Element is the base class of all UML classes
     * It has three main attributes
     * owner - the element that has ownership of this element
     * ownedElements - the elements that this element owns
     * id - the elements unique id for quick comparison and indexing
     **/
    class Element : public BaseElement<UmlTypes> {

        friend class AbstractManager;
        friend struct ManagerNode;
//        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        template <class T, class U, class DataTypePolicy, class ApiPolicy> friend class PrivateSet;
        template <class T> friend class UmlPtr;
        template <class T, class U, class ApiPolicy>
        friend class Singleton;
        template <class T, class U, class ApiPolicy>
        friend class Set;
        template <class T, class U, class ApiPolicy>
        friend class OrderedSet;
        friend std::string emit(Element& el, EmitterData& data);
        friend ElementPtr parse(std::string data, ParserData& metaData);
        friend bool parseElementScope(YAML::Node node, Element& el, ParserData& data);
        friend class Classifier; // TODO remove
        friend class BehavioredClassifier;
        friend class Usage;
        friend class InterfaceRealization;

        private:
        protected:
            
            typedef TypeInfo<std::tuple<>> Info;    
            
            std::unique_ptr<ReadOnlySingleton<Element, Element>> m_owner;
            std::unique_ptr<ReadOnlySet<Element, Element>> m_ownedElements;
            std::unique_ptr<Set<Comment, Element, DoNothingPolicy>> m_ownedComments;
            std::unique_ptr<Set<InstanceSpecification, Element, DoNothingPolicy>> m_appliedStereotypes;
            ReadOnlySingleton<Element, Element>& getOwnerSingleton();
            void setOwner(ElementPtr el);
            virtual void restoreReferences();
            virtual void referenceErased(ID id);
            Element(std::size_t elementType, AbstractManager& manager);
            void eraseFromSet(ID id, AbstractSet& set);
        public:
            Element(const Element&) = delete;
            Element& operator=(const Element&) = delete;
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
            template <class T = Element> T& as() {
                T& ret = dynamic_cast<T&>(*this);
                // if (!ret) {
                //     throw InvalidElementCastException(getElementTypeString().c_str() , elementTypeToString(T::elementType()).c_str());
                // }
                return ret;
            }

            inline friend bool operator==(const Element& lhs, const Element& rhs) {
                return lhs.m_id == rhs.m_id;
            };
            inline friend bool operator!=(const Element& lhs, const Element& rhs) {
                return lhs.m_id != rhs.m_id;
            };
        private:
            void setOwner(ID id);
    };

    //Exceptions
    class InvalidID_Exception: public std::exception {
        public:
        virtual const char* what() const throw() {
            return "String of id is not a valid UUID4";
        }
    };
}
