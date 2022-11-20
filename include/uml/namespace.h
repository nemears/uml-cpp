#ifndef _UML_NAMESPACE_H_
#define _UML_NAMESPACE_H_

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
        protected:
            class AddElementImportPolicy {
                public:
                    static void apply(ElementImport& el, Namespace& me);
            };
            class RemoveElementImportPolicy {
                public:
                    static void apply(ElementImport& el, Namespace& me);
            };
            class AddPackageImportPolicy {
                public:
                    static void apply(PackageImport& el, Namespace& me);
            };
            class RemovePackageImportPolicy {
                public:
                    static void apply(PackageImport& el, Namespace& me);
            };
            CustomSet<NamedElement, Namespace> m_members = CustomSet<NamedElement, Namespace>(this);
            CustomSet<NamedElement, Namespace> m_ownedMembers = CustomSet<NamedElement, Namespace>(this);
            CustomSet<Constraint, Namespace> m_ownedRules = CustomSet<Constraint, Namespace>(this);
            CustomSet<ElementImport, Namespace, AddElementImportPolicy, RemoveElementImportPolicy> m_elementImports = CustomSet<ElementImport, Namespace, AddElementImportPolicy, RemoveElementImportPolicy>(this);
            CustomSet<PackageImport, Namespace, AddPackageImportPolicy, RemovePackageImportPolicy> m_packageImports = CustomSet<PackageImport, Namespace, AddPackageImportPolicy, RemovePackageImportPolicy>(this);
            CustomSet<PackageableElement, Namespace> m_importedMembers = CustomSet<PackageableElement, Namespace>(this);
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            void init();
            Namespace();
        public:
            virtual ~Namespace();
            void setName(const std::string& name) override;
            Set<NamedElement, Namespace>& getMembers();
            Set<NamedElement, Namespace>& getOwnedMembers();
            Set<Constraint, Namespace>& getOwnedRules();
            Set<PackageableElement, Namespace>& getImportedMembers();
            Set<ElementImport, Namespace>& getElementImports();
            Set<PackageImport, Namespace>& getPackageImports();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::NAMESPACE;
            };
    };
}
#endif