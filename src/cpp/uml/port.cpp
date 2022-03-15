#include "uml/port.h"
#include "uml/operation.h"
#include "uml/manifestation.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"
#include "uml/setReferenceFunctor.h"
#include "uml/umlPtr.h"

using namespace UML;

void Port::setPortInterfaces(BehavioredClassifier& clazz) {
    for (auto& realization : clazz.getInterfaceRealizations()) {
        if (realization.getContract()) {
            if (isConjugated()) {
                m_required.addReadOnly(realization.getContract().id());
            } else {
               m_provided.addReadOnly(realization.getContract().id());
            }
        }
    }
    for (auto& dependency : clazz.getClientDependencies()) {
        if (dependency.isSubClassOf(ElementType::USAGE)) {
            for (auto& supplier : dependency.getSupplier()) {
                if (supplier.isSubClassOf(ElementType::INTERFACE_UML)) {
                    if (isConjugated()) {
                       m_provided.nonOppositeAdd(supplier.as<Interface>());
                    } else {
                        m_required.nonOppositeAdd(supplier.as<Interface>());
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

void Port::SetTypeFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::BEHAVIORED_CLASSIFIER)) {
        m_el.as<Port>().setPortInterfaces(el.as<BehavioredClassifier>());
    } else if (el.isSubClassOf(ElementType::INTERFACE_UML)) {
        if (m_el.as<Port>().isConjugated()) {
            m_el.as<Port>().m_required.nonOppositeAdd(el.as<Interface>());
        } else {
            m_el.as<Port>().m_provided.nonOppositeAdd(el.as<Interface>());
        }
    }
}

void Port::removePortInterfaces(BehavioredClassifier& clazz) {
    for (auto& realization : clazz.getInterfaceRealizations()) {
        if (realization.getContract()) {
            if (isConjugated()) {
                m_required.removeReadOnly(realization.getContract().id());
            } else {
               m_provided.removeReadOnly(realization.getContract().id());
            }
        }
    }
    for (auto& dependency : clazz.getClientDependencies()) {
        if (dependency.isSubClassOf(ElementType::USAGE)) {
            for (auto& supplier : dependency.getSupplier()) {
                if (supplier.isSubClassOf(ElementType::INTERFACE_UML)) {
                    if (isConjugated()) {
                       m_provided.removeReadOnly(supplier.getID());
                    } else {
                        m_required.removeReadOnly(supplier.getID());
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

void Port::RemoveTypeFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::BEHAVIORED_CLASSIFIER)) {
        m_el.as<Port>().removePortInterfaces(el.as<BehavioredClassifier>());
    } else if (el.isSubClassOf(ElementType::INTERFACE_UML)) {
        if (m_el.as<Port>().isComposite()) {
            m_el.as<Port>().m_required.removeReadOnly(el.getID());
        } else {
            m_el.as<Port>().m_provided.removeReadOnly(el.getID());
        }
    }
}

void Port::referencingReleased(ID id) {
    Property::referencingReleased(id);
    m_required.release(id);
    m_provided.release(id);
}

void Port::restoreReference(Element* el) {
    Property::restoreReference(el);
    if (m_required.contains(el->getID())){
        el->setReference(this);
    }
    if (m_provided.contains(el->getID())) {
        el->setReference(this);
    }
}

void Port::referenceReindexed(ID oldID, ID newID) {
    Property::referenceReindexed(oldID, newID);
    m_required.reindex(oldID, newID);
    m_provided.reindex(oldID, newID);
}

void Port::reindexName(ID id, std::string newName) {
    Property::reindexName(id, newName);
    m_required.reindexName(id, newName);
    m_provided.reindexName(id, newName);
}

void Port::referenceErased(ID id) {
    Property::referenceErased(id);
    m_required.eraseElement(id);
    m_provided.eraseElement(id);
}

void Port::init() {
    m_type.m_addFunctors.insert(new SetTypeFunctor(this));
    m_type.m_removeFunctors.insert(new RemoveTypeFunctor(this));
    m_required.m_readOnly = true;
    m_required.m_signature = &Port::getRequired;
    m_required.m_addFunctors.insert(new SetReferenceFunctor(this));
    m_required.m_removeFunctors.insert(new RemoveReferenceFunctor(this));
    m_provided.m_readOnly = true;
    m_provided.m_signature = &Port::getProvided;
    m_provided.m_addFunctors.insert(new SetReferenceFunctor(this));
    m_provided.m_removeFunctors.insert(new RemoveReferenceFunctor(this));
}

Port::Port() : Element(ElementType::PORT) {
    init();
}

Port::~Port() {
    mountAndRelease();
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
        std::vector<Interface*> oldRequired;
        std::vector<Interface*> oldProvided;
        for (auto& required : m_required) {
            oldRequired.push_back(&required);
        }
        for (auto& provided : m_provided) {
            oldProvided.push_back(&provided);
        }
        for (auto& newProvided : oldRequired) {
            m_required.removeReadOnly(newProvided->getID());
            m_provided.nonOppositeAdd(*newProvided);
        }
        for (auto& newRequired : oldProvided) {
            m_provided.removeReadOnly(newRequired->getID());
            m_required.nonOppositeAdd(*newRequired);
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