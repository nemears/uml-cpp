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
    BEHAVIOR,
    CLASS,
    CLASSIFIER,
    ELEMENT,
    INSTANCE_SPECIFICATION,
    INSTANCE_VALUE,
    LITERAL_BOOL,
    LITERAL_INT,
    LITERAL_REAL,
    LITERAL_STRING,
    MODEL,
    NAMED_ELEMENT,
    NAMESPACE,
    OPAQUE_BEHAVIOR,
    OPERATION,
    PARAMETER,
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
        // Moved everything to public for now

    public:
        list<Element*> ownedElements;
        boost::uuids::uuid uuid;
        Element() {
            uuid = boost::uuids::random_generator()();
        };
        virtual ~Element() {};
        void setID(string id);
        class InvalidID_Exception: public exception {
            public:
            virtual const char* what() const throw() {
                return "String of id is not a valid UUID4";
            }
        } invalidID_Exception;
        virtual ElementType getElementType();
        string getElementTypeString();
        string getIDstring();
        void addOwnedElement(Element& el);
        void removeOwnedElement(Element& el);
};
}
#endif