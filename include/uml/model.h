#ifndef MODELH
#define MODELH

#include "package.h"

namespace UML{
class Model : public Package{
    public:
        ElementType getElementType() override;
        bool isSubClassOf(ElementType eType) override;
};
}
#endif