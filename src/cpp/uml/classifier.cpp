#include "uml/classifier.h"
#include "uml/property.h"
#include "uml/generalization.h"

using namespace std;
using namespace UML;

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

Classifier::Classifier(const Classifier& clazz) : Namespace(clazz), PackageableElement(clazz), NamedElement(clazz), Element(clazz) {
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
    Namespace::reindexID(oldID, newID);
}

// void Classifier::reindexName(string oldName, string newName) {
//     Namespace::reindexName(oldName, newName);
// }

void Classifier::AddAttributeFunctor::operator()(Element& el) const {
    if (dynamic_cast<Property&>(el).getClassifier() != m_el) {
        dynamic_cast<Property&>(el).setClassifier(dynamic_cast<Classifier*>(m_el));
    }

    if (dynamic_cast<Property&>(el).getFeaturingClassifier() != m_el) {
        dynamic_cast<Property&>(el).setFeaturingClassifier(dynamic_cast<Classifier*>(m_el));
    }
}

void Classifier::RemoveAttributeFunctor::operator()(Element& el) const {
    if (dynamic_cast<Property&>(el).getClassifier() == m_el) {
        dynamic_cast<Property&>(el).setClassifier(0);
    }

    if (dynamic_cast<Property&>(el).getFeaturingClassifier() == m_el) {
        if (dynamic_cast<Property&>(el).getFeaturingClassifier()->getFeatures().count(el.getID())) {
            dynamic_cast<Property&>(el).getFeaturingClassifier()->getFeatures().remove(dynamic_cast<Property&>(el));
        }
    }
}

void Classifier::CheckGeneralizationFunctor::operator()(Element& el) const {
    if (dynamic_cast<Generalization&>(el).getGeneral()) {
        if (dynamic_cast<Generalization&>(el).getGeneral() == m_el) {
            throw InvalidGeneralizationException(el.getID().string());
        }
    }
}

void Classifier::AddGeneralizationFunctor::operator()(Element& el) const {
    if (!m_el->getDirectedRelationships().count(el.getID())) {
        m_el->getDirectedRelationships().add(dynamic_cast<DirectedRelationship&>(el));
    }

    if (dynamic_cast<Generalization&>(el).getSpecific() != m_el) {
        dynamic_cast<Generalization&>(el).setSpecific(dynamic_cast<Classifier*>(m_el));
    }

    if (dynamic_cast<Generalization&>(el).getSpecific()) {
        if (dynamic_cast<Generalization&>(el).getSpecific() == m_el) {
            if (dynamic_cast<Generalization&>(el).getGeneral()) {
                if (!dynamic_cast<Classifier*>(m_el)->getGenerals().count(dynamic_cast<Generalization&>(el).getGeneral()->getID())) {
                    dynamic_cast<Classifier*>(m_el)->getGenerals().add(*dynamic_cast<Generalization&>(el).getGeneral());
                }
            }
        }
    }
    
    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalAdd(el);
    }
}

void Classifier::RemoveGeneralizationFunctor::operator()(Element& el) const {
    if (m_el->getDirectedRelationships().count(el.getID())) {
        m_el->getDirectedRelationships().remove(dynamic_cast<DirectedRelationship&>(el));
    }

    if (dynamic_cast<Generalization&>(el).getSpecific() == m_el) {
        dynamic_cast<Generalization&>(el).setSpecific(0);
    }

    if (dynamic_cast<Generalization&>(el).getSpecific()) {
        if (dynamic_cast<Generalization&>(el).getSpecific() == m_el) {
            if (dynamic_cast<Generalization&>(el).getGeneral()) {
                if (dynamic_cast<Classifier*>(m_el)->getGenerals().count(dynamic_cast<Generalization&>(el).getGeneral()->getID())) {
                    dynamic_cast<Classifier*>(m_el)->getGenerals().remove(*dynamic_cast<Generalization&>(el).getGeneral());
                }
            }
        }
    }
    
    if (m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalRemove(el);
    }
}

void Classifier::AddGeneralFunctor::operator()(Element& el) const {
    bool foundGeneralization = false;
    for (auto& general : dynamic_cast<Classifier*>(m_el)->getGeneralizations()) {
        if (general.getTargets().count(el.getID())) {
            foundGeneralization = true;
        }
    }

    if (!foundGeneralization) {
        if (!dynamic_cast<Classifier*>(m_el)->m_manager) {
            throw NoManagerException();
        }
        Generalization& newGen = dynamic_cast<Classifier*>(m_el)->m_manager->create<Generalization>();
        newGen.setGeneral(&dynamic_cast<Classifier&>(el));
        newGen.setSpecific(dynamic_cast<Classifier*>(m_el));
    }
}

void Classifier::RemoveGeneralFunctor::operator()(Element& el) const {
    for (auto& general : dynamic_cast<Classifier*>(m_el)->getGeneralizations()) {
        if (general.getGeneral() == &dynamic_cast<Classifier&>(el)) {
            general.setGeneral(0);
        }
    }
}

void Classifier::AddFeatureFunctor::operator()(Element& el) const {
    if (dynamic_cast<Feature&>(el).getFeaturingClassifier() != m_el) {
        dynamic_cast<Feature&>(el).setFeaturingClassifier(dynamic_cast<Classifier*>(m_el));
    }

    if (!dynamic_cast<Feature&>(el).getMemberNamespace().count(m_el->getID())) {
        dynamic_cast<Feature&>(el).getMemberNamespace().add(*dynamic_cast<Classifier*>(m_el));
    }
}

void Classifier::RemoveFeatureFunctor::operator()(Element& el) const {
    if (dynamic_cast<Feature&>(el).getFeaturingClassifier() == m_el) {
        dynamic_cast<Feature&>(el).setFeaturingClassifier(0);
    }

    if (dynamic_cast<Feature&>(el).getMemberNamespace().count(m_el->getID())) {
        if (dynamic_cast<Classifier*>(m_el)->getMembers().count(el.getID())) {
            dynamic_cast<Classifier*>(m_el)->getMembers().remove(dynamic_cast<Feature&>(el));
        }
    }
}

void Classifier::AddInheritedMemberFunctor::operator()(Element& el) const {
    if (!dynamic_cast<Classifier*>(m_el)->getMembers().count(el.getID())) {
        dynamic_cast<Classifier*>(m_el)->getMembers().add(dynamic_cast<NamedElement&>(el));
    }
}

void Classifier::RemoveInheritedMemberFunctor::operator()(Element& el) const {
    if (dynamic_cast<Classifier*>(m_el)->getMembers().count(el.getID())) {
        dynamic_cast<Classifier*>(m_el)->getMembers().remove(dynamic_cast<NamedElement&>(el));
    }
}

void Classifier::ClassifierAddMemberFunctor::operator()(Element& el) const {
    if (dynamic_cast<NamedElement&>(el).getVisibility() != VisibilityKind::PRIVATE) {
        for (auto& relationship : dynamic_cast<Classifier*>(m_el)->getRelationships()) {
            if (relationship.isSubClassOf(ElementType::GENERALIZATION)) {
                if (dynamic_cast<Generalization&>(relationship).getGeneral() == m_el) {
                    if (dynamic_cast<Generalization&>(relationship).getSpecific()) {
                        if (!dynamic_cast<Generalization&>(relationship).getSpecific()->getInheritedMembers().count(el.getID())) {
                            dynamic_cast<Generalization&>(relationship).getSpecific()->getInheritedMembers().add(dynamic_cast<NamedElement&>(el));
                        }
                    }
                }
            }
        }
    }
}

void Classifier::ClassifierRemoveMemberFunctor::operator()(Element& el) const {
    if (dynamic_cast<NamedElement&>(el).getVisibility() != VisibilityKind::PRIVATE) {
        for (auto& relationship : dynamic_cast<Classifier*>(m_el)->getRelationships()) {
            if (relationship.isSubClassOf(ElementType::GENERALIZATION)) {
                if (dynamic_cast<Generalization&>(relationship).getGeneral() == m_el) {
                    if (dynamic_cast<Generalization&>(relationship).getSpecific()) {
                        if (dynamic_cast<Generalization&>(relationship).getSpecific()->getInheritedMembers().count(el.getID())) {
                            dynamic_cast<Generalization&>(relationship).getSpecific()->getInheritedMembers().remove(dynamic_cast<NamedElement&>(el));
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

bool Classifier::isSubClassOf(ElementType eType) {
    bool ret = Namespace::isSubClassOf(eType);

    if (!ret) {
        ret = Type::isSubClassOf(eType);
    }

    if (!ret) {
        ret = RedefinableElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::CLASSIFIER;
    }

    return ret;
}