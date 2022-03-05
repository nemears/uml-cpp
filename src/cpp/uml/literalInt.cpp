#include "uml/literalInt.h"
#include "uml/package.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/interface.h"
#include "uml/deployment.h"

using namespace UML;

LiteralInt::LiteralInt() : Element(ElementType::LITERAL_INT) {

}

int LiteralInt::getValue() {
    return m_value;
}

void LiteralInt::setValue(int val) {
    m_value = val;
}

bool LiteralInt::isSubClassOf(ElementType eType) const {
    bool ret = LiteralSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_INT;
    }

    return ret;
}