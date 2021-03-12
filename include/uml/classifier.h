#ifndef CLASSIFIERH
#define CLASSIFIERH
#include <list>
#include "type.h"
#include "property.h"
#include "sequence.h"
using namespace std;

namespace UML{
class Classifier: public Type {
    protected:
        Sequence<Property>* m_attributes;
        
    public:
        Classifier();
        ~Classifier();
        Sequence<Property>& getAttributes();
        ElementType getElementType() override;
        bool isPrimitive() override;
};
}
#endif