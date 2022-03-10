#include "uml/literalString.h"
#include "uml/package.h"
#include "uml/type.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/interface.h"
#include "uml/deployment.h"

using namespace UML;

LiteralString::LiteralString() : Element(ElementType::LITERAL_STRING) {

}

std::string LiteralString::getValue() {
    return m_value;
}

void LiteralString::setValue(std::string val) {
    m_value = val;
}

bool LiteralString::isSubClassOf(ElementType eType) const {
    bool ret = LiteralSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_STRING;
    }

    return ret;
}