#ifndef NAMEDELEMENTH
#define NAMEDELEMENTH
#include <string>
#include "element.h"
using namespace std;

namespace UML{
class NamedElement : public Element {
    protected:
        string name; // Move this to public ?
    
    public:
        virtual string getName();
        virtual void setName(const string &name);
        ElementType getElementType() override;
};
}

#endif