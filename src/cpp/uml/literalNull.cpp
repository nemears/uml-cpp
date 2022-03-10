#include "uml/literalNull.h"
#include "uml/package.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/interface.h"
#include "uml/deployment.h"

using namespace UML;

LiteralNull::LiteralNull() : Element(ElementType::LITERAL_NULL) {

}

bool LiteralNull::isSubClassOf(ElementType eType) const {
    bool ret = LiteralSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_NULL;
    }

    return ret;
}