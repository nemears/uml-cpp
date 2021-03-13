#ifndef NAMEDELEMENTH
#define NAMEDELEMENTH
#include <string>
#include "element.h"
using namespace std;

namespace UML{
    class NamedElement : virtual public Element {
        protected:
            string name;
        
        public:
            virtual string getName();
            virtual void setName(const string &name);
            ElementType getElementType() override;
            NamedElement(){};
    };
}

#endif