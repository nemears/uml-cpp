#include "uml/types/stereotype.h"
#include "uml/types/profile.h"
#include "uml/types/behavior.h"
#include "uml/types/association.h"
#include "uml/types/dataType.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

void Stereotype::SetOwningPackagePolicy::apply(Package& el, Stereotype& me) {
    if (el.isSubClassOf(ElementType::PROFILE) && me.m_profile.get().id() != el.getID()) {
        me.m_profile.addReadOnly(el.as<Profile>());
    }
}

void Stereotype::RemoveOwningPackagePolicy::apply(Package& el, Stereotype& me) {
    if (el.isSubClassOf(ElementType::PROFILE) && me.m_profile.get().id() == el.getID()) {
        me.m_profile.removeReadOnly(el.getID());
    }
}

TypedSet<Profile, Stereotype>& Stereotype::getProfileSingleton() {
    return m_profile;
}

void Stereotype::restoreReference(Element* el) {
    Class::restoreReference(el);
    if (el->isSubClassOf(ElementType::PROFILE) && m_owningPackage.get().id() == el->getID()&& m_profile.get().id() != el->getID()) {
        m_profile.addReadOnly(el->as<Profile>());
    }
}

Stereotype::Stereotype() : Element(ElementType::STEREOTYPE) {
    m_stereotypeOwningPackage.redefines(m_owningPackage);
    m_profile.subsets(m_stereotypeOwningPackage);
    m_profile.opposite(&Profile::getOwnedStereotypes);
    m_profile.m_readOnly = true;
}

Stereotype::~Stereotype() {
    
}

ProfilePtr Stereotype::getProfile() const {
    return m_profile.get();
}

bool Stereotype::isSubClassOf(ElementType eType) const {
    bool ret = Class::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::STEREOTYPE;
    }

    return ret;
}