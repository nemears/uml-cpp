#ifndef ELEMENTH
#define ELEMENTH
#include <string>
#include <list>
#include <regex>
#include <exception>
#include <memory>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include "id.h"

namespace UML {

    // Element Type enum to get the type of object on runtime
    enum class ElementType {
        ACTION,
        ACTIVITY,
        ACTIVITY_EDGE,
        ACTIVITY_NODE,
        ASSOCIATION,
        BEHAVIOR,
        BEHAVIORAL_FEATURE,
        CALL_BEHAVIOR_ACTION,
        CLASS,
        CLASSIFIER,
        COMMENT,
        CONNECTABLE_ELEMENT,
        CONTROL_FLOW,
        CREATE_OBJECT_ACTION,
        DATA_TYPE,
        DECISION_NODE,
        DIRECTED_RELATIONSHIP,
        ELEMENT,
        ENUMERATION,
        ENUMERATION_LITERAL,
        EXPRESSION,
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
        LITERAL_REAL,
        LITERAL_SPECIFICATION,
        LITERAL_STRING,
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
        PARAMETER_NODE,
        PIN,
        PRIMITIVE_TYPE,
        PROPERTY,
        REDEFINABLE_ELEMENT,
        RELATIONSHIP,
        SLOT,
        STRUCTURAL_FEATURE,
        STRUCTURED_CLASSIFIER,
        TYPE,
        TYPED_ELEMENT,
        VALUE_SPECIFICATION
    };

    // Helper function to assess possible uuids
    static bool isValidUUID4(std::string strn) {
        return std::regex_match (strn, std::regex("[0-9a-f]{8}-[0-9a-f]{4}-4[0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}"));
    }

    template <class T> class Sequence;
    //template <class T> class Sequence2;
    //template <class T> struct SequenceIterator;
    class ElementDoesntExistException;
    class Relationship;
    class DirectedRelationship;
    class Comment;
    class SetOwnerFunctor;
    class RemoveOwnerFunctor;
    class Slot;
    class UmlManager;
    class ID;

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
        protected:
            // new id implementation
            UmlManager* m_manager;
            ID m_id2;

            // owner
            ID m_ownerID;
            Element* m_ownerPtr;
            
            // ownedElements



            //----------------------------------------------------------------------------------------------------
            // old id implementation
            Element* m_owner;
            // Sequences need to be pointers in element, still encapsulated but slightly different internal syntax
            Sequence<Element>* m_ownedElements;
            Sequence<Relationship>* m_relationships;
            Sequence<DirectedRelationship>* m_directedRelationships;
            Sequence<Comment>* m_ownedComments;
            boost::uuids::uuid m_id;
            virtual void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID);
            void setOwner(Element* owner);
        public:
            Element();
            virtual ~Element();

            // new implementation
            ID getID2();
            Element* getOwner2();
            void setOwner2(Element* el);
            static ElementType elementType() {
                return ElementType::ELEMENT;
            };


            //----------------------------------------------------------------------------------------------------
            // old implementation
            // Think about making the api non copyable, copying messes with the pointer vals
            // Maybe move to copy function
            // WARNING copying element dereferences it from model, elements it points to won't point to it
            Element(const Element& el);
            /**
             * Container for owned elements
             * opposite is getOwner
             * read_only (TODO implement)
             **/
            Sequence<Element>& getOwnedElements();
            /**
             * Container for relationships
             * opposite is relatedElements
             * read_only (TODO implement)
             **/
            Sequence<Relationship>& getRelationships();
            Sequence<DirectedRelationship>& getDirectedRelationships();
            Sequence<Comment>& getOwnedComments();
            boost::uuids::uuid getID() { return m_id; };
            virtual void setID(std::string id);
            void setID(boost::uuids::uuid id);
            virtual ElementType getElementType() const;
            virtual bool isSubClassOf(ElementType eType);
            virtual std::string getElementTypeString() const;
            virtual std::string getIDstring();
            Element* getOwner();
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
                msg("Removed " + el.getElementTypeString() + " that isn't in the Sequence\nuuid: " + boost::lexical_cast<std::string>(el.m_id))
                {}
            virtual const char* what() const throw() {
                return msg.c_str();
            }
    };
}
#endif