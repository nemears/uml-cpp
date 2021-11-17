#include "uml/parameterableElement.h"
// #include "uml/templateParameter.h"

using namespace UML;

// void ParameterableElement::RemoveOwningTemplateParameterProcedure::operator()(TemplateParameter* el) const {
//     if (el->getOwnedParameteredElementID() == m_me->getID()) {
//         el->setOwnedParameteredElement(0);
//     }
//     if (m_me->getOwnerID() == el->getID()) {
//         m_me->setOwner(0);
//     }
// }

// void ParameterableElement::AddOwningTemplateParameterProcedure::operator()(TemplateParameter* el) const {
//     if (el->getOwnedParameteredElementID() != m_me->getID()) {
//         el->setOwnedParameteredElement(m_me);
//     }
//     if (m_me->getOwnerID() != el->getID()) {
//         m_me->setOwner(el);
//     }
// }

// void ParameterableElement::AddOwningTemplateParameterProcedure::operator()(ID id) const {
//     if (m_me->getOwnerID() != id) {
//         m_me->setOwnerByID(id);
//     }
// }

// void ParameterableElement::RemoveTemplateParameterProcedure::operator()(TemplateParameter* el) const {
//     if (el->getParameteredElementID() == m_me->getID()) {
//         el->setParameteredElement(0);
//     }
//     if (el->getDefaultID() == m_me->getID()) {
//         el->setDefault(0);
//     }
// }

// void ParameterableElement::AddTemplateParameterProcedure::operator()(TemplateParameter* el) const {
//     if (el->getParameteredElementID() != m_me->getID() && el->getDefaultID() != m_me->getID()) {
//         el->setParameteredElement(m_me);
//     }
// }

void ParameterableElement::referencingReleased(ID id) {
    // m_owningTemplateParameter.release(id);
    // m_templateParameter.release(id);
}

void ParameterableElement::referenceReindexed(ID oldID, ID newID) {
    // m_owningTemplateParameter.reindex(oldID, newID);
    // m_templateParameter.reindex(oldID, newID);
}

void ParameterableElement::restoreReferences() {
    // m_owningTemplateParameter.restoreReference();
    // m_templateParameter.restoreReference();
}

void ParameterableElement::referenceErased(ID id) {
    // m_owningTemplateParameter.elementErased(id);
    // m_templateParameter.elementErased(id);
}

ParameterableElement::ParameterableElement() : Element(ElementType::PARAMETERABLE_ELEMENT) {
    // m_owningTemplateParameter.m_signature = &ParameterableElement::m_owningTemplateParameter;
    // m_owningTemplateParameter.m_addProcedures.push_back(new AddOwningTemplateParameterProcedure(this));
    // m_owningTemplateParameter.m_removeProcedures.push_back(new RemoveOwningTemplateParameterProcedure(this));
    // m_templateParameter.m_signature = &ParameterableElement::m_templateParameter;
    // m_templateParameter.m_removeProcedures.push_back(new RemoveTemplateParameterProcedure(this));
    // m_templateParameter.m_addProcedures.push_back(new AddTemplateParameterProcedure(this));
}

ParameterableElement::ParameterableElement(const ParameterableElement& el) : Element(el, ElementType::PARAMETERABLE_ELEMENT) {
    // m_owningTemplateParameter = el.m_owningTemplateParameter;
    // m_owningTemplateParameter.m_me = this;
    // m_owningTemplateParameter.m_addProcedures.clear();
    // m_owningTemplateParameter.m_removeProcedures.clear();
    // m_owningTemplateParameter.m_addProcedures.push_back(new AddOwningTemplateParameterProcedure(this));
    // m_owningTemplateParameter.m_removeProcedures.push_back(new RemoveOwningTemplateParameterProcedure(this));
    // m_templateParameter = el.m_templateParameter;
    // m_templateParameter.m_me = this;
    // m_templateParameter.m_addProcedures.clear();
    // m_templateParameter.m_removeProcedures.clear();
    // m_templateParameter.m_removeProcedures.push_back(new RemoveTemplateParameterProcedure(this));
    // m_templateParameter.m_addProcedures.push_back(new AddTemplateParameterProcedure(this));
}

ParameterableElement::~ParameterableElement() {

}

// TemplateParameter* ParameterableElement::getOwningTemplateParameter() {
//     // return m_owningTemplateParameter.get();
// }

// TemplateParameter& ParameterableElement::getOwningTemplateParameterRef() {
//     // return m_owningTemplateParameter.getRef();
// }

// ID ParameterableElement::getOwningTemplateParameterID() const {
//     return m_owningTemplateParameter.id();
// }

// bool ParameterableElement::hasOwningTemplateParameter() const {
//     return m_owningTemplateParameter.has();
// }

// void ParameterableElement::setOwningTemplateParameter(TemplateParameter* parameter) {
//     m_owningTemplateParameter.set(parameter);
// }

// void ParameterableElement::setOwningTemplateParameter(TemplateParameter& parameter) {
//     m_owningTemplateParameter.set(parameter);
// }

// TemplateParameter* ParameterableElement::getTemplateParameter() {
//     return m_templateParameter.get();
// }

// TemplateParameter& ParameterableElement::getTemplateParameterRef() {
//     return m_templateParameter.getRef();
// }

// ID ParameterableElement::getTemplateParameterID() const {
//     return m_templateParameter.id();
// }

// bool ParameterableElement::hasTemplateParameter() const {
//     return m_templateParameter.has();
// }

// void ParameterableElement::setTemplateParameter(TemplateParameter* parameter) {
//     m_templateParameter.set(parameter);
// }

// void ParameterableElement::setTemplateParameter(TemplateParameter& parameter) {
//     m_templateParameter.set(parameter);
// }

bool ParameterableElement::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PARAMETERABLE_ELEMENT;
    }

    return ret;
}