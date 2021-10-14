#include "uml/classifier.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/instanceSpecification.h"
#include "uml/class.h"

using namespace std;
using namespace UML;

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

void Classifier::AddAttributeFunctor::operator()(ID id) const {
    if (!m_el->getFeatures().count(id)) {
        m_el->getFeatures().addByID(id);
    }
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

void Classifier::AddGeneralizationFunctor::operator()(ID id) const {
    if (!m_el->getDirectedRelationships().count(id)) {
        m_el->getDirectedRelationships().addByID(id);
    }
    if (!m_el->getOwnedElements().count(id)) {
        m_el->getOwnedElements().addByID(id);
    }
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
    el.setReference(m_el);
    updateCopiedSequenceAddedTo(el, &Classifier::getGenerals);
}

void Classifier::RemoveGeneralFunctor::operator()(Classifier& el) const {
    for (auto& general : m_el->getGeneralizations()) {
        if (general.getGeneral() == &el) {
            general.setGeneral(0);
        }
    }
    el.removeReference(m_el->getID());
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

void Classifier::AddFeatureFunctor::operator()(ID id) const {
    if (!m_el->getMembers().count(id)) {
        m_el->getMembers().addByID(id);
    }
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

void Classifier::AddInheritedMemberFunctor::operator()(ID id) const {
    if (!m_el->getMembers().count(id)) {
        m_el->getMembers().addByID(id);
    }
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

void Classifier::RemoveNestingClassProcedure::operator()(Class* el) const {
    if (el->getNestedClassifiers().count(m_me->getID())) {
        el->getNestedClassifiers().remove(*m_me);
    }
}

void Classifier::AddNestingClassProcedure::operator()(Class* el) const {
    if (!el->getNestedClassifiers().count(m_me->getID())) {
        el->getNestedClassifiers().add(*m_me);
    }
    if (m_me->getNamespaceID() != el->getID()) {
        m_me->setNamespace(el);
    }
}

void Classifier::AddNestingClassProcedure::operator()(ID id) const {
    if (m_me->getNamespaceID() != id) {
        m_me->m_namespace.setByID(id);
    }
}

void Classifier::setManager(UmlManager* manager) {
    Namespace::setManager(manager);
    RedefinableElement::setManager(manager); // not polymorphic
    TemplateableElement::setManager(manager);
    m_attributes.m_manager = manager;
    m_inheritedMembers.m_manager = manager;
    m_generalizations.m_manager = manager;
    m_generals.m_manager = manager;
    m_features.m_manager = manager;
}

void Classifier::referencingReleased(ID id) {
    Namespace::referencingReleased(id);
    PackageableElement::referencingReleased(id);
    TemplateableElement::referencingReleased(id);
    m_attributes.elementReleased(id, &Classifier::getAttributes);
    m_features.elementReleased(id, &Classifier::getFeatures);
    m_generalizations.elementReleased(id, &Classifier::getGeneralizations);
    m_generals.elementReleased(id, &Classifier::getGenerals);
    m_inheritedMembers.elementReleased(id, &Classifier::getInheritedMembers);
    m_nestingClass.release(id);
}

void Classifier::referenceReindexed(ID oldID, ID newID) {
    Namespace::referenceReindexed(oldID, newID);
    PackageableElement::referenceReindexed(oldID, newID);
    TemplateableElement::referenceReindexed(oldID, newID);
    m_attributes.reindex(oldID, newID, &Classifier::getAttributes);
    m_generalizations.reindex(oldID, newID, &Classifier::getGeneralizations);
    m_features.reindex(oldID, newID, &Classifier::getFeatures);
    m_generals.reindex(oldID, newID, &Classifier::getGenerals);
    m_inheritedMembers.reindex(oldID, newID, &Classifier::getInheritedMembers);
    m_nestingClass.reindex(oldID, newID);
}

void Classifier::restoreReferences() {
    Namespace::restoreReferences();
    PackageableElement::restoreReferences();
    m_attributes.restoreReferences();
    m_generalizations.restoreReferences();
    // need to load generalizations
    for (auto& generalization : m_generalizations) {
        if (generalization.hasGeneral()) {
            if (!m_generals.count(generalization.getGeneralID())) {
                m_generals.addByID(generalization.getGeneralID());
            }
        }
    }
    m_generals.restoreReferences();
    // need to load generals
    for (auto& general : m_generals) {
        for (auto& member : general.getMembers()) {
            if (member.getVisibility() != VisibilityKind::PRIVATE && !m_inheritedMembers.count(member.getID())) {
                m_inheritedMembers.addByID(member.getID());
            }
        }
    }
    m_features.restoreReferences();
    m_inheritedMembers.restoreReferences();
    m_nestingClass.restoreReference();
}

void Classifier::referenceErased(ID id) {
    Namespace::referenceErased(id);
    Type::referenceErased(id);
    RedefinableElement::referenceErased(id);
    TemplateableElement::referenceErased(id);
    m_attributes.elementErased(id);
    m_features.elementErased(id);
    m_generalizations.elementErased(id);
    m_generals.elementErased(id);
    m_inheritedMembers.elementErased(id);
    m_nestingClass.elementErased(id);
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
    m_nestingClass.m_signature = &Classifier::m_nestingClass;
    m_nestingClass.m_removeProcedures.push_back(new RemoveNestingClassProcedure(this));
    m_nestingClass.m_addProcedures.push_back(new AddNestingClassProcedure(this));
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
    m_nestingClass = clazz.m_nestingClass;
    m_nestingClass.m_me = this;
    m_nestingClass.m_addProcedures.clear();
    m_nestingClass.m_removeProcedures.clear();
    m_nestingClass.m_removeProcedures.push_back(new RemoveNestingClassProcedure(this));
    m_nestingClass.m_addProcedures.push_back(new AddNestingClassProcedure(this));
}

void Classifier::reindexName(string oldName, string newName) {
    Namespace::reindexName(oldName, newName);
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

Class* Classifier::getNestingClass() {
    return m_nestingClass.get();
}

Class& Classifier::getNestingClassRef() {
    return m_nestingClass.getRef();
}

ID Classifier::getNestingClassID() const {
    return m_nestingClass.id();
}

bool Classifier::hasNestingClass() const {
    return m_nestingClass.has();
}

void Classifier::setNestingClass(Class* clazz) {
    m_nestingClass.set(clazz);
}

void Classifier::setNestingClass(Class& clazz) {
    m_nestingClass.set(clazz);
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