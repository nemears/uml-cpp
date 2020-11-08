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
        // Moved everything to public for now

    public:
        list<Element*> ownedElements;
        boost::uuids::uuid uuid;
        Element() {
            uuid = boost::uuids::random_generator()();
        };
        virtual ~Element() {};
        void setID(string id);
};
#endif