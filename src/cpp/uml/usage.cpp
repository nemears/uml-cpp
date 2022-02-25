#include "uml/usage.h"
#include "uml/uml-stable.h"

using namespace UML;

void Usage::RemoveClientFunctor::operator()(Element& el) const {
    for (auto& supplier : m_el.as<Usage>().m_supplier) {
        if (supplier.isSubClassOf(ElementType::INTERFACE) && el.isSubClassOf(ElementType::CLASSIFIER)) {
            for (auto& pair : el.m_node->m_references) {
                std::list<Classifier*> queue = {&el.as<Classifier>()};
                while (!queue.empty()) {
                    Classifier* front = queue.front();
                    queue.pop_front();
                    for (auto& pair : front->m_node->m_references) {
                        if (pair.second) {
                            if (pair.second->m_managerElementMemory->isSubClassOf(ElementType::PORT)) {
                                if (pair.second->m_managerElementMemory->as<Port>().isConjugated()) {
                                    if (pair.second->m_managerElementMemory->as<Port>().getProvided().contains(supplier.getID())) {
                                        pair.second->m_managerElementMemory->as<Port>().getProvided().removeReadOnly(supplier.getID());
                                    }
                                } else {
                                    if (pair.second->m_managerElementMemory->as<Port>().getRequired().contains(supplier.getID())) {
                                        pair.second->m_managerElementMemory->as<Port>().getRequired().removeReadOnly(supplier.getID());
                                    }
                                }
                            } else if (pair.second->m_managerElementMemory->isSubClassOf(ElementType::CLASSIFIER)) {
                                if (pair.second->m_managerElementMemory->as<Classifier>().getGenerals().contains(*front)) {
                                    queue.push_back(&pair.second->m_managerElementMemory->as<Classifier>());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void Usage::SetClientFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::CLASSIFIER)) {
        for (auto& supplier : m_el.as<Usage>().getSupplier()) {
            if (supplier.isSubClassOf(ElementType::INTERFACE)) {
                std::list<Classifier*> queue = {&el.as<Classifier>()};
                while(!queue.empty()) {
                    Classifier* front = queue.front();
                    queue.pop_front();
                    for (auto& pair : front->m_node->m_references) {
                        if (pair.second) {
                            if (pair.second->m_managerElementMemory->isSubClassOf(ElementType::PORT)) {
                                if (pair.second->m_managerElementMemory->as<Port>().getTypeID() == front->getID()) {
                                    if (pair.second->m_managerElementMemory->as<Port>().isConjugated()) {
                                        pair.second->m_managerElementMemory->as<Port>().getProvided().nonOppositeAdd(supplier.as<Interface>());
                                    } else {
                                        pair.second->m_managerElementMemory->as<Port>().getRequired().nonOppositeAdd(supplier.as<Interface>());
                                    }
                                }
                            } else if (pair.second->m_managerElementMemory->isSubClassOf(ElementType::CLASSIFIER)) {
                                if (pair.second->m_managerElementMemory->as<Classifier>().getGenerals().contains(*front)) {
                                    queue.push_back(&pair.second->m_managerElementMemory->as<Classifier>());
                                }
                            }
                        }
                    }

                }
            }
        }
        
    }
}

void Usage::init() {
    m_client.m_addFunctors.insert(new SetClientFunctor(this));
    m_client.m_removeFunctors.insert(new RemoveClientFunctor(this));
}

Usage::Usage() : Element(ElementType::USAGE) {
    init();
};

Usage::Usage(const Usage& rhs) : Element(rhs, ElementType::USAGE) {
    init();
    Element::copy(rhs);
    Relationship::copy(rhs);
    DirectedRelationship::copy(rhs);
    NamedElement::copy(rhs);
    ParameterableElement::copy(rhs);
    PackageableElement::copy(rhs);
    Dependency::copy(rhs);
    if (!m_copiedElementFlag) {
        delete &rhs;
    }
}

Usage::~Usage() {
    mountAndRelease();
}

bool Usage::isSubClassOf(ElementType eType) const {
    bool ret = Dependency::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::USAGE;
    }

    return ret;
}