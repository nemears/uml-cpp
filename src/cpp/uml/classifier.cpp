#include "uml/classifier.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/instanceSpecification.h"

using namespace std;
using namespace UML;

void Classifier::referenceReindexed(ID oldID, ID newID) {
    Namespace::referenceReindexed(oldID, newID);
    PackageableElement::referenceReindexed(oldID, newID);
    if (m_attributes.count(oldID)) {
        m_attributes.reindex(oldID, newID, &Classifier::getAttributes);
    }
    if (m_generalizations.count(oldID)) {
        m_generalizations.reindex(oldID, newID, &Classifier::getGeneralizations);
    }
    if (m_features.count(oldID)) {
        m_features.reindex(oldID, newID, &Classifier::getFeatures);
    }
    if (m_generals.count(oldID)) {
        m_generals.reindex(oldID, newID, &Classifier::getGenerals);
    }
    if (m_inheritedMembers.count(oldID)) {
        m_inheritedMembers.reindex(oldID, newID, &Classifier::getInheritedMembers);
    }
}

Classifier::Classifier() {
    m_attributes.addProcedures.push_back(new AddAttributeFunctor(this));
    m_attributes.removeProcedures.push_back(new RemoveAttributeFunctor(this));
    m_generalizations.addProcedures.push_back(new AddGeneralizationFunctor(this));
    m_generalizations.removeProcedures.push_back(new RemoveGeneralizationFunctor(this));
    m_generalizations.addChecks.push_back(new CheckGeneralizationFunctor(this));
    m_generals.addProcedures.push_back(new AddGeneralFunctor(this));
    m_generals.removeProcedures.push_back(new RemoveGeneralFunctor(this));
    m_features.addProcedures.push_back(new AddFeatureFunctor(this));
    m_features.removeProcedures.push_back(new RemoveFeatureFunctor(this));
    m_inheritedMembers.addProcedures.push_back(new AddInheritedMemberFunctor(this));
    m_inheritedMembers.removeProcedures.push_back(new RemoveInheritedMemberFunctor(this));
    m_members.addProcedures.push_back(new ClassifierAddMemberFunctor(this));
    m_members.removeProcedures.push_back(new ClassifierRemoveMemberFunctor(this));
}

Classifier::~Classifier() {

}

Classifier::Classifier(const Classifier& clazz) : 
Namespace(clazz), 
PackageableElement(clazz), 
NamedElement(clazz), 
TemplateableElement(clazz), 
Element(clazz) {
    m_attributes = clazz.m_attributes;
    m_attributes.addProcedures.clear();
    m_attributes.addProcedures.push_back(new AddAttributeFunctor(this));
    m_attributes.removeProcedures.clear();
    m_attributes.removeProcedures.push_back(new RemoveAttributeFunctor(this));
    m_generalizations = clazz.m_generalizations;
    m_generalizations.addProcedures.clear();
    m_generalizations.addProcedures.push_back(new AddGeneralizationFunctor(this));
    m_generalizations.addChecks.clear();
    m_generalizations.addChecks.push_back(new CheckGeneralizationFunctor(this));
    m_generalizations.removeProcedures.clear();
    m_generalizations.removeProcedures.push_back(new RemoveGeneralizationFunctor(this));
    m_generals = clazz.m_generals;
    m_generals.addProcedures.clear();
    m_generals.addProcedures.push_back(new AddGeneralFunctor(this));
    m_generals.removeProcedures.clear();
    m_generals.removeProcedures.push_back(new RemoveGeneralFunctor(this));
    m_features = clazz.m_features;
    m_features.addProcedures.clear();
    m_features.addProcedures.push_back(new AddFeatureFunctor(this));
    m_features.removeProcedures.clear();
    m_features.removeProcedures.push_back(new RemoveFeatureFunctor(this));
    m_inheritedMembers = clazz.m_inheritedMembers;
    m_inheritedMembers.addProcedures.clear();
    m_inheritedMembers.addProcedures.push_back(new AddInheritedMemberFunctor(this));
    m_inheritedMembers.removeProcedures.clear();
    m_inheritedMembers.removeProcedures.push_back(new RemoveInheritedMemberFunctor(this));
    m_members.addProcedures.push_back(new ClassifierAddMemberFunctor(this));
    m_members.removeProcedures.push_back(new ClassifierRemoveMemberFunctor(this));
}

void Classifier::reindexID(ID oldID, ID newID) {
    /**Namespace::reindexID(oldID, newID);
    Type::reindexID(oldID, newID);
    if (m_node) {
        for (auto& ref : m_node->m_references) {    
            // TODO change to polymorphic?
            if (ref.second->m_managerElementMemory->isSubClassOf(ElementType::INSTANCE_SPECIFICATION)) {
                if (ref.second->m_managerElementMemory->as<InstanceSpecification>().hasClassifier()) {
                    if (ref.second->m_managerElementMemory->as<InstanceSpecification>().getClassifierRef() == *this) {
                        ref.second->m_managerElementMemory->as<InstanceSpecification>().m_classifier.m_id = newID;
                        ref.second->m_managerElementMemory->as<InstanceSpecification>().m_classifier.m_ptr = 0;
                        ref.second->m_managerElementMemory->as<InstanceSpecification>().m_classifier.updateCopies();
                    }
                }
            }
        }
    }**/
}

void Classifier::reindexName(string oldName, string newName) {
    Namespace::reindexName(oldName, newName);
}

void Classifier::AddAttributeFunctor::operator()(Property& el) const {
    if (el.getClassifier() != m_el) {
        el.setClassifier(m_el);
    }
    
    if (!m_el->getFeatures().count(el.getID())) {
        m_el->getFeatures().add(el);
    }

    if (el.getFeaturingClassifier() != m_el) {
        el.setFeaturingClassifier(m_el);
    }

    updateCopiedSequenceAddedTo(el, &Classifier::getAttributes);
}

void Classifier::RemoveAttributeFunctor::operator()(Property& el) const {
    if (el.getClassifier() == m_el) {
        el.setClassifier(0);
    }

    if (m_el->getFeatures().count(el.getID())) {
        m_el->getFeatures().remove(el);
    }

    if (el.getFeaturingClassifier() == m_el) {
        if (el.getFeaturingClassifier()->getFeatures().count(el.getID())) {
            el.getFeaturingClassifier()->getFeatures().remove(el);
        }
    }

    updateCopiedSequenceRemovedFrom(el, &Classifier::getAttributes);
}

void Classifier::CheckGeneralizationFunctor::operator()(Generalization& el) const {
    if (el.hasGeneral()) {
        if (el.getGeneralRef() == *m_el) {
            throw InvalidGeneralizationException(el.getID().string());
        }
    }
}

void Classifier::AddGeneralizationFunctor::operator()(Generalization& el) const {
    if (!m_el->getDirectedRelationships().count(el.getID())) {
        m_el->getDirectedRelationships().add(el);
    }

    if (el.getSpecific() != m_el) {
        el.setSpecific(m_el);
    }

    if (el.getSpecific()) {
        if (el.getSpecific() == m_el) {
            if (el.getGeneral()) {
                if (!m_el->getGenerals().count(el.getGeneral()->getID())) {
                    m_el->getGenerals().add(*el.getGeneral());
                }
            }
        }
    }
    
    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalAdd(el);
    }
    updateCopiedSequenceAddedTo(el, &Classifier::getGeneralizations);
}

void Classifier::RemoveGeneralizationFunctor::operator()(Generalization& el) const {
    if (m_el->getDirectedRelationships().count(el.getID())) {
        m_el->getDirectedRelationships().remove(el);
    }

    if (el.getSpecific() == m_el) {
        el.setSpecific(0);
    }

    if (el.getSpecific()) {
        if (el.getSpecific() == m_el) {
            if (el.getGeneral()) {
                if (m_el->getGenerals().count(el.getGeneral()->getID())) {
                    m_el->getGenerals().remove(*el.getGeneral());
                }
            }
        }
    }
    
    if (m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalRemove(el);
    }
    updateCopiedSequenceRemovedFrom(el, &Classifier::getGeneralizations);
}

void Classifier::AddGeneralFunctor::operator()(Classifier& el) const {
    bool foundGeneralization = false;
    for (auto& general : m_el->getGeneralizations()) {
        if (general.getTargets().count(el.getID())) {
            foundGeneralization = true;
        }
    }

    if (!foundGeneralization) {
        if (!m_el->m_manager) {
            throw NoManagerException();
        }
        Generalization& newGen = dynamic_cast<Classifier*>(m_el)->m_manager->create<Generalization>();
        newGen.setGeneral(&dynamic_cast<Classifier&>(el));
        newGen.setSpecific(dynamic_cast<Classifier*>(m_el));
    }
    updateCopiedSequenceAddedTo(el, &Classifier::getGenerals);
}

void Classifier::RemoveGeneralFunctor::operator()(Classifier& el) const {
    for (auto& general : m_el->getGeneralizations()) {
        if (general.getGeneral() == &el) {
            general.setGeneral(0);
        }
    }
    updateCopiedSequenceRemovedFrom(el, &Classifier::getGenerals);
}

void Classifier::AddFeatureFunctor::operator()(Feature& el) const {
    if (el.getFeaturingClassifier() != m_el) {
        el.setFeaturingClassifier(m_el);
    }

    if (!el.getMemberNamespace().count(m_el->getID())) {
        el.getMemberNamespace().add(*m_el);
    }

    if (!m_el->getMembers().count(el.getID())) {
        m_el->getMembers().add(el);
    }

    updateCopiedSequenceAddedTo(el, &Classifier::getFeatures);
}

void Classifier::RemoveFeatureFunctor::operator()(Feature& el) const {
    if (el.getFeaturingClassifier() == m_el) {
        el.setFeaturingClassifier(0);
    }

    if (el.getMemberNamespace().count(m_el->getID())) {
        if (m_el->getMembers().count(el.getID())) {
            m_el->getMembers().remove(el);
        }
    }

    if (m_el->getMembers().count(el.getID())) {
        m_el->getMembers().add(el);
    }

    updateCopiedSequenceRemovedFrom(el, &Classifier::getFeatures);
}

void Classifier::AddInheritedMemberFunctor::operator()(NamedElement& el) const {
    if (!m_el->getMembers().count(el.getID())) {
        m_el->getMembers().add(el);
    }
    updateCopiedSequenceAddedTo(el, &Classifier::getInheritedMembers);
}

void Classifier::RemoveInheritedMemberFunctor::operator()(NamedElement& el) const {
    if (m_el->getMembers().count(el.getID())) {
        m_el->getMembers().remove(el);
    }
    updateCopiedSequenceRemovedFrom(el, &Classifier::getInheritedMembers);
}

void Classifier::ClassifierAddMemberFunctor::operator()(NamedElement& el) const {
    if (el.getVisibility() != VisibilityKind::PRIVATE) {
        for (auto& relationship : m_el->getRelationships()) {
            if (relationship.isSubClassOf(ElementType::GENERALIZATION)) {
                if (dynamic_cast<Generalization&>(relationship).getGeneral() == m_el) {
                    if (dynamic_cast<Generalization&>(relationship).getSpecific()) {
                        if (!dynamic_cast<Generalization&>(relationship).getSpecific()->getInheritedMembers().count(el.getID())) {
                            dynamic_cast<Generalization&>(relationship).getSpecific()->getInheritedMembers().add(el);
                        }
                    }
                }
            }
        }
    }
}

void Classifier::ClassifierRemoveMemberFunctor::operator()(NamedElement& el) const {
    if (el.getVisibility() != VisibilityKind::PRIVATE) {
        for (auto& relationship : m_el->getRelationships()) {
            if (relationship.isSubClassOf(ElementType::GENERALIZATION)) {
                if (dynamic_cast<Generalization&>(relationship).getGeneral() == m_el) {
                    if (dynamic_cast<Generalization&>(relationship).getSpecific()) {
                        if (dynamic_cast<Generalization&>(relationship).getSpecific()->getInheritedMembers().count(el.getID())) {
                            dynamic_cast<Generalization&>(relationship).getSpecific()->getInheritedMembers().remove(el);
                        }
                    }
                }
            }
        }
    }
}

void Classifier::setManager(UmlManager* manager) {
    Namespace::setManager(manager);
    RedefinableElement::setManager(manager); // not polymorphic
    m_attributes.m_manager = manager;
    m_inheritedMembers.m_manager = manager;
    m_generalizations.m_manager = manager;
    m_generals.m_manager = manager;
    m_features.m_manager = manager;
}

string Classifier::getName() {
    return Namespace::getName();
}

void Classifier::setName(const string& name) {
    Namespace::setName(name);
}

Sequence<Feature>& Classifier::getFeatures() {
    return m_features;
}

Sequence<Property>& Classifier::getAttributes() {
    return m_attributes;
}

Sequence<Generalization>& Classifier::getGeneralizations() {
    return m_generalizations;
}

Sequence<Classifier>& Classifier::getGenerals() {
    return m_generals;
}

Sequence<NamedElement>& Classifier::getInheritedMembers() {
    return m_inheritedMembers;
}

ElementType Classifier::getElementType() const {
    return ElementType::CLASSIFIER;
}

bool Classifier::isSubClassOf(ElementType eType) const {
    bool ret = Namespace::isSubClassOf(eType);

    if (!ret) {
        ret = Type::isSubClassOf(eType);
    }

    if (!ret) {
        ret = RedefinableElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = TemplateableElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::CLASSIFIER;
    }

    return ret;
}

void Classifier::restoreReleased(ID id, Element* released) {
    Namespace::restoreReleased(id, released);
    if (m_attributes.count(id)) {
        released->as<Property>().setClassifier(this);
    }
    if (m_features.count(id)) {
        released->as<Feature>().setFeaturingClassifier(this);
    }
    if (m_generalizations.count(id)) {
        released->as<Generalization>().setSpecific(this);
    }
    if (m_generals.count(id)) {
        /** TODO: do we do anything here?**/
    }
}

void Classifier::referencingReleased(ID id) {
    Namespace::referencingReleased(id);
    PackageableElement::referencingReleased(id);
    m_attributes.elementReleased(id, &Classifier::getAttributes);
    m_features.elementReleased(id, &Classifier::getFeatures);
    m_generalizations.elementReleased(id, &Classifier::getGeneralizations);
    m_generals.elementReleased(id, &Classifier::getGenerals);
    m_inheritedMembers.elementReleased(id, &Classifier::getInheritedMembers);
}