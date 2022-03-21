#include "uml/pin.h"
#include "uml/activity.h"
#include "uml/activityEdge.h"
#include "uml/package.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interruptibleActivityRegion.h"
#include "uml/interface.h"
#include "uml/deployment.h"
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