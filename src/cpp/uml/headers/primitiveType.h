#ifndef PRIMITIVETYPE
#define PRIMITIVETYPE
#include "classifier.h"

namespace UML{
class PrimitiveType : public Classifier {
    public:
        enum Primitive {INT, REAL, BOOL, STRING};
        void setPrimitiveType(Primitive primitive);
        Primitive getPrimitiveType();
    private:
        Primitive primitiveType;
};
}
#endif