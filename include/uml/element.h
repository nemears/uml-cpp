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
#include <mutex>

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

    class Element;
    class AbstractUmlPtr;

    /**
     * The ManagerNode struct is used as nodes in the internal graphs of the element's manager
     * It is also stored as a pointer from within the element so each element can quickly access
     * its place in the reference graph
     **/
    struct ManagerNode {
        Element* m_managerElementMemory = 0;
        std::string m_path;
        bool m_mountedFlag = false;
        std::unordered_map<ID, ManagerNode*> m_references;
        std::unordered_map<ID, size_t> m_referenceCount;
        std::vector<ID> m_referenceOrder;
        std::list<void*> m_ptrs; // list to UmlPtr*'s
    };

    // Helper function to assess possible ids
    static bool isValidID(std::string strn) {
        return std::regex_match (strn, std::regex("(?:[A-Za-z0-9_&]{28})"));
    }

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

    class ElementDoesntExistException;
    class Relationship;
    class DirectedRelationship;
    class Comment;
    class Slot;
    class InstanceSpecification;
    class Property;
    class Association;
    class UmlManager;
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
    template <class T, class U> class Set;
    template <class T> struct SetIterator;
    template <class V, class W> class OppositeFunctor;
    template <class T, class U> class Singleton;
    template <class T, class U> class OrderedSet;
    template <class T, class U> struct OrderedSetIterator;
    template <class T> class UmlPtr;
    class SetReferenceFunctor;
    class RemoveReferenceFunctor;
    namespace Parsers {
        struct EmitterMetaData;
        EmitterMetaData getData(Element& el);
        void setOwner(Element& el, ID id);
    }

    typedef UmlPtr<Element> ElementPtr;
    /**
     * Element is the base class of all UML classes
     * It has three main attributes
     * owner - the element that has ownership of this element
     * ownedElements - the elements that this element owns
     * id - the elements unique id for quick comparison and indexing
     **/
    class Element {

        friend class ElementDoesntExistException;
        friend class Slot;
        friend class Property;
        friend class Association;
        friend class UmlManager;
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
        template <class T, class U> friend class Singleton;
        template <class T, class U> friend class Set;
        template <class V, class W> friend class OppositeFunctor;
        template <class T> friend struct SetIterator;
        template <class T, class U> friend class OrderedSet;
        template <class T, class U> friend struct OrderedSetIterator;
        template <class T> friend class UmlPtr;
        friend Parsers::EmitterMetaData Parsers::getData(Element& el);
        friend void Parsers::setOwner(Element& el, ID id);
        friend class SetReferenceFunctor;
        friend class RemoveReferenceFunctor;

        private:
        protected:
            UmlManager* m_manager;
            ManagerNode* m_node;
            ID m_id;

            const ElementType m_elementType;

            // owner
            Singleton<Element, Element>* m_owner;
            Set<Element, Element>& getOwnerSingleton();
            
            // ownedElements
            Set<Element, Element>* m_ownedElements;
            Set<Comment, Element>* m_ownedComments;
            Set<InstanceSpecification, Element>* m_appliedStereotype;
            void setOwner(Element* el);
            void setOwnerByID(ID id);
            virtual void referencingReleased(ID id);
            void setReference(Element* referencing);
            void removeReference(ID referencing);
            void setReference(ID id);
            virtual void referenceReindexed(ID oldID, ID newID);
            virtual void reindexName(ID id, std::string newName);
            virtual void restoreReferences();
            virtual void restoreReference(Element* el);
            virtual void referenceErased(ID id);
            void mountAndRelease();
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