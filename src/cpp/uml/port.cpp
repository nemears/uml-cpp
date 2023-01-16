#include "uml/port.h"
#include "uml/operation.h"
#include "uml/manifestation.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

void Port::setPortInterfaces(BehavioredClassifier& clazz) {
    for (auto& realization : clazz.getInterfaceRealizations()) {
        if (realization.getContract()) {
            if (isConjugated()) {
                m_required.addReadOnly(*realization.getContract());
            } else {
               m_provided.addReadOnly(*realization.getContract());
            }
        }
    }
    for (auto& dependency : clazz.getClientDependencies()) {
        if (dependency.isSubClassOf(ElementType::USAGE)) {
            for (auto& supplier : dependency.getSuppliers()) {
                if (supplier.isSubClassOf(ElementType::INTERFACE_UML)) {
                    if (isConjugated()) {
                       m_provided.innerAdd(supplier.as<Interface>());
                    } else {
                        m_required.innerAdd(supplier.as<Interface>());
                    }
                }
            }
        }
    }
    for (auto& general : clazz.getGenerals()) {
        if (general.isSubClassOf(ElementType::BEHAVIORED_CLASSIFIER)) {
            setPortInterfaces(general.as<BehavioredClassifier>());
        }
    }
}

void Port::SetTypePolicy::apply(Type& el, Port& me) {
    if (el.isSubClassOf(ElementType::BEHAVIORED_CLASSIFIER)) {
        me.setPortInterfaces(el.as<BehavioredClassifier>());
    } else if (el.isSubClassOf(ElementType::INTERFACE_UML)) {
        if (me.isConjugated()) {
            me.m_required.innerAdd(el.as<Interface>());
        } else {
            me.m_provided.innerAdd(el.as<Interface>());
        }
    }
}

void Port::removePortInterfaces(BehavioredClassifier& clazz) {
    for (auto& realization : clazz.getInterfaceRealizations()) {
        if (realization.getContract()) {
            if (isConjugated()) {
                m_required.innerRemove(realization.getContract().id());
            } else {
               m_provided.innerRemove(realization.getContract().id());
            }
        }
    }
    for (auto& dependency : clazz.getClientDependencies()) {
        if (dependency.isSubClassOf(ElementType::USAGE)) {
            for (auto& supplier : dependency.getSuppliers()) {
                if (supplier.isSubClassOf(ElementType::INTERFACE_UML)) {
                    if (isConjugated()) {
                       m_provided.innerRemove(supplier.getID());
                    } else {
                        m_required.innerRemove(supplier.getID());
                    }
                }
            }
        }
    }
    for (auto& general : clazz.getGenerals()) {
        if (general.isSubClassOf(ElementType::BEHAVIORED_CLASSIFIER)) {
            removePortInterfaces(general.as<BehavioredClassifier>());
        }
    }
}

void Port::RemoveTypePolicy::apply(Type& el, Port& me) {
    if (el.isSubClassOf(ElementType::BEHAVIORED_CLASSIFIER)) {
        me.removePortInterfaces(el.as<BehavioredClassifier>());
    } else if (el.isSubClassOf(ElementType::INTERFACE_UML)) {
        if (me.isConjugated()) {
            me.m_required.removeReadOnly(el.getID());
        } else {
            me.m_provided.removeReadOnly(el.getID());
        }
    }
}

void Port::restoreReference(Element* el) {
    Property::restoreReference(el);
}

void Port::referenceReindexed(ID newID) {
    Property::referenceReindexed(newID);
    m_required.reindex(newID);
    m_provided.reindex(newID);
}

void Port::referenceErased(ID id) {
    Property::referenceErased(id);
    m_required.eraseElement(id);
    m_provided.eraseElement(id);
}

Port::Port() : Element(ElementType::PORT) {
    m_required.m_readOnly = true;
    m_provided.m_readOnly = true;
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
            m_required.innerRemove(newProvided.id());
            m_provided.innerAdd(*newProvided);
        }
        for (auto& newRequired : oldProvided) {
            m_provided.innerRemove(newRequired.id());
            m_required.innerAdd(*newRequired);
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

Set<Interface, Port>& Port::getRequired() {
    return m_required;
}

Set<Interface, Port>& Port::getProvided() {
    return m_provided;
}

bool Port::isSubClassOf(ElementType eType) const {
    bool ret = Property::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PORT;
    }

    return ret;
}