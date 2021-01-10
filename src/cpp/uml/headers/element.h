#ifndef ELEMENT
#define ELEMENT
#include <string>
#include <list>
#include <regex>
#include <exception>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
using namespace std;

namespace UML{

static bool isValidUUID4(string strn);

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
            virtual const char* what() const throw() {
                return "String of id is not a valid UUID4";
            }
        } invalidID_Exception;
};
}
#endif