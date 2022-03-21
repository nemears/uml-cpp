#include "uml/inputPin.h"
#include "uml/exceptionHandler.h"
#include "uml/activity.h"
#include "uml/activityEdge.h"
#include "uml/package.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/interruptibleActivityRegion.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/objectNode.h"
#include "uml/umlPtr.h"

using namespace UML;

InputPin::InputPin() : Element(ElementType::INPUT_PIN) {

}

InputPin::~InputPin() {

}

bool InputPin::isSubClassOf(ElementType eType) const {
    bool ret = Pin::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::INPUT_PIN;
    }

    return ret;
}