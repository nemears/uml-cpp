#ifndef MODELH
#define MODELH

#include "package.h"
#include <string>
using namespace std;

namespace UML{
class Model : public Package{
    public:
        ElementType getElementType() override;
        bool isSubClassOf(ElementType eType) override;
};
}
#endif