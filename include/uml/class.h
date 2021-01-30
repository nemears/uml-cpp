#ifndef CLASSH
#define CLASSH
#include <list>
#include "classifier.h"
using namespace std;

namespace UML{

    /**
     * This is a forward declaration of Operation because the method field in Operation is of type Behavior
     * which inherits from this class (Class), so we just declare operation here and include the header in class.cpp
     **/
    class Operation;

    /**
     * Definition of class here
     **/
    class Class : public Classifier {
        public:
            list<Operation*> operations;
            ElementType getElementType() override;
    };
}

#endif