#include "uml/types/abstraction.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/deployment.h"

using namespace UML;

Abstraction::Abstraction() : Element(ElementType::ABSTRACTION) {

}

Abstraction::~Abstraction() {
    
}

bool Abstraction::isSubClassOf(ElementType eType) const {
    bool ret = Dependency::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ABSTRACTION;
    }

    return ret;
}