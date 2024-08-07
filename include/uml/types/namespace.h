#pragma once

#include "namedElement.h"
#include "uml/set/set.h"

namespace UML{

    class Constraint;
    class ElementImport;
    class PackageImport;
    class PackageableElement;

    /**
     * A Namespace is an Element in a model that contains a set of NamedElements that can be identified by name
     **/
    class Namespace : virtual public NamedElement {

        friend class ElementImport;
        friend class Package;
        friend class PackageImport;

        protected:
            struct ElementImportPolicy {
                void elementAdded(ElementImport& el, Namespace& me);
                void elementRemoved(ElementImport& el, Namespace& me);
            };
            struct PackageImportPolicy {
                void elementAdded(PackageImport& el, Namespace& me);
                void elementRemoved(PackageImport& el, Namespace& me);
            };
            ReadOnlySet<NamedElement, Namespace> m_members = ReadOnlySet<NamedElement, Namespace>(this);
            ReadOnlySet<NamedElement, Namespace> m_ownedMembers = ReadOnlySet<NamedElement, Namespace>(this);
            Set<Constraint, Namespace> m_ownedRules = Set<Constraint, Namespace>(this);
            Set<ElementImport, Namespace, ElementImportPolicy> m_elementImports = Set<ElementImport, Namespace, ElementImportPolicy>(this);
            Set<PackageImport, Namespace, PackageImportPolicy> m_packageImports = Set<PackageImport, Namespace, PackageImportPolicy>(this);
            ReadOnlySet<PackageableElement, Namespace> m_importedMembers = ReadOnlySet<PackageableElement, Namespace>(this);
            void referenceErased(ID id) override;
            Namespace();
        public:
            virtual ~Namespace();
            void setName(const std::string& name) override;
            ReadOnlySet<NamedElement, Namespace>& getMembers();
            ReadOnlySet<NamedElement, Namespace>& getOwnedMembers();
            Set<Constraint, Namespace>& getOwnedRules();
            ReadOnlySet<PackageableElement, Namespace>& getImportedMembers();
            Set<ElementImport, Namespace, ElementImportPolicy>& getElementImports();
            Set<PackageImport, Namespace, PackageImportPolicy>& getPackageImports();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::NAMESPACE;
            };
    };
}
