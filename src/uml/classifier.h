#ifndef CLASSIFIER
#define CLASSIFIER
#include <list>
#include "type.h"
#include "property.h"
using namespace std;

class Classifier: public Type {
    protected:
        list<Property*> ownedAttributes;
    public:
        list<Property*> getOwnedAttributes();
};
#endif