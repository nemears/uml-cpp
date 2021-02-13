#ifndef PRIMITIVETYPEH
#define PRIMITIVETYPEH
#include "classifier.h"

namespace UML{
class PrimitiveType : public Classifier {
    public:
        enum Primitive {INT = 0, REAL = 1, BOOL = 2, STRING = 3, NONE = 4};
        void setPrimitiveType(Primitive primitive);
        Primitive getPrimitiveType();
        bool isPrimitive() override;
        ElementType getElementType() override;
        string getPrimitiveTypeString();
        void setPrimitiveTypeString(const string& primitiveString);
    private:
        Primitive primitiveType;
};
}
#endif