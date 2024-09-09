#include "uml/types/namedElement.h"
#include "uml/types/packageableElement.h"
#include "uml/types/parameterableElement.h"
#include "uml/types/structuredClassifier.h"
#include "uml/uml-stable.h"

using namespace UML;

void EncapsulatedClassifier::OwnedAttributePolicy::elementAdded(Property& el, EncapsulatedClassifier& me) {
    if (el.is<Port>() && !me.m_ownedPorts.contains(el.getID())) {
        me.m_ownedPorts.innerAdd(&el);
    }
}

void EncapsulatedClassifier::OwnedAttributePolicy::elementRemoved(Property& el, EncapsulatedClassifier& me) {
    if (el.is<Port>() && me.m_ownedPorts.contains(el.getID())) {
        me.m_ownedPorts.innerRemove(&el);
    }
}

EncapsulatedClassifier::EncapsulatedClassifier(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    PackageableElement(elementType, manager),
    Classifier(elementType, manager),
    StructuredClassifier(elementType, manager)
{
    m_ownedPorts.subsets(m_ownedAttributes);
    m_encapsulatedClassifierOwnedAttributes.redefines(m_ownedAttributes);
}

ReadOnlyIndexableSet<Port, EncapsulatedClassifier>& EncapsulatedClassifier::getOwnedPorts() {
    return m_ownedPorts;
}
