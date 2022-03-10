#include "uml/literalBool.h"
#include "uml/package.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/interface.h"
#include "uml/deployment.h"

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