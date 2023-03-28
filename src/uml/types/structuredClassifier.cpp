#include "uml/types/structuredClassifier.h"
#include "uml/types/property.h"
#include "uml/types/type.h"
#include "uml/types/connectableElement.h"
#include "uml/types/package.h"
#include "uml/types/generalization.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"

using namespace UML;

void StructuredClassifier::AddPartPolicy::apply(Property& el, StructuredClassifier& me) {
    if (el.isComposite() && !me.m_parts.contains(el)) {
        me.m_parts.innerAdd(el);
    }
}

void StructuredClassifier::RemovePartPolicy::apply(Property& el, StructuredClassifier& me) {
    if (el.isComposite() && me.m_parts.contains(el)) {
        me.m_parts.innerRemove(el.getID());
    }
}

void StructuredClassifier::restoreReferences() {
    Classifier::restoreReferences();
    for (auto& prop : m_ownedAttributes) {
        if (prop.isComposite() && !m_parts.contains(prop)) {
            m_parts.innerAdd(prop.getID());
        }
    }
}

void StructuredClassifier::restoreReference(Element* el) {
    Classifier::restoreReference(el);
    if (el->isSubClassOf(ElementType::CONNECTOR)) {
        Connector& connector = el->as<Connector>();
        if (connector.getNamespace().id() == m_id && connector.getFeaturingClassifier().id() == ID::nullID() && m_ownedConnectors.contains(connector.getID())) {
            connector.m_featuringClassifier.innerAdd(*this);
        }
    }
}

StructuredClassifier::StructuredClassifier() : Element(ElementType::STRUCTURED_CLASSIFIER) {
    m_roles.subsets(m_members);
    m_roles.m_readOnly = true;
    m_ownedAttributes.subsets(m_ownedMembers);
    m_ownedAttributes.subsets(m_attributes);
    m_ownedAttributes.subsets(m_roles);
    m_parts.subsets(m_ownedAttributes);
    m_parts.m_readOnly = true;
    m_ownedConnectors.subsets(m_ownedMembers);
    m_ownedConnectors.subsets(m_features);
    m_ownedConnectors.opposite(&Connector::getStructuredClassifierSingleton);
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