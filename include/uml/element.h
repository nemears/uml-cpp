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

static bool isValidUUID4(string strn) {
    return regex_match (strn, regex("[0-9a-f]{8}-[0-9a-f]{4}-4[0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}"));
}

enum class ElementType {
    ACTION,
    ACTIVITY,
    ACTIVITY_EDGE,
    ACTIVITY_NODE,
    BEHAVIOR,
    CLASS,
    CLASSIFIER,
    CONTROL_FLOW,
    DECISION_NODE,
    ELEMENT,
    FINAL_NODE,
    FORK_NODE,
    INITIAL_NODE,
    INPUT_PIN,
    INSTANCE_SPECIFICATION,
    INSTANCE_VALUE,
    JOIN_NODE,
    LITERAL_BOOL,
    LITERAL_INT,
    LITERAL_REAL,
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
    PARAMETER,
    PIN,
    PRIMITIVE_TYPE,
    PROPERTY,
    SLOT,
    STRUCTURAL_FEATURE,
    TYPE,
    TYPED_ELEMENT,
    VALUE_SPECIFICATION
};

class Element {
    protected:
        Element* owner;

    public:
        list<Element*> ownedElements;
        boost::uuids::uuid uuid;
        Element() {
            uuid = boost::uuids::random_generator()();
            owner = NULL;
        };
        virtual ~Element() {};
        virtual void setID(string id);
        class InvalidID_Exception: public exception {
            public:
            virtual const char* what() const throw() {
                return "String of id is not a valid UUID4";
            }
        } invalidID_Exception;
        virtual ElementType getElementType();
        virtual string getElementTypeString();
        virtual string getIDstring();
        void addOwnedElement(Element& el);
        void removeOwnedElement(Element& el);
        Element* getOwner();
        void setOwner(Element* owner);
};
}
#endif