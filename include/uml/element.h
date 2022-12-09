#ifndef _UML_ELEMENT_H_
#define _UML_ELEMENT_H_

#include <string>
#include <list>
#include <regex>
#include <exception>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "id.h"
// #include <mutex>
#include "set/doNothingPolicy.h"
#include "set/setLock.h"

namespace YAML {
    class Node;
}

namespace UML {

    // Element Type enum to get the type of object on runtime
    enum class ElementType {
        ABSTRACTION,
        ACTION,
        ACTION_INPUT_PIN,
        ACTIVITY,
        ACTIVITY_EDGE,
        ACTIVITY_FINAL_NODE,
        ACTIVITY_GROUP,
        ACTIVITY_NODE,
        ACTIVITY_PARAMETER_NODE,
        ACTIVITY_PARTITION,
        ARTIFACT,
        ASSOCIATION,
        BEHAVIOR,
        BEHAVIORAL_FEATURE,
        BEHAVIORED_CLASSIFIER,
        CALL_ACTION,
        CALL_BEHAVIOR_ACTION,
        CENTRAL_BUFFER_NODE,
        CLASS,
        CLASSIFIER,
        CLASSIFIER_TEMPLATE_PARAMETER,
        COMMENT,
        CONNECTABLE_ELEMENT,
        CONNECTOR,
        CONNECTOR_END,
        CONSTRAINT,
        CONTROL_FLOW,
        CONTROL_NODE,
        CREATE_OBJECT_ACTION,
        DATA_STORE_NODE,
        DATA_TYPE,
        DECISION_NODE,
        DEPENDENCY,
        DEPLOYED_ARTIFACT,
        DEPLOYMENT,
        DEPLOYMENT_TARGET,
        DIRECTED_RELATIONSHIP,
        ELEMENT,
        ELEMENT_IMPORT,
        ENCAPSULATED_CLASSIFIER,
        ENUMERATION,
        ENUMERATION_LITERAL,
        EXCEPTION_HANDLER,
        EXECUTABLE_NODE,
        EXPRESSION,
        EXTENSION,
        EXTENSION_END,
        FEATURE,
        FINAL_NODE,
        FLOW_FINAL_NODE,
        FORK_NODE,
        GENERALIZATION,
        GENERALIZATION_SET,
        INITIAL_NODE,
        INPUT_PIN,
        INSTANCE_SPECIFICATION,
        INSTANCE_VALUE,
        INTERFACE_UML, // why microsoft
        INTERFACE_REALIZATION,
        INTERRUPTIBLE_ACTIVITY_REGION,
        INVOCATION_ACTION,
        JOIN_NODE,
        LITERAL_BOOL,
        LITERAL_INT,
        LITERAL_NULL,
        LITERAL_REAL,
        LITERAL_SPECIFICATION,
        LITERAL_STRING,
        LITERAL_UNLIMITED_NATURAL,
        MANIFESTATION,
        MERGE_NODE,
        MODEL,
        MULTIPLICITY_ELEMENT,
        NAMED_ELEMENT,
        NAMESPACE,
        OBJECT_FLOW,
        OBJECT_NODE,
        OPAQUE_ACTION,
        OPAQUE_BEHAVIOR,
        OPERATION,
        OUTPUT_PIN,
        PACKAGE,
        PACKAGEABLE_ELEMENT,
        PACKAGE_IMPORT,
        PACKAGE_MERGE,
        PARAMETER,
        PARAMETERABLE_ELEMENT,
        PARAMETER_SET,
        PIN,
        PORT,
        PRIMITIVE_TYPE,
        PROFILE,
        PROFILE_APPLICATION,
        PROPERTY,
        REALIZATION,
        RECEPTION,
        REDEFINABLE_ELEMENT,
        REDEFINABLE_TEMPLATE_SIGNATURE,
        RELATIONSHIP,
        SIGNAL,
        SLOT,
        STEREOTYPE,
        STRUCTURAL_FEATURE,
        STRUCTURED_CLASSIFIER,
        TEMPLATEABLE_ELEMENT,
        TEMPLATE_BINDING,
        TEMPLATE_PARAMETER,
        TEMPLATE_PARAMETER_SUBSTITUTION,
        TEMPLATE_SIGNATURE,
        TYPE,
        TYPED_ELEMENT,
        USAGE,
        VALUE_PIN,
        VALUE_SPECIFICATION,

