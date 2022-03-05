#include "uml/literalReal.h"
#include "uml/package.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/interface.h"
#include "uml/deployment.h"

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