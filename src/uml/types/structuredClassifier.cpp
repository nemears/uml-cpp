#include "uml/types/structuredClassifier.h"
#include "uml/uml-stable.h"

using namespace UML;

void StructuredClassifier::PartPolicy::elementAdded(Property& el, StructuredClassifier& me) {
    if (el.isComposite() && !me.m_parts.contains(el)) {
        me.m_parts.innerAdd(&el);
    }
}

void StructuredClassifier::PartPolicy::elementRemoved(Property& el, StructuredClassifier& me) {
    if (el.isComposite() && me.m_parts.contains(el)) {
        me.m_parts.innerRemove(&el);
    }
}

void StructuredClassifier::restoreReferences() {
    Classifier::restoreReferences();
    std::list<PropertyPtr> props;
    for (auto& prop : m_ownedAttributes) {
        if (prop.isComposite() && !m_parts.contains(prop)) {
            props.push_back(&prop);
        }
    }
    for (auto prop : props) {
        m_parts.innerAdd(prop);
    }
}

// void StructuredClassifier::restoreReference(Element* el) {
//     Classifier::restoreReference(el);
//     if (el->isSubClassOf(ElementType::CONNECTOR)) {
//         Connector& connector = el->as<Connector>();
//         if (connector.getNamespace().id() == m_id && connector.getFeaturingClassifier().id() == ID::nullID() && m_ownedConnectors.contains(connector.getID())) {
//             connector.m_featuringClassifier.innerAdd(*this);
//         }
//     }
// }

StructuredClassifier::StructuredClassifier() : Element(ElementType::STRUCTURED_CLASSIFIER) {
    m_roles.subsets(m_members);
    m_ownedAttributes.subsets(m_ownedMembers);
    m_ownedAttributes.subsets(m_attributes);
    m_ownedAttributes.subsets(m_roles);
    m_parts.subsets(m_ownedAttributes);
    m_ownedConnectors.subsets(m_ownedMembers);
    m_ownedConnectors.subsets(m_features);
}

StructuredClassifier::~StructuredClassifier() {
    
}

Set<Property, StructuredClassifier, StructuredClassifier::PartPolicy>& StructuredClassifier::getOwnedAttributes() {
    return m_ownedAttributes;
}

ReadOnlySet<ConnectableElement, StructuredClassifier>& StructuredClassifier::getRoles() {
    return m_roles;
}

ReadOnlySet<Property, StructuredClassifier>& StructuredClassifier::getParts() {
    return m_parts;
}

Set<Connector, StructuredClassifier>& StructuredClassifier::getOwnedConnectors() {
    return m_ownedConnectors;
}

bool StructuredClassifier::is(ElementType eType) const {
    bool ret = Classifier::is(eType);

    if (!ret) {
        ret = eType == ElementType::STRUCTURED_CLASSIFIER;
    }

    return ret;
}
