#include "uml/types/namedElement.h"
#include "uml/types/parameterableElement.h"
#include "uml/types/valueSpecification.h"
#include "uml/uml-stable.h"

using namespace UML;

Singleton<InstanceSpecification, InstanceValue>& InstanceValue::getInstanceSingleton() {
    return m_instance;
}

InstanceValue::InstanceValue(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    ValueSpecification(elementType, manager)
{}

InstanceSpecificationPtr  InstanceValue::getInstance() const {
    return m_instance.get();
}

void InstanceValue::setInstance(InstanceSpecificationPtr inst) {
    m_instance.set(inst);
}

void InstanceValue::setInstance(InstanceSpecification& inst) {
    m_instance.set(inst);
}

void InstanceValue::setInstance(ID id) {
    m_instance.set(id);
}
