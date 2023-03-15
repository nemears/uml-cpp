#include "uml/types/pin.h"
#include "uml/types/activity.h"
#include "uml/types/activityEdge.h"
#include "uml/types/package.h"
#include "uml/types/property.h"
#include "uml/types/generalization.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interruptibleActivityRegion.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

Pin::Pin() : Element(ElementType::PIN) {
    
}

Pin::~Pin() {
    
}

bool Pin::isControl() const {
    return m_isControl;
}

void Pin::setIsControl(bool val) {
    m_isControl = val;
}

bool Pin::isSubClassOf(ElementType eType) const {
    bool ret = ObjectNode::isSubClassOf(eType);

    if (!ret) {
        ret = MultiplicityElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::PIN;
    }

    return ret;
}