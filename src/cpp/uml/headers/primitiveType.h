#ifndef PRIMITIVETYPE
#define PRIMITIVETYPE
#include "classifier.h"

namespace UML{
class PrimitiveType : public Classifier {
    public:
        enum Primitive {INT = 0, REAL = 1, BOOL = 2, STRING = 3};
        void setPrimitiveType(Primitive primitive);
        Primitive getPrimitiveType();
    private:
        Primitive primitiveType;
};
}
#endif