#pragma once

#include "namedElement.h"
#include "constraint.h"
#include "elementImport.h"
#include "packageImport.h"

namespace UML{

    class Constraint;

    /**
     * A Namespace is an Element in a model that contains a set of NamedElements that can be identified by name
     **/
    class Namespace : virtual public NamedElement {

        friend class ElementImport;
        friend class Package;
        friend class PackageImport;

        protected:
            class AddElementImportPolicy {
                public:
                    void apply(ElementImport& el, Namespace& me);
            };
            class RemoveElementImportPolicy {
                public:
                    void apply(ElementImport& el, Namespace& me);
            };
            class AddPackageImportPolicy {
                public:
                    void apply(PackageImport& el, Namespace& me);
            };
            class RemovePackageImportPolicy {
                public:
                    void apply(PackageImport& el, Namespace& me);
            };
            CustomReadOnlySet<NamedElement, Namespace> m_members = CustomReadOnlySet<NamedElement, Namespace>(this);
            CustomReadOnlySet<NamedElement, Namespace> m_ownedMembers = CustomReadOnlySet<NamedElement, Namespace>(this);
            CustomSet<Constraint, Namespace> m_ownedRules = CustomSet<Constraint, Namespace>(this);
            CustomSet<ElementImport, Namespace, AddElementImportPolicy, RemoveElementImportPolicy> m_elementImports = CustomSet<ElementImport, Namespace, AddElementImportPolicy, RemoveElementImportPolicy>(this);
            CustomSet<PackageImport, Namespace, AddPackageImportPolicy, RemovePackageImportPolicy> m_packageImports = CustomSet<PackageImport, Namespace, AddPackageImportPolicy, RemovePackageImportPolicy>(this);
            CustomReadOnlySet<PackageableElement, Namespace> m_importedMembers = CustomReadOnlySet<PackageableElement, Namespace>(this);
            void referenceErased(ID id) override;
            Namespace();
        public:
            virtual ~Namespace();
            void setName(const std::string& name) override;
            ReadOnlySet<NamedElement, Namespace>& getMembers();
            ReadOnlySet<NamedElement, Namespace>& getOwnedMembers();
            Set<Constraint, Namespace>& getOwnedRules();
            ReadOnlySet<PackageableElement, Namespace>& getImportedMembers();
            Set<ElementImport, Namespace>& getElementImports();
            Set<PackageImport, Namespace>& getPackageImports();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::NAMESPACE;
            };
    };
}