#include "uml/templateSignature.h"
#include "uml/templateableElement.h"
#include "uml/umlManager.h"
#include "uml/templateParameter.h"
#include "uml/uml-stable.h"

using namespace UML;

void TemplateSignature::referencingReleased(ID id) {
    Element::referencingReleased(id);
    m_parameters.release(id);
}

void TemplateSignature::reindexName(std::string oldName, std::string newName) {
    Element::reindexName(oldName, newName);
    m_parameters.reindexName(oldName, newName);
}

void TemplateSignature::referenceReindexed(ID oldID, ID newID) {
    Element::referenceReindexed(oldID, newID);
    m_parameters.reindex(oldID, newID);
}

void TemplateSignature::referenceErased(ID id) {
    Element::referenceErased(id);
    m_parameters.eraseElement(id);
}

Set<TemplateableElement, TemplateSignature>& TemplateSignature::getTemplateSingleton() {
    return m_template;
}

Set<TemplateParameter, TemplateSignature>& TemplateSignature::getParametersSet() {
    return m_parameters;
}

Set<TemplateParameter, TemplateSignature>& TemplateSignature::getOwnedParametersSet() {
    return m_ownedParameters;
}

void TemplateSignature::init() {
    m_template.subsets(*m_owner);
    m_template.opposite(&TemplateableElement::getOwnedTemplateSignatureSingleton);
    m_template.m_signature = &TemplateSignature::getTemplateSingleton;
    m_parameters.m_signature = &TemplateSignature::getParametersSet;
    m_ownedParameters.subsets(m_parameters);
    m_ownedParameters.subsets(*m_ownedElements);
    m_ownedParameters.opposite(&TemplateParameter::getSignatureSingleton);
    m_ownedParameters.m_signature = &TemplateSignature::getOwnedParametersSet;
}

void TemplateSignature::copy(const TemplateSignature& rhs) {
    m_template = rhs.m_template;
}

TemplateSignature::TemplateSignature() : Element(ElementType::TEMPLATE_SIGNATURE) {
    init();
}

TemplateSignature::TemplateSignature(const TemplateSignature& rhs) : Element(rhs, ElementType::TEMPLATE_SIGNATURE) {
    init();
    Element::copy(rhs);
    copy(rhs);
    if (!m_copiedElementFlag) {
        delete &rhs;
    }
}

TemplateSignature::~TemplateSignature() {
    mountAndRelease();
}

TemplateableElement* TemplateSignature::getTemplate() {
    return m_template.get();
}

TemplateableElement& TemplateSignature::getTemplateRef() {
    return m_template.getRef();
}

ID TemplateSignature::getTemplateID() const {
    return m_template.id();
}

bool TemplateSignature::hasTemplate() const {
    return m_template.has();
}

void TemplateSignature::setTemplate(TemplateableElement* temp) {
    m_template.set(temp);
}

void TemplateSignature::setTemplate(TemplateableElement& temp) {
    m_template.set(temp);
}

void TemplateSignature::setTemplate(ID id) {
    m_template.set(id);
}

OrderedSet<TemplateParameter, TemplateSignature>& TemplateSignature::getOwnedParameters() {
    return m_ownedParameters;
}

OrderedSet<TemplateParameter, TemplateSignature>& TemplateSignature::getParameters() {
    return m_parameters;
}

bool TemplateSignature::isSubClassOf(ElementType eType) const {
    bool ret = Element::isSubClassOf(eType);

    if (!ret) {
        return eType == ElementType::TEMPLATE_SIGNATURE;
    }

    return ret;
}