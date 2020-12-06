#ifndef PRIMITIVETYPE
#define PRIMITIVETYPE
#include "classifier.h"

class PrimitiveType : public Classifier {
    public:
        enum primitives {INT, REAL, BOOL, STRING};
};
#endif