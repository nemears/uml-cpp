#include "uml/types/literalBool.h"
#include "uml/types/package.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"

using namespace UML;

LiteralBool::LiteralBool() : Element(ElementType::LITERAL_BOOL) {

}

bool LiteralBool::getValue() {
    return m_value;;
}

void LiteralBool::setValue(bool val) {
    m_value = val;
}

bool LiteralBool::isSubClassOf(ElementType eType) const {
    bool ret = LiteralSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_BOOL;
    }

    return ret;
}