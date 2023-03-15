#include "uml/types/primitiveType.h"
#include "uml/types/property.h"
#include "uml/types/generalization.h"
#include "uml/types/package.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"

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