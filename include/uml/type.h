#ifndef TYPEH
#define TYPEH
#include "namedElement.h"
using namespace std;

namespace UML{
    class Type : virtual public NamedElement {
        public:
            virtual bool isPrimitive() = 0;
            ElementType getElementType() override;
            Type& operator=(Type&&) {
                return *this;
            };
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif