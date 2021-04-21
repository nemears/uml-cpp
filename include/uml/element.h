#ifndef ELEMENTH
#define ELEMENTH
#include <string>
#include <list>
#include <regex>
#include <exception>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;

namespace UML {

    // Element Type enum to get the type of object on runtime
    enum class ElementType {
        ACTION,
        ACTIVITY,
        ACTIVITY_EDGE,
        ACTIVITY_NODE,
        BEHAVIOR,
        BEHAVIORAL_FEATURE,
        CALL_BEHAVIOR_ACTION,
        CLASS,
        CLASSIFIER,
        CONTROL_FLOW,
        CREATE_OBJECT_ACTION,
        DECISION_NODE,
        DIRECTED_RELATIONSHIP,
        ELEMENT,
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
        PARAMETER,
        PARAMETER_NODE,
        PIN,
        PRIMITIVE_TYPE,
        PROPERTY,
        RELATIONSHIP,
        SLOT,
        STRUCTURAL_FEATURE,
        TYPE,
        TYPED_ELEMENT,
        VALUE_SPECIFICATION
    };

    // Helper function to assess possible uuids
    static bool isValidUUID4(string strn) {
        return regex_match (strn, regex("[0-9a-f]{8}-[0-9a-f]{4}-4[0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}"));
    }

    // Forward Declarations for Sequence
    template <class T> class Sequence;

    // Forward Declaration for errors
    class ElementDoesntExistException;

    class Relationship;

    /**
     * Element is the base class of all UML classes
     * It has three main attributes
     * owner - the element that has ownership of this element
     * ownedElements - the elements that this element owns
     * id - the elements unique id for quick comparison and indexing
     **/
    class Element {
        friend class ElementDoesntExistException;
        protected:
            Element* m_owner;
            Sequence<Element>* m_ownedElements;
            Sequence<Relationship>* m_relationships;
            boost::uuids::uuid m_id;
            virtual void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID);
        public:
            Element();
            ~Element();
            // Think about making the api non copyable, copying messes with the pointer vals
            // Maybe move to copy function
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
            boost::uuids::uuid getID() { return m_id; };
            virtual void setID(string id);
            void setID(boost::uuids::uuid id);
            virtual ElementType getElementType();
            virtual bool isSubClassOf(ElementType eType);
            virtual string getElementTypeString();
            virtual string getIDstring();
            Element* getOwner();
            /**
             * TODO delete, owner should be read_only
             * opposite is owned elements
             **/
            void setOwner(Element* owner);
    };

    //Exceptions

    class InvalidID_Exception: public exception {
        public:
        virtual const char* what() const throw() {
            return "String of id is not a valid UUID4";
        }
    };
    
    class ElementDoesntExistException : public exception {
        friend class Element;
        private:
            string msg;

        public:
            ElementDoesntExistException(const Element& el) : 
                msg("Removed element that isn't in the Sequence\nuuid: " + boost::lexical_cast<string>(el.m_id))
                {}
            virtual const char* what() const throw() {
                return msg.c_str();
            }
    };
}
#endif