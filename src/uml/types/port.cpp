#include "uml/uml-stable.h"

using namespace UML;

void Port::setPortInterfaces(BehavioredClassifier& clazz) {
    for (auto& realization : clazz.getInterfaceRealizations()) {
        if (realization.getContract()) {
            if (isConjugated()) {
                m_required.innerAdd(realization.getContract());
            } else {
               m_provided.innerAdd(realization.getContract());
            }
        }
    }
    for (auto& dependency : clazz.getClientDependencies()) {
        if (dependency.is(ElementType::USAGE)) {
            for (auto& supplier : dependency.getSuppliers()) {
                if (supplier.is(ElementType::INTERFACE_UML)) {
                    if (isConjugated()) {
                       m_provided.innerAdd(&supplier);
                    } else {
                        m_required.innerAdd(&supplier);
                    }
                }
            }
        }
    }
    for (auto& general : clazz.getGenerals()) {
        if (general.is(ElementType::BEHAVIORED_CLASSIFIER)) {
            setPortInterfaces(general.as<BehavioredClassifier>());
        }
    }
}

void Port::TypePolicy::elementAdded(Type& el, Port& me) {
    if (el.is(ElementType::BEHAVIORED_CLASSIFIER)) {
        me.setPortInterfaces(el.as<BehavioredClassifier>());
    } else if (el.is(ElementType::INTERFACE_UML)) {
        if (me.isConjugated()) {
            me.m_required.innerAdd(&el);
        } else {
            me.m_provided.innerAdd(&el);
        }
    }
}

void Port::removePortInterfaces(BehavioredClassifier& clazz) {
    for (auto& realization : clazz.getInterfaceRealizations()) {
        if (realization.getContract()) {
            if (isConjugated()) {
                m_required.innerRemove(realization.getContract());
            } else {
               m_provided.innerRemove(realization.getContract());
            }
        }
    }
    for (auto& dependency : clazz.getClientDependencies()) {
        if (dependency.is(ElementType::USAGE)) {
            for (auto& supplier : dependency.getSuppliers()) {
                if (supplier.is(ElementType::INTERFACE_UML)) {
                    if (isConjugated()) {
                       m_provided.innerRemove(&supplier);
                    } else {
                        m_required.innerRemove(&supplier);
                    }
                }
            }
        }
    }
    for (auto& general : clazz.getGenerals()) {
        if (general.is(ElementType::BEHAVIORED_CLASSIFIER)) {
            removePortInterfaces(general.as<BehavioredClassifier>());
        }
    }
}

void Port::TypePolicy::elementRemoved(Type& el, Port& me) {
    if (el.is(ElementType::BEHAVIORED_CLASSIFIER)) {
        me.removePortInterfaces(el.as<BehavioredClassifier>());
    } else if (el.is(ElementType::INTERFACE_UML)) {
        if (me.isConjugated()) {
            me.m_required.innerRemove(&el);
        } else {
            me.m_provided.innerRemove(&el);
        }
    }
}

void Port::referenceErased(ID id) {
    Property::referenceErased(id);
    eraseFromSet(id, m_required);
    eraseFromSet(id, m_provided);
}

void Port::restoreReferences() {
    if (m_type.get()) {
        if (m_type.get()->is(ElementType::BEHAVIORED_CLASSIFIER)) {
            setPortInterfaces(m_type.get()->as<BehavioredClassifier>());
        } else if (m_type.get()->is(ElementType::INTERFACE_UML)) {
            if (isConjugated() && !m_required.contains(m_type.get().id())) {
                m_required.innerAdd(m_type.get());
            } else if (!isConjugated() && !m_provided.contains(m_type.get().id())) {
                m_provided.innerAdd(m_type.get());
            }
        }
    }
}

Port::Port() : Element(ElementType::PORT) {
    m_portType.redefines(m_type);
}

Port::~Port() {
    
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

ReadOnlySet<Interface, Port>& Port::getRequired() {
    return m_required;
}

ReadOnlySet<Interface, Port>& Port::getProvided() {
    return m_provided;
}

bool Port::is(ElementType eType) const {
    bool ret = Property::is(eType);

    if (!ret) {
        ret = eType == ElementType::PORT;
    }

    return ret;
}
