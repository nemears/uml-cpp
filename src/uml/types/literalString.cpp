#include "uml/types/literalString.h"
#include "uml/types/package.h"
#include "uml/types/type.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"

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