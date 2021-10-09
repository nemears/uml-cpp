#include "uml/behavior.h"
#include "uml/behavioralFeature.h"
#include "uml/operation.h"
#include "uml/parameter.h"

using namespace UML;

void Behavior::RemoveSpecificationProcedure::operator()(BehavioralFeature* el) const {
    if (el->getMethods().count(m_me->getID())) {
        el->getMethods().remove(*m_me);
    }
}

void Behavior::AddSpecificationProcedure::operator()(BehavioralFeature* el) const {
    if (!el->getMethods().count(m_me->getID())) {
        el->getMethods().add(*m_me);
    }
}

void Behavior::RemoveBehavioredClassifierProcedure::operator()(BehavioredClassifier* el) const {
    if (el->getOwnedBehaviors().count(m_me->getID())) {
        el->getOwnedBehaviors().remove(*m_me);
    }
}

void Behavior::AddBehavioredClassifierProcedure::operator()(BehavioredClassifier* el) const {
    if (!el->getOwnedBehaviors().count(m_me->getID())) {
        el->getOwnedBehaviors().add(*m_me);
    }
}

void Behavior::AddParameterFunctor::operator()(Parameter& el) const {
    if (!m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().internalAdd(el);
    }

    if (m_el->getSpecification()) {
        el.setOperation(dynamic_cast<Operation*>(m_el->getSpecification()));
    }
    updateCopiedSequenceAddedTo(el, &Behavior::getParameters);
}

void Behavior::RemoveParameterFunctor::operator()(Parameter& el) const {
    if (m_el->getSpecification()) {
        if (el.getOperation() == m_el->getSpecification()) {

            bool usedElsewhere = false;

            // note slow performance for removing
            for (auto& method : dynamic_cast<Operation*>(el.getOperation())->getMethods()) {
                if (method.getID() != m_el->getID()) {
                    if (method.getParameters().count(el.getID())) {
                        usedElsewhere = true;
                    }
                }
            }

            if (!usedElsewhere) {
                el.setOperation(0);
            }
        }
    }

    if (m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().internalRemove(el);
    }
    updateCopiedSequenceRemovedFrom(el, &Behavior::getParameters);
}

void Behavior::setManager(UmlManager* manager) {
    Class::setManager(manager);
    m_parameters.m_manager = manager;
}

void Behavior::referencingReleased(ID id) {
    Class::referencingReleased(id);
    if (m_parameters.count(id)) {
        m_parameters.elementReleased(id, &Behavior::getParameters);
    }
    if (m_specification.id() == id) {
        m_specification.release();
    }
    if (m_behavioredClassifier.id() == id) {
        m_behavioredClassifier.release();
    }
}

void Behavior::referenceReindexed(ID oldID, ID newID) {
    Class::referenceReindexed(oldID, newID);
    if (m_parameters.count(oldID)) {
        m_parameters.reindex(oldID, newID, &Behavior::getParameters);
    }
    if (m_specification.id() == oldID) {
        m_specification.reindex(oldID, newID);
    }
    if (m_behavioredClassifier.id() == oldID) {
        m_behavioredClassifier.reindex(oldID, newID);
    }
}

Behavior::Behavior() {
    m_parameters.addProcedures.push_back(new AddParameterFunctor(this));
    m_parameters.removeProcedures.push_back(new RemoveParameterFunctor(this));
    m_specification.m_signature = &Behavior::m_specification;
    m_specification.m_removeProcedures.push_back(new RemoveSpecificationProcedure(this));
    m_specification.m_addProcedures.push_back(new AddSpecificationProcedure(this));
    m_behavioredClassifier.m_signature = &Behavior::m_behavioredClassifier;
    m_behavioredClassifier.m_removeProcedures.push_back(new RemoveBehavioredClassifierProcedure(this));
    m_behavioredClassifier.m_addProcedures.push_back(new AddBehavioredClassifierProcedure(this));
}

Behavior::Behavior(const Behavior& rhs) : Class(rhs), Classifier(rhs), PackageableElement(rhs), NamedElement(rhs), Element(rhs) {
    m_parameters = rhs.m_parameters;
    m_parameters.m_el = this;
    m_parameters.addProcedures.clear();
    m_parameters.removeProcedures.clear();
    m_parameters.addProcedures.push_back(new AddParameterFunctor(this));
    m_parameters.removeProcedures.push_back(new RemoveParameterFunctor(this));
    m_specification = rhs.m_specification;
    m_specification.m_me = this;
    m_specification.m_removeProcedures.clear();
    m_specification.m_addProcedures.clear();
    m_specification.m_removeProcedures.push_back(new RemoveSpecificationProcedure(this));
    m_specification.m_addProcedures.push_back(new AddSpecificationProcedure(this));
    m_behavioredClassifier = rhs.m_behavioredClassifier;
    m_behavioredClassifier.m_me = this;
    m_behavioredClassifier.m_removeProcedures.clear();
    m_behavioredClassifier.m_addProcedures.clear();
    m_behavioredClassifier.m_removeProcedures.push_back(new RemoveBehavioredClassifierProcedure(this));
    m_behavioredClassifier.m_addProcedures.push_back(new AddBehavioredClassifierProcedure(this));
}

Behavior::~Behavior() {
    
}

Sequence<Parameter>& Behavior::getParameters() {
    return m_parameters;
}

BehavioralFeature* Behavior::getSpecification() {
    return m_specification.get();
}

BehavioralFeature& Behavior::getSpecificationRef() {
    return m_specification.getRef();
}

ID Behavior::getSpecificationID() const {
    return m_specification.id();
}

bool Behavior::hasSpecification() const {
    return m_specification.has();
}

void Behavior::setSpecification(BehavioralFeature* specification) {
    m_specification.set(specification);
}

void Behavior::setSpecification(BehavioralFeature& specification) {
    m_specification.set(specification);
}

BehavioredClassifier* Behavior::getBehavioredClassifier() {
    return m_behavioredClassifier.get();
}

BehavioredClassifier& Behavior::getBehavioredClassifierRef() {
    return m_behavioredClassifier.getRef();
}

ID Behavior::getBehavioredClassifierID() const {
    return m_behavioredClassifier.id();
}

bool Behavior::hasBehavioredClassifier() const {
    return m_behavioredClassifier.has();
}

void Behavior::setBehavioredClassifier(BehavioredClassifier* classifier) {
    m_behavioredClassifier.set(classifier);
}

void Behavior::setBehavioredClassifier(BehavioredClassifier& classifier) {
    m_behavioredClassifier.set(classifier);
}

ElementType Behavior::getElementType() const {
    return ElementType::BEHAVIOR;
}

bool Behavior::isSubClassOf(ElementType eType) const {
    bool ret = Class::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::BEHAVIOR;
    }
    
    return ret;
}