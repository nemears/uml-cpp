#include "uml/types/outputPin.h"
#include "uml/types/exceptionHandler.h"
#include "uml/types/activity.h"
#include "uml/types/activityEdge.h"
#include "uml/types/package.h"
#include "uml/types/property.h"
#include "uml/types/generalization.h"
#include "uml/types/interruptibleActivityRegion.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
#include "uml/types/objectNode.h"
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