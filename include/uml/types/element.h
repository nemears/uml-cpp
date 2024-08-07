#pragma once

#include <string>
#include <exception>
#include <memory>
#include <list>
#include "uml/id.h"
#include "uml/set/doNothingPolicy.h"

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
    struct ManagerNode;
    class InstanceSpecification;
    class Comment;
    class Classifier; // TODO remove this is because of weird policy in Classifier members set
    /**
     * Element is the base class of all UML classes
     * It has three main attributes
     * owner - the element that has ownership of this element
     * ownedElements - the elements that this element owns
     * id - the elements unique id for quick comparison and indexing
     **/
    class Element {

        friend class AbstractManager;
        friend struct ManagerNode;
        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        template <class T, class U, class DataTypePolicy, class ApiPolicy> friend class PrivateSet;
        template <class T> friend class UmlPtr;
        template <class T, class U, class ApiPolicy>
        friend class Singleton;
        template <class T, class U, class ApiPolicy>
        friend class Set;
        friend std::string emit(Element& el, EmitterData& data);
        friend ElementPtr parse(std::string data, ParserData& metaData);
        friend bool parseElementScope(YAML::Node node, Element& el, ParserData& data);
        friend class Classifier; // TODO remove
        friend class BehavioredClassifier;
        friend class Usage;
        friend class InterfaceRealization;

        private:
        protected:
            AbstractManager* m_manager = 0;
            ManagerNode* m_node = 0;
            ID m_id;

            const ElementType m_elementType;

            // owner
            std::unique_ptr<ReadOnlySingleton<Element, Element>> m_owner;
            // ownedElements
            std::unique_ptr<ReadOnlySet<Element, Element>> m_ownedElements;
            std::unique_ptr<Set<Comment, Element, DoNothingPolicy>> m_ownedComments;
            std::unique_ptr<Set<InstanceSpecification, Element, DoNothingPolicy>> m_appliedStereotypes;
            ReadOnlySingleton<Element, Element>& getOwnerSingleton();
            void setOwner(ElementPtr el);
            virtual void restoreReferences();
            virtual void referenceErased(ID id);
            Element(ElementType elementType);
            void eraseFromSet(ID id, AbstractSet& set);
        public:
            Element(const Element&) = delete;
            Element& operator=(const Element&) = delete;
            virtual ~Element();
            ID getID() const;
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
            virtual void setID(std::string id);
            void setID(ID id);
            static std::string elementTypeToString(ElementType eType);
            static ElementType elementType() {
                return ElementType::ELEMENT;
            };
            virtual bool is(ElementType eType) const;
            template <class T = Element> T& as() {
                if (this->is(T::elementType())) {
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
