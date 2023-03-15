#include "uml/types/literalSpecification.h"
#include "uml/types/package.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/association.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"

using namespace UML;

LiteralSpecification::LiteralSpecification() : Element(ElementType::LITERAL_SPECIFICATION) {

}

bool LiteralSpecification::isSubClassOf(ElementType eType) const {
    bool ret = ValueSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_SPECIFICATION;
    }

    return ret;
}