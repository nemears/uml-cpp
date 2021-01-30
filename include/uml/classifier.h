#ifndef CLASSIFIERH
#define CLASSIFIERH
#include <list>
#include "type.h"
#include "property.h"
using namespace std;

namespace UML{
class Classifier: public Type {
    protected:
        
    public:
        list<Property*> ownedAttributes;
        ElementType getElementType() override;
};
}
#endif