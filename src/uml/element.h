#ifndef ELEMENT
#define ELEMENT
#include <string>
//#include <boost/uuid/uuid.hpp>
//#include <boost/uuid/uuid_generators.hpp>
//#include <boost/uuid/uuid_io.hpp>
using namespace std;

class Element {
    protected:
    //boost::uuids::uuid uuid;

    public:
    Element() {
        //uuid = boost::uuids::random_generator()();
    };
    virtual ~Element() {};
    //boost::uuids::uuid getID();
};
#endif