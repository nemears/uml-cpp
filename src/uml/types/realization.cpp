#include "uml/types/realization.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/deployment.h"

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