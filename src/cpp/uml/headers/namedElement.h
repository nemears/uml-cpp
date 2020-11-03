#ifndef NAMEDELEMENT
#define NAMEDELEMENT
#include <string>
#include "element.h"
using namespace std;

class NamedElement : public Element {
    protected:
        string name;
    
    public:
        string getName();
        void setName(const string &name);
};

#endif