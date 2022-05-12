#include "uml/structuredClassifier.h"
#include "uml/property.h"
#include "uml/type.h"
#include "uml/connectableElement.h"
#include "uml/package.h"
#include "uml/generalization.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"

using namespace UML;

void StructuredClassifier::AddPartFunctor::operator()(Element& el) const {
    if (el.as<Property>().isComposite() && !m_el.as<StructuredClassifier>().m_parts.contains(el.getID())) {
        m_el.as<StructuredClassifier>().m_parts.nonOppositeAdd(el.as<Property>());
    }
}

void StructuredClassifier::restoreReferences() {
    Classifier::restoreReferences();
    for (auto& prop : m_ownedAttributes) {
        if (prop.isComposite() && !m_parts.contains(prop)) {
            m_parts.addReadOnly(prop.getID());
        }
    }
}

void StructuredClassifier::init() {
    m_roles.subsets(m_members);
    m_roles.m_readOnly = true;
    m_ownedAttributes.subsets(m_ownedMembers);
    m_ownedAttributes.subsets(m_attributes);
    m_ownedAttributes.subsets(m_roles);
    m_ownedAttributes.m_addFunctors.insert(new AddPartFunctor(this));
    m_parts.subsets(m_ownedAttributes);
    m_parts.m_readOnly = true;
    m_ownedConnectors.subsets(m_ownedMembers);
    m_ownedConnectors.subsets(m_features);
}

StructuredClassifier::StructuredClassifier() : Element(ElementType::STRUCTURED_CLASSIFIER) {
    init();
}

StructuredClassifier::~StructuredClassifier() {
    
}

Set<Property, StructuredClassifier>& StructuredClassifier::getOwnedAttributes() {
    return m_ownedAttributes;
}

Set<ConnectableElement, StructuredClassifier>& StructuredClassifier::getRoles() {
    return m_roles;
}

Set<Property, StructuredClassifier>& StructuredClassifier::getParts() {
    return m_parts;
}

Set<Connector, StructuredClassifier>& StructuredClassifier::getOwnedConnectors() {
    return m_ownedConnectors;
}

bool StructuredClassifier::isSubClassOf(ElementType eType) const {
    bool ret = Classifier::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::STRUCTURED_CLASSIFIER;
    }

    return ret;
}