        NOT_SET
    };

    // Helper function to assess possible ids
    bool isValidID(std::string strn);

    class InvalidElementCastException : public std::exception {
        
        private:
            char m_msg[200];
        
        public:
            InvalidElementCastException(const char* first, const char* second) {
                sprintf(m_msg, "Cannot cast %s to %s", first, second);
            };
            virtual const char* what() const throw() {
                return m_msg;
            };
    };  

    class Element;
    class ElementDoesntExistException;
    class Relationship;
    class DirectedRelationship;
    class Comment;
    class Slot;
    class InstanceSpecification;
    class Property;
    class Association;
    class AbstractAccessPolicy;
    class AbstractManager;
    class UmlServer;
    struct ManagerNode;
    class AddToMountFunctor;
    class PackageMerge;
    class Classifier;
    class Namespace;
    class NamedElement;
    class Generalization;
    class Dependency;
    class TemplateBinding;
    class TemplateParameterSubstitution;
    class TypedElement;
    class Connector;
    class ConnectableElement;
    class Port;
    class BehavioredClassifier;
    class InterfaceRealization;
    class Usage;
    class RedefinableTemplateSignature;
    template <class T, class U, class AdditionPolicy, class RemovalPolicy> class CustomSet;
    template <class T, class U> class Set;
    template <class T, class U, class AdditionPolicy, class RemovalPolicy> class CustomSingleton;
    template <class T, class U> class Singleton;
    template <class T, class U, class AdditionPolicy, class RemovalPolicy> class CustomOrderedSet;
    template< class T, class U> class TypedSet;
    template <class T> class UmlPtr;
    typedef UmlPtr<Element> ElementPtr;
    namespace Parsers {
        struct EmitterMetaData;
        class ParserMetaData;
        EmitterMetaData getData(Element& el);
        void setOwner(Element& el, ID id);
        void emitToFile(Element& el, EmitterMetaData& data, std::string path, std::string fileName);
        ElementPtr parse(ParserMetaData& data);
        ElementPtr parseYAML(YAML::Node node, ParserMetaData& data);
    }
    /**
     * Element is the base class of all UML classes
     * It has three main attributes
     * owner - the element that has ownership of this element
     * ownedElements - the elements that this element owns
     * id - the elements unique id for quick comparison and indexing
     **/
    class Element {

        friend class AbstractManager;
        friend class AbstractAccessPolicy;
        friend struct ManagerNode;
        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;
        // friend class UmlClient;
        friend class UmlServer;
        friend class ElementDoesntExistException;
        friend class Slot;
        friend class Property;
        friend class Association;
        friend class AddToMountFunctor;
        friend class PackageMerge;
        friend class Classifier;
        friend class Namespace;
        friend class InstanceSpecification;
        friend class NamedElement;
        friend class Generalization;
        friend class Dependency;
        friend class TemplateBinding;
        friend class TemplateParameterSubstitution;
        friend class TypedElement;
        friend class Connector;
        friend class ConnectableElement;
        friend class Port;
        friend class BehavioredClassifier;
        friend class InterfaceRealization;
        friend class Usage;
        friend class RedefinableTemplateSignature;
        template <class T, class U, class AdditionPolicy, class RemovalPolicy> friend class CustomSingleton;
        template <class T, class U, class AdditionPolicy, class RemovalPolicy> friend class CustomSet;
        template <class T, class U, class AdditionPolicy, class RemovalPolicy> friend class CustomOrderedSet;
        template <class T, class U, class AdditionPolicy, class RemovalPolicy, class AllocationPolicy> friend class PrivateSet;
        template <class T> friend class UmlPtr;
        friend Parsers::EmitterMetaData Parsers::getData(Element& el);
        friend ElementPtr Parsers::parseYAML(YAML::Node node, Parsers::ParserMetaData& data);
        friend void Parsers::setOwner(Element& el, ID id);
        friend void Parsers::emitToFile(Element& el, Parsers::EmitterMetaData& data, std::string path, std::string fileName);
        friend ElementPtr Parsers::parse(Parsers::ParserMetaData& data);

        private:
        protected:
            AbstractManager* m_manager = 0;
            ManagerNode* m_node = 0;
            ID m_id;

            const ElementType m_elementType;

            // owner
            CustomSingleton<Element, Element, DoNothingAdd<Element, Element>, DoNothingRemove<Element, Element>>* m_owner;
            TypedSet<Element, Element>& getOwnerSingleton();
            
            // ownedElements
            CustomSet<Element, Element, DoNothingAdd<Element, Element>, DoNothingRemove<Element, Element>>* m_ownedElements;
            CustomSet<Comment, Element, DoNothingAdd<Comment, Element>, DoNothingRemove<Comment, Element>>* m_ownedComments;
            CustomSet<InstanceSpecification, Element, DoNothingAdd<InstanceSpecification, Element>, DoNothingRemove<InstanceSpecification, Element>>* m_appliedStereotype;
            void setOwner(Element* el);
            void setOwnerByID(ID id);
            void setReference(Element* referencing);
            void removeReference(ID referencing);
            void setReference(ID id);
            virtual void referenceReindexed(ID newID);
            virtual void restoreReferences();
            virtual void restoreReference(Element* el);
            virtual void referenceErased(ID id);
            void mountAndRelease();
            SetLock lockEl(Element& el);
            Element(ElementType elementType);
        public:
            Element(const Element&) = delete;
            Element& operator=(const Element&) = delete;
            virtual ~Element();
            ID getID() const;
            ElementPtr getOwner() const;
            Set<Element, Element>& getOwnedElements();
            Set<Comment, Element>& getOwnedComments();
            /**
             * TODO: I am keeping it simple for now, instance specification of stereotype to
             *       hold tags and operations, but I think it would be cool to dynamically map
             *       methods if we load the stereotype before runtime. Also would be cool to have
             *       stereotype tags as keyword in yaml config for disk storage (not necessarily useful though?)
             **/
            Set<InstanceSpecification, Element>& getAppliedStereotypes();
            virtual void setID(std::string id);
            void setID(ID id);
            static std::string elementTypeToString(ElementType eType);
            static ElementType elementType() {
                return ElementType::ELEMENT;
            };
            virtual bool isSubClassOf(ElementType eType) const;
            template <class T = Element> T& as() {
                if (this->isSubClassOf(T::elementType())) {
                    return dynamic_cast<T&>(*this);
                }
                throw InvalidElementCastException(getElementTypeString().c_str() , elementTypeToString(T::elementType()).c_str());
            }
            ElementType getElementType() const;
            virtual std::string getElementTypeString() const;

            inline friend bool operator==(const Element& lhs, const Element& rhs) {
                return lhs.m_id == rhs.m_id;
            };
            inline friend bool operator!=(const Element& lhs, const Element& rhs) {
                return lhs.m_id != rhs.m_id;
            };
    };

    //Exceptions

    class InvalidID_Exception: public std::exception {
        public:
        virtual const char* what() const throw() {
            return "String of id is not a valid UUID4";
        }
    };
    
    class ElementDoesntExistException : public std::exception {
        friend class Element;
        private:
            std::string msg;

        public:
            ElementDoesntExistException(const Element& el) : 
                msg("Removed " + el.getElementTypeString() + " that isn't in the Sequence\nuuid: " + el.m_id.string())
                {}
            virtual const char* what() const throw() {
                return msg.c_str();
            }
    };
}
#endif