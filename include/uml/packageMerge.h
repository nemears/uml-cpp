#ifndef PACKAGE_MERGE_H
#define PACKAGE_MERGE_H

#include "directedRelationship.h"

namespace UML {
    class PackageMerge : public DirectedRelationship {
        protected:

        public:
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif