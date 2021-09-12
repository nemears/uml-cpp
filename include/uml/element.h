#ifndef ELEMENTH
#define ELEMENTH
#include <string>
#include <list>
#include <regex>
#include <exception>
#include <memory>
#include "id.h"

namespace UML {

    // Element Type enum to get the type of object on runtime
    enum class ElementType {
        ABSTRACTION,
        ACTION,
        ACTIVITY,
        ACTIVITY_EDGE,
        ACTIVITY_NODE,
        ARTIFACT,
        ASSOCIATION,
        BEHAVIOR,
        BEHAVIORAL_FEATURE,
        BEHAVIORED_CLASSIFIER,
        CALL_BEHAVIOR_ACTION,
        CLASS,
        CLASSIFIER,
        COMMENT,
        CONNECTABLE_ELEMENT,
        CONTROL_FLOW,
        CREATE_OBJECT_ACTION,
        DATA_TYPE,
        DECISION_NODE,
        DEPENDENCY,
        DEPLOYED_ARTIFACT,
        DEPLOYMENT,
        DEPLOYMENT_TARGET,
        DIRECTED_RELATIONSHIP,
        ELEMENT,
        ENUMERATION,
        ENUMERATION_LITERAL,
        EXPRESSION,
        EXTENSION,
        EXTENSION_END,
        FEATURE,
        FINAL_NODE,
        FORK_NODE,
        GENERALIZATION,
        INITIAL_NODE,
        INPUT_PIN,
        INSTANCE_SPECIFICATION,
        INSTANCE_VALUE,
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
        OPAQUE_BEHAVIOR,
        OPERATION,
        OUTPUT_PIN,
        PACKAGE,
        PACKAGEABLE_ELEMENT,
        PACKAGE_MERGE,
        PARAMETER,
        PARAMETERABLE_ELEMENT,
        PARAMETER_NODE,
        PIN,
        PRIMITIVE_TYPE,
        PROFILE,
        PROFILE_APPLICATION,
        PROPERTY,
        REALIZATION,
        REDEFINABLE_ELEMENT,
        RELATIONSHIP,
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
        VALUE_SPECIFICATION
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

    template <class T> class Sequence;
    template <class T, class U> class TemplateAbstractSequenceFunctor;
    template <class T, class U> class Singleton;
    class ElementDoesntExistException;
    class Relationship;
    class DirectedRelationship;
    class Comment;
    class SetOwnerFunctor;
    class RemoveOwnerFunctor;
    class Slot;
    class InstanceSpecification;
    class UmlManager;
    struct ManagerNode;
    class AddOwnedCommentFunctor;
    /**
     * Element is the base class of all UML classes
     * It has three main attributes
     * owner - the element that has ownership of this element
     * ownedElements - the elements that this element owns
     * id - the elements unique id for quick comparison and indexing
     **/
    class Element {
        friend class ElementDoesntExistException;
        friend class SetOwnerFunctor;
        friend class RemoveOwnerFunctor;
        friend class Slot;
        friend class UmlManager;
        friend class AddOwnedCommentFunctor;
        template<typename> friend class Sequence;
        template <class T, class U> friend class TemplateAbstractSequenceFunctor;
        template <class T, class U> friend class Singleton;
        protected:
            UmlManager* m_manager;
            ManagerNode* m_node;
            ID m_id;

            // owner
            ID m_ownerID;
            Element* m_ownerPtr;
            
            // ownedElements
            Sequence<Element>* m_ownedElements;

            virtual void setManager(UmlManager* manager);
            Sequence<Relationship>* m_relationships;
            Sequence<DirectedRelationship>* m_directedRelationships;
            Sequence<Comment>* m_ownedComments;
            Sequence<InstanceSpecification>* m_appliedStereotype;
            virtual void reindexID(ID oldID, ID newID);
            void setOwner(Element* el);
            static bool isSameOrNull(ID id, Element* el);
            virtual void restoreReleased(ID id, Element* released);
            virtual void referencingReleased(ID id);
            void setReference(Element* referencing);
            void removeReference(ID referencing);
            virtual void referenceReindexed(ID oldID, ID newID);
            Element();
        public:
            Element(const Element& el);
            virtual ~Element();
            ID getID();
            Element* getOwner();
            Sequence<Element>& getOwnedElements();
            Sequence<Relationship>& getRelationships();
            Sequence<DirectedRelationship>& getDirectedRelationships();
            Sequence<Comment>& getOwnedComments();
            /**
             * TODO: I am keeping it simple for now, instance specification of stereotype to
             *       hold tags and operations, but I think it would be cool to dynamically map
             *       methods if we load the stereotype bef ore runtime. Also would be cool to have
             *       stereotype tags as keyword in yaml config for disk storage (not necessarily useful though?)
             **/
            Sequence<InstanceSpecification>& getAppliedStereotypes();
            virtual void setID(std::string id);
            void setID(ID id);
            static std::string elementTypeToString(ElementType eType);
            static ElementType elementType() {
                return ElementType::ELEMENT;
            };
            template <class T = Element> T& as() {
                if (isSubClassOf(T::elementType())) {
                    return dynamic_cast<T&>(*this);
                }
                throw InvalidElementCastException(getElementTypeString().c_str() , elementTypeToString(T::elementType()).c_str());
            }
            virtual ElementType getElementType() const;
            virtual bool isSubClassOf(ElementType eType) const;
            virtual std::string getElementTypeString() const;

            friend bool operator==(const Element& lhs, const Element& rhs) {
                return lhs.m_id == rhs.m_id;
            };
            friend bool operator!=(const Element& lhs, const Element& rhs) {
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