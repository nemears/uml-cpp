#ifndef ASSOCIATION_H
#define ASSOCIATION_H

#include "classifier.h"
#include "relationship.h"

namespace UML {
    class Association : public Classifier, public Relationship {
        protected:
        public:
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType);
    };
}

#endif