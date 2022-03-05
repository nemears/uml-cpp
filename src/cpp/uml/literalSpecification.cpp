#include "uml/literalSpecification.h"
#include "uml/package.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/association.h"
#include "uml/interface.h"
#include "uml/deployment.h"

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