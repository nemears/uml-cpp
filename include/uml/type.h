#ifndef TYPEH
#define TYPEH
#include "packageableElement.h"
using namespace std;

namespace UML{
    class Type : virtual public PackageableElement {
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