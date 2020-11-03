#ifndef ELEMENT
#define ELEMENT
#include <string>
#include <list>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
using namespace std;

class Element {
    protected:
        boost::uuids::uuid uuid;
        list<Element*> ownedElements;

    public:
        Element() {
            uuid = boost::uuids::random_generator()();
        };
        virtual ~Element() {};
        boost::uuids::uuid getID();
        void setID(string id);
        list<Element*> getOwnedElements();
};
#endif