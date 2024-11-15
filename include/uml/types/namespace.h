#pragma once

#include "namedElement.h"
#include "uml/managers/templateTypeList.h"
#include "uml/set/indexableSet.h"

namespace UML{

    template <class>
    class Constraint;
    template <class>
    class ElementImport;
    template <class>
    class PackageImport;
    template <class>
    class PackageableElement;
    // typedef UmlPtr<NamedElement> NamedElementPtr;

    /**
     * A Namespace is an Element in a model that contains a set of NamedElements that can be identified by name
     **/
    template <class ManagerPolicy>
    class Namespace : public ManagerPolicy {

        template <class>
        friend class ElementImport;
        template <class>
        friend class Package;
        template <class>
        friend class PackageImport;

        using ManagedElementImport = ElementImport<typename ManagerPolicy::Manager::template GenBaseHierarchy<ElementImport>>;
        using ManagedPackageImport = PackageImport<typename ManagerPolicy::Manager::template GenBaseHierarchy<PackageImport>>;
        using ManagedNamedElement = NamedElement<typename ManagerPolicy::Manager::template GenBaseHierarchy<NamedElement>>;
        using ManagedConstraint = Constraint<typename ManagerPolicy::Manager::template GenBaseHierarchy<Constraint>>;

        protected:

            // struct ElementImportPolicy {
            //     void elementAdded(ManagedElementImport& el, Namespace& me) {
            //         if (el.getImportedElement() && !me.getImportedMembers().contains(el.getImportedElement().id())) {
            //             me.m_importedMembers.innerAdd(el.getImportedElement());
            //         } 
            //     }
            //     void elementRemoved(ManagedElementImport& el, Namespace& me) {
            //         if (el.getImportedElement() && me.getImportedMembers().contains(el.getImportedElement().id())) {
            //             me.m_importedMembers.innerRemove(el.getImportedElement());
            //         }
            //     }
            // };
            // struct PackageImportPolicy {
            //     void elementAdded(ManagedPackageImport& el, Namespace& me) {
            //         if (el.getImportedPackage()) {
            //             for (auto& pckgMember : el.getImportedPackage()->getPackagedElements()) {
            //                 if (!me.getImportedMembers().contains(pckgMember)) {
            //                     me.m_importedMembers.innerAdd(ElementPtr(&pckgMember));
            //                 }
            //             }
            //         } 
            //     }
            //     void elementRemoved(ManagedPackageImport& el, Namespace& me) {
            //         if (el.getImportedPackage()) {
            //             for (auto& pckgMember : el.getImportedPackage()->getPackagedElements()) {
            //                 if (me.getImportedMembers().contains(pckgMember)) {
            //                     me.m_importedMembers.innerRemove(ElementPtr(&pckgMember));
            //                 }
            //             }
            //         } 
            //     }
            // };
            ReadOnlyIndexableSet<NamedElement, Namespace> m_members = ReadOnlyIndexableSet<NamedElement, Namespace>(this);
            ReadOnlyIndexableSet<NamedElement, Namespace> m_ownedMembers = ReadOnlyIndexableSet<NamedElement, Namespace>(this);
            // Set<Constraint, Namespace> m_ownedRules = Set<Constraint, Namespace>(this);
            // Set<ElementImport, Namespace, ElementImportPolicy> m_elementImports = Set<ElementImport, Namespace, ElementImportPolicy>(this);
            // Set<PackageImport, Namespace, PackageImportPolicy> m_packageImports = Set<PackageImport, Namespace, PackageImportPolicy>(this);
            ReadOnlyIndexableSet<PackageableElement, Namespace> m_importedMembers = ReadOnlyIndexableSet<PackageableElement, Namespace>(this);
            Namespace(std::size_t elementType, AbstractManager& manager) : ManagerPolicy(elementType, manager) {
                m_ownedMembers.subsets(ManagerPolicy::m_ownedElements);
                m_ownedMembers.subsets(m_members);
                m_ownedMembers.opposite(&ManagedNamedElement::getNamespaceSingleton);
                // m_ownedRules.subsets(m_ownedMembers);
                // m_ownedRules.opposite(&ManagedConstraint::getContextSingleton);
                m_importedMembers.subsets(m_members);
                // m_elementImports.subsets(ManagerPolicy::m_ownedElements);
                // m_elementImports.opposite(&ManagedElementImport::getImportingNamespaceSingleton);
                // m_packageImports.subsets(ManagerPolicy::m_ownedElements);
                // m_packageImports.opposite(&ManagedPackageImport::getImportingNamespaceSingleton); 
            }
        public:
            void setName(const std::string& name) override {
                ManagedNamedElement::setName(name);
                for (auto member : getOwnedMembers().ptrs()) {
                    if (!member.loaded()) {
                        continue;
                    }
                    member->updateQualifiedName(ManagerPolicy::getQualifiedName());
                } 
            }
            ReadOnlyIndexableSet<NamedElement, Namespace>& getMembers() {
                return m_members;
            }
            ReadOnlyIndexableSet<NamedElement, Namespace>& getOwnedMembers() {
                return m_ownedMembers;
            }
            // Set<Constraint, Namespace>& getOwnedRules() {
            //     return m_ownedRules;
            // }
            ReadOnlyIndexableSet<PackageableElement, Namespace>& getImportedMembers() {
                return m_importedMembers;
            }
            // Set<ElementImport, Namespace, ElementImportPolicy>& getElementImports() {
            //     return m_elementImports;
            // }
            // Set<PackageImport, Namespace, PackageImportPolicy>& getPackageImports() {
            //     return m_packageImports;
            // }
            typedef TypeInfo<TemplateTypeList<NamedElement>, Namespace> Info;
    };

    template <class ManagerPolicy>
    struct ElementInfo<Namespace<ManagerPolicy>> : public DefaultInfo {
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "Namespace"; }
        static SetList sets(Namespace<ManagerPolicy>& el) {
            return SetList{
                std::make_pair<std::string, AbstractSet*>("members", &el.getMembers()),
                std::make_pair<std::string, AbstractSet*>("ownedMembers", &el.getOwnedMembers()),
                // std::make_pair<std::string, AbstractSet*>("ownedRules", &el.getOwnedRules()),
                // std::make_pair<std::string, AbstractSet*>("elementImports", &el.getElementImports()),
                // std::make_pair<std::string, AbstractSet*>("packageImports", &el.getPackageImports())
            };
        }
    };
}
