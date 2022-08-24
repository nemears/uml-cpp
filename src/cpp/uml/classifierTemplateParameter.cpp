#include "uml/classifierTemplateParameter.h"
#include "uml/templateableElement.h"
#include "uml/parameterableElement.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/package.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"
#include "uml/setReferenceFunctor.h"

using namespace UML;

Set<Classifier, ClassifierTemplateParameter>& ClassifierTemplateParameter::getParameteredElementSingleton() {
    return m_classifierParameteredElement;
}

void ClassifierTemplateParameter::referenceReindexed(ID oldID, ID newID) {
    TemplateParameter::referenceReindexed(oldID, newID);
    m_constrainingClassifiers.reindex(oldID, newID);
}

void ClassifierTemplateParameter::reindexName(ID id, std::string newName) {
    TemplateParameter::reindexName(id, newName);
    m_constrainingClassifiers.reindexName(id, newName);
}

void ClassifierTemplateParameter::referencingReleased(ID id) {
    TemplateParameter::referencingReleased(id);
    m_constrainingClassifiers.release(id);
}

void ClassifierTemplateParameter::referenceErased(ID id) {
    TemplateParameter::referenceErased(id);
    m_constrainingClassifiers.eraseElement(id);
}

void ClassifierTemplateParameter::init() {
    m_classifierParameteredElement.redefines(m_parameteredElement);
    m_classifierParameteredElement.opposite(&Classifier::getTemplateParameterSingleton);
    m_constrainingClassifiers.m_addFunctors.insert(new SetReferenceFunctor(this));
    m_constrainingClassifiers.m_removeFunctors.insert(new RemoveReferenceFunctor(this));
}

ClassifierTemplateParameter::ClassifierTemplateParameter() : Element(ElementType::CLASSIFIER_TEMPLATE_PARAMETER) {
    init();
}

ClassifierTemplateParameter::~ClassifierTemplateParameter() {

}

ClassifierPtr ClassifierTemplateParameter::getParameteredElement() const {
    return m_classifierParameteredElement.get();
}

void ClassifierTemplateParameter::setParameteredElement(Classifier* parameteredElement) {
    m_classifierParameteredElement.set(parameteredElement);
}

void ClassifierTemplateParameter::setParameteredElement(Classifier& parameteredElement) {
    m_classifierParameteredElement.set(parameteredElement);
}

void ClassifierTemplateParameter::setParameteredElement(ClassifierPtr parameteredElement) {
    m_classifierParameteredElement.set(parameteredElement);
}

void ClassifierTemplateParameter::setParameteredElement(ID id) {
    m_classifierParameteredElement.set(id);
}

Set<Classifier, ClassifierTemplateParameter>& ClassifierTemplateParameter::getConstrainingClassifiers() {
    return m_constrainingClassifiers;
}

bool ClassifierTemplateParameter::isAllowSubstitutable() const {
    return m_allowSubstitutable;
}

void ClassifierTemplateParameter::setAllowSubstitutable(bool allowSubstitutable) {
    m_allowSubstitutable = allowSubstitutable;
}

bool ClassifierTemplateParameter::isSubClassOf(ElementType eType) const {
    bool ret = false;
    if ((ret = TemplateParameter::isSubClassOf(eType))) {
        return ret;
    }

    ret = eType == ElementType::CLASSIFIER_TEMPLATE_PARAMETER;

    return ret;
}