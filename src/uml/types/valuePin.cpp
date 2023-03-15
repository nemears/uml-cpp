#include "uml/types/valuePin.h"
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

TypedSet<ValueSpecification, ValuePin>& ValuePin::getValueSingleton() {
    return m_value;
}

ValuePin::ValuePin() : Element(ElementType::VALUE_PIN) {
    m_value.subsets(*m_ownedElements);
}

ValuePin::~ValuePin() {

}

ValueSpecificationPtr ValuePin::getValue() const {
    return m_value.get();
}

void ValuePin::setValue(ValueSpecification* value) {
    m_value.set(value);
}

void ValuePin::setValue(ValueSpecification& value) {
    m_value.set(value);
}

void ValuePin::setValue(ValueSpecificationPtr value) {
    m_value.set(value);
}

void ValuePin::setValue(ID id) {
    m_value.set(id);
}

bool ValuePin::isSubClassOf(ElementType eType) const {
    bool ret = InputPin::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::VALUE_PIN;
    }

    return ret;
}