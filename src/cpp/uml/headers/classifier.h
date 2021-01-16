#ifndef CLASSIFIER
#define CLASSIFIER
#include <list>
#include "type.h"
#include "property.h"
using namespace std;

namespace UML{
class Classifier: public Type {
    protected:
        
    public:
        list<Property*> ownedAttributes;
        //list<Property*> getOwnedAttributes();
};
}
#endif