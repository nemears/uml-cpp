#include "uml/valuePin.h"
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

Set<ValueSpecification, ValuePin>& ValuePin::getValueSingleton() {
    return m_value;
}

void ValuePin::init() {
    m_value.subsets(*m_ownedElements);
}

ValuePin::ValuePin() : Element(ElementType::VALUE_PIN) {
    init();
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