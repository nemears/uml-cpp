#include "uml/outputPin.h"
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

OutputPin::OutputPin() : Element(ElementType::OUTPUT_PIN) {

}

OutputPin::~OutputPin() {

}

bool OutputPin::isSubClassOf(ElementType eType) const {
    bool ret = Pin::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::OUTPUT_PIN;
    }

    return ret;
}