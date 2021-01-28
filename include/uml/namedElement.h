#ifndef NAMEDELEMENT
#define NAMEDELEMENT
#include <string>
#include "element.h"
using namespace std;

namespace UML{
class NamedElement : public Element {
    protected:
        string name; // Move this to public ?
    
    public:
        string getName();
        void setName(const string &name);
};
}

#endif