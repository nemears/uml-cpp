#include "uml/primitiveType.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/package.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/interface.h"
#include "uml/deployment.h"

using namespace UML;

PrimitiveType::PrimitiveType() : Element(ElementType::PRIMITIVE_TYPE) {

}

bool PrimitiveType::isSubClassOf(ElementType eType) const {
    bool ret = DataType::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PRIMITIVE_TYPE;
    }

    return ret;
}