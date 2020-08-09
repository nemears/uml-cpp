#ifndef CLASSIFIER
#define CLASSIFIER
#include <list>
#include "namedElement.h"
#include "property.h"
using namespace std;

class Classifier: public NamedElement {
    protected:
        list<Property*> ownedAttributes;
    public:
        list<Property*> getOwnedAttributes();
};
#endif