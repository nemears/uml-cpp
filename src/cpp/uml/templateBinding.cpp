#include "uml/templateBinding.h"
#include "uml/templateParameterSubstitution.h"
#include "uml/templateableElement.h"
#include "uml/templateSignature.h"
#include "uml/namespace.h"
#include "uml/parameterableElement.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

void TemplateBinding::restoreReference(Element* el) {
    DirectedRelationship::restoreReference(el);
    // m_signature.restore(el);
    if (m_signature.get().id() == el->getID()) {
        el->setReference(this);
    }
}

TypedSet<TemplateableElement, TemplateBinding>& TemplateBinding::getBoundElementSingleton() {
    return m_boundElement;
}

TypedSet<TemplateSignature, TemplateBinding>& TemplateBinding::getSignatureSingleton() {
    return m_signature;
}

TemplateBinding::TemplateBinding() : Element(ElementType::TEMPLATE_BINDING) {
    m_boundElement.subsets(m_sources);
    m_boundElement.subsets(*m_owner);
    m_boundElement.opposite(&TemplateableElement::getTemplateBindings);
    m_signature.subsets(m_targets);
    m_parameterSubstitutions.subsets(*m_ownedElements);
    m_parameterSubstitutions.opposite(&TemplateParameterSubstitution::getTemplateBindingSingleton);
}

TemplateBinding::~TemplateBinding() {
    
}

TemplateableElementPtr TemplateBinding::getBoundElement() const {
    return m_boundElement.get();
}

void TemplateBinding::setBoundElement(TemplateableElement* el) {
    m_boundElement.set(el);
}

void TemplateBinding::setBoundElement(TemplateableElement& el) {
    m_boundElement.set(el);
}

void TemplateBinding::setBoundElement(ID id) {
    m_boundElement.set(id);
}

TemplateSignaturePtr TemplateBinding::getSignature() const {
    return m_signature.get();
}

void TemplateBinding::setSignature(TemplateSignature* signature) {
    m_signature.set(signature);
}

void TemplateBinding::setSignature(TemplateSignature& signature) {
    m_signature.set(signature);
}

void TemplateBinding::setSignature(ID id) {
    m_signature.set(id);
}

Set<TemplateParameterSubstitution, TemplateBinding>& TemplateBinding::getParameterSubstitutions() {
    return m_parameterSubstitutions;
}

bool TemplateBinding::isSubClassOf(ElementType eType) const {
    bool ret = DirectedRelationship::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TEMPLATE_BINDING;
    }

    return ret;
}