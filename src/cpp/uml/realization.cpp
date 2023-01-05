#include "uml/realization.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"

using namespace UML;

Realization::Realization() : Element(ElementType::REALIZATION) {

}

Realization::~Realization() {
    
}

bool Realization::isSubClassOf(ElementType eType) const {
    bool ret = Abstraction::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::REALIZATION;
    }

    return ret;
}