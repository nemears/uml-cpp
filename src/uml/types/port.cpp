#include "uml/types/namedElement.h"
#include "uml/types/redefinableElement.h"
#include "uml/uml-stable.h"

using namespace UML;

void Port::setPortInterfaces(BehavioredClassifier& clazz) {
    for (auto& realization : clazz.getInterfaceRealizations()) {
        if (realization.getContract()) {
            if (isConjugated()) {
                if (!m_required.contains(realization.getContract())) {
                    m_required.innerAdd(realization.getContract());
                }
            } else {
                if (!m_provided.contains(realization.getContract())) {
                    m_provided.innerAdd(realization.getContract());
                }
            }
        }
    }
    for (auto& dependency : clazz.getClientDependencies()) {
        if (dependency.is<Usage>()) {
            for (auto& supplier : dependency.getSuppliers()) {
                if (supplier.is<Interface>()) {
                    if (isConjugated()) {
                        if (!m_provided.contains(&supplier)) {
                            m_provided.innerAdd(&supplier);
                        }
                    } else {
                        if (!m_required.contains(&supplier)) {
                            m_required.innerAdd(&supplier);
                        }
                    }
                }
            }
        }
    }
    for (auto& general : clazz.getGenerals()) {
        if (general.is<BehavioredClassifier>()) {
            setPortInterfaces(general.as<BehavioredClassifier>());
        }
    }
}

void Port::TypePolicy::elementAdded(Type& el, Port& me) {
    if (el.is<BehavioredClassifier>()) {
        me.setPortInterfaces(el.as<BehavioredClassifier>());
    } else if (el.is<Interface>()) {
        if (me.isConjugated()) {
            if (!me.m_required.contains(&el)) {
                me.m_required.innerAdd(&el);
            }
        } else {
            if (!me.m_provided.contains(&el)) {
                me.m_provided.innerAdd(&el);
            }
        }
    }
}

void Port::removePortInterfaces(BehavioredClassifier& clazz) {
    for (auto& realization : clazz.getInterfaceRealizations()) {
        if (realization.getContract()) {
            if (isConjugated()) {
                if (!m_required.contains(realization.getContract())) {
                    m_required.innerRemove(realization.getContract());
                }
            } else {
                if (m_provided.contains(realization.getContract())) {
                    m_provided.innerRemove(realization.getContract());
                }
            }
        }
    }
    for (auto& dependency : clazz.getClientDependencies()) {
        if (dependency.is<Usage>()) {
            for (auto& supplier : dependency.getSuppliers()) {
                if (supplier.is<Interface>()) {
                    if (isConjugated()) {
                        if (!m_provided.contains(&supplier)) {
                            m_provided.innerRemove(&supplier);
                        }
                    } else {
                        if (!m_required.contains(&supplier)) {
                            m_required.innerRemove(&supplier);
                        }
                    }
                }
            }
        }
    }
    for (auto& general : clazz.getGenerals()) {
        if (general.is<BehavioredClassifier>()) {
            removePortInterfaces(general.as<BehavioredClassifier>());
        }
    }
}

void Port::TypePolicy::elementRemoved(Type& el, Port& me) {
    if (el.is<BehavioredClassifier>()) {
        me.removePortInterfaces(el.as<BehavioredClassifier>());
    } else if (el.is<Interface>()) {
        if (me.isConjugated()) {
            if (!me.m_required.contains(&el)) {
                me.m_required.innerRemove(&el);
            }
        } else {
            if (!me.m_provided.contains(&el)) {
                me.m_provided.innerRemove(&el);
            }
        }
    }
}

Port::Port(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    TypedElement(elementType, manager),
    RedefinableElement(elementType, manager),
    ParameterableElement(elementType, manager),
    Property(elementType, manager)
{
    m_portType.redefines(m_type);
}

bool Port::isBehavior() const {
    return m_isBehavior;
}

void Port::setIsBehavior(bool isBehavior) {
    m_isBehavior = isBehavior;
}

bool Port::isConjugated() const {
    return m_isConjugated;
}

void Port::setIsConjugated(bool isConjugated) {
    if (isConjugated != m_isConjugated) {
        std::vector<InterfacePtr> oldRequired;
        std::vector<InterfacePtr> oldProvided;
        for (const InterfacePtr required : m_required.ptrs()) {
            oldRequired.push_back(required);
        }
        for (const InterfacePtr provided : m_provided.ptrs()) {
            oldProvided.push_back(provided);
        }
        for (auto& newProvided : oldRequired) {
            m_required.innerRemove(newProvided);
            m_provided.innerAdd(newProvided);
        }
        for (auto& newRequired : oldProvided) {
            m_provided.innerRemove(newRequired);
            m_required.innerAdd(newRequired);
        }
    }
    m_isConjugated = isConjugated;
}

bool Port::isService() const {
    return m_isService;
}

void Port::setIsService(bool isService) {
    m_isService = isService;
}

ReadOnlyIndexableSet<Interface, Port>& Port::getRequired() {
    return m_required;
}

ReadOnlyIndexableSet<Interface, Port>& Port::getProvided() {
    return m_provided;
}
