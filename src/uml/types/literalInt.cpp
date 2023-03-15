#include "uml/types/literalInt.h"
#include "uml/types/package.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"

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