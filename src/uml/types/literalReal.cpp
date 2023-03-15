#include "uml/types/literalReal.h"
#include "uml/types/package.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"

using namespace UML;

LiteralReal::LiteralReal() : Element(ElementType::LITERAL_REAL) {

}

double LiteralReal::getValue() {
    return m_value;
}

void LiteralReal::setValue(double val) {
    m_value = val;
}

bool LiteralReal::isSubClassOf(ElementType eType) const {
    bool ret = LiteralSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_REAL;
    }

    return ret;
}