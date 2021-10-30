#include "uml/stereotype.h"
#include "uml/profile.h"

using namespace UML;

void Stereotype::RemoveProfileProcedure::operator()(Profile* el) const {
    el->removeReference(m_me->getID());
}

void Stereotype::AddProfileProcedure::operator()(Profile* el) const {
    el->setReference(m_me);
}

void Stereotype::referencingReleased(ID id) {
    Class::referencingReleased(id);
    m_profile.release(id);
}

void Stereotype::referenceReindexed(ID oldID, ID newID) {
    Class::referenceReindexed(oldID, newID);
    m_profile.reindex(oldID, newID);
}

void Stereotype::restoreReferences() {
    Class::restoreReferences();
    m_profile.restoreReference();
}

void Stereotype::referenceErased(ID id) {
    Class::referenceErased(id);
    m_profile.elementErased(id);
}

Stereotype::Stereotype() : Element(ElementType::STEREOTYPE) {
    m_profile.m_signature = &Stereotype::m_profile;
    m_profile.m_addProcedures.push_back(new AddProfileProcedure(this));
    m_profile.m_removeProcedures.push_back(new RemoveProfileProcedure(this));
}

Stereotype::Stereotype(const Stereotype& stereotype) :
Class(stereotype),
Classifier(stereotype),
ParameterableElement(stereotype),
PackageableElement(stereotype),
NamedElement(stereotype),
Element(stereotype, ElementType::STEREOTYPE) {
    m_profile.m_signature = &Stereotype::m_profile;
    m_profile.m_me = this;
    m_profile.m_addProcedures.clear();
    m_profile.m_removeProcedures.clear();
    m_profile.m_addProcedures.push_back(new AddProfileProcedure(this));
    m_profile.m_removeProcedures.push_back(new RemoveProfileProcedure(this));
}

Stereotype::~Stereotype() {

}

Profile* Stereotype::getProfile() {
    return m_profile.get();
}

Profile& Stereotype::getProfileRef() {
    return m_profile.getRef();
}

ID Stereotype::getProfileID() const {
    return m_profile.id();
}

bool Stereotype::hasProfile() const {
    return m_profile.has();
}

void Stereotype::setProfile(Profile* profile) {
    m_profile.set(profile);
}

void Stereotype::setProfile(Profile& profile) {
    m_profile.set(profile);
}

bool Stereotype::isSubClassOf(ElementType eType) const {
    bool ret = Class::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::STEREOTYPE;
    }

    return ret;
}