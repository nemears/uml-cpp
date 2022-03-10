#include "uml/abstraction.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"

using namespace UML;

Abstraction::Abstraction() : Element(ElementType::ABSTRACTION) {

}

Abstraction::~Abstraction() {
    mountAndRelease();
}

bool Abstraction::isSubClassOf(ElementType eType) const {
    bool ret = Dependency::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ABSTRACTION;
    }

    return ret;
}