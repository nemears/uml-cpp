#ifndef MODELH
#define MODELH
#include "namespace.h"
#include <string>
using namespace std;

namespace UML{
class Model : public Namespace {
    public:
        ElementType getElementType() override;
};
}
#endif