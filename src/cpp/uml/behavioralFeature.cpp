#include "uml/behavioralFeature.h"
#include "uml/operation.h"
#include "uml/behavior.h"
#include "uml/parameter.h"

using namespace UML;

void BehavioralFeature::AddMethodFunctor::operator()(Behavior& el) const {
    if (!el.getSpecification()) {
        el.setSpecification(m_el);
    }
    updateCopiedSequenceAddedTo(el, &BehavioralFeature::getMethods);
}

void BehavioralFeature::RemoveMethodFunctor::operator()(Behavior& el) const {
    if (el.getSpecification() == m_el) {
        el.setSpecification(0);
    }
    updateCopiedSequenceRemovedFrom(el, &BehavioralFeature::getMethods);
}

void BehavioralFeature::AddParameterFunctor::operator()(Parameter& el) const {
    if (m_el->isSubClassOf(ElementType::OPERATION)) {
        if (el.getOperation() != m_el) {
            el.setOperation(&m_el->as<Operation>());
        }
    }

    if (!m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().add(el);
    }

    // I think we shouldn't enforce parameters being the same OMG UML 2.5.1 spec pg 289 bottom of page

    // TODO behavior must have same parameters as it's specification, but they are not the same parameters
    // TODO think about whether to be different just in memory or have different ids but same everything else
    // for (auto const& method : dynamic_cast<BehavioralFeature*>(m_el)->getMethods()) {
    //     if (!method->getParameters().count(el.getID())) {
    //         method->getParameters().add(dynamic_cast<Parameter&>(el));
    //     }
    // }
    updateCopiedSequenceAddedTo(el, &BehavioralFeature::getOwnedParameters);
}

void BehavioralFeature::CheckParameterFunctor::operator()(Parameter& el) const {
    ParameterDirectionKind direction = el.getDirection();
    if (direction == ParameterDirectionKind::RETURN || direction == ParameterDirectionKind::OUT || direction == ParameterDirectionKind::INOUT) {
        if (m_el->m_returnSpecified) {
            if (m_el->isSubClassOf(ElementType::OPERATION)) {
                if (el.getOperation() == m_el) {
                    el.setOperation(0);
                }
            }
            throw ReturnParameterException(m_el->getElementTypeString() + " " + m_el->getID().string());
        } else {
            m_el->m_returnSpecified = true;
        }
    }
}

void BehavioralFeature::RemoveParameterFunctor::operator()(Parameter& el) const {
    if (m_el->isSubClassOf(ElementType::OPERATION)) {
        if (el.getOperation() == m_el) {
            el.setOperation(0);
        }
    }

    if (m_el->getMembers().count(el.getID())) {
        m_el->getMembers().remove(el);
    }
    updateCopiedSequenceRemovedFrom(el, &BehavioralFeature::getOwnedParameters);
}

void BehavioralFeature::setManager(UmlManager* manager) {
    Namespace::setManager(manager);
    RedefinableElement::setManager(manager);
    m_methods.m_manager = manager;
    m_ownedParameters.m_manager = manager;
}

void BehavioralFeature::referenceReindexed(ID oldID, ID newID) {
    Feature::referenceReindexed(oldID, newID);
    Namespace::referenceReindexed(oldID, newID);
    if (m_methods.count(oldID)) {
        m_methods.reindex(oldID, newID, &BehavioralFeature::getMethods);
    }
    if (m_ownedParameters.count(oldID)) {
        m_ownedParameters.reindex(oldID, newID, &BehavioralFeature::getOwnedParameters);
    }
}

BehavioralFeature::BehavioralFeature() {
    m_methods.addProcedures.push_back(new AddMethodFunctor(this));
    m_methods.removeProcedures.push_back(new RemoveMethodFunctor(this));
    m_ownedParameters.addProcedures.push_back(new AddParameterFunctor(this));
    m_ownedParameters.addChecks.push_back(new CheckParameterFunctor(this));
    m_ownedParameters.removeProcedures.push_back(new RemoveParameterFunctor(this));
}

BehavioralFeature::BehavioralFeature(const BehavioralFeature& el) : NamedElement(el), Element(el) {
    m_methods = el.m_methods;
    m_methods.addProcedures.clear();
    m_methods.removeProcedures.clear();
    m_methods.addProcedures.push_back(new AddMethodFunctor(this));
    m_methods.removeProcedures.push_back(new RemoveMethodFunctor(this));
    m_ownedParameters = el.m_ownedParameters;
    m_ownedParameters.addProcedures.clear();
    m_ownedParameters.addChecks.clear();
    m_ownedParameters.removeProcedures.clear();
    m_ownedParameters.addProcedures.push_back(new AddParameterFunctor(this));
    m_ownedParameters.addChecks.push_back(new CheckParameterFunctor(this));
    m_ownedParameters.removeProcedures.push_back(new RemoveParameterFunctor(this));
}

BehavioralFeature::~BehavioralFeature() {

}

Sequence<Behavior>& BehavioralFeature::getMethods() {
    return m_methods;
}

Sequence<Parameter>& BehavioralFeature::getOwnedParameters() {
    return m_ownedParameters;
}

bool BehavioralFeature::isAbstract() {
    return m_methods.size() > 0;
}

ElementType BehavioralFeature::getElementType() const {
    return ElementType::BEHAVIORAL_FEATURE;
}

bool BehavioralFeature::isSubClassOf(ElementType eType) const {
    bool ret = Feature::isSubClassOf(eType);

    if (!ret) {
        ret = Namespace::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::BEHAVIORAL_FEATURE;
    }

    return ret;
}

void BehavioralFeature::restoreReleased(ID id, Element* released) {
    Namespace::restoreReleased(id, released);
    Feature::restoreReleased(id, released);
}

void BehavioralFeature::referencingReleased(ID id) {
    Namespace::referencingReleased(id);
    Feature::referencingReleased(id);
    if (m_methods.count(id)) {
        m_methods.elementReleased(id, &BehavioralFeature::getMethods);
    }
    if (m_ownedParameters.count(id)) {
        m_ownedParameters.elementReleased(id, &BehavioralFeature::getOwnedParameters);
    }
}