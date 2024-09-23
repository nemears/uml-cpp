#pragma once

#include "namedElement.h"
#include "uml/set/indexableSet.h"

namespace UML{

    class Constraint;
    class ElementImport;
    class PackageImport;
    class PackageableElement;
    typedef UmlPtr<NamedElement> NamedElementPtr;

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
            ReadOnlyIndexableSet<NamedElement, Namespace> m_members = ReadOnlyIndexableSet<NamedElement, Namespace>(this);
            ReadOnlyIndexableSet<NamedElement, Namespace> m_ownedMembers = ReadOnlyIndexableSet<NamedElement, Namespace>(this);
            Set<Constraint, Namespace> m_ownedRules = Set<Constraint, Namespace>(this);
            Set<ElementImport, Namespace, ElementImportPolicy> m_elementImports = Set<ElementImport, Namespace, ElementImportPolicy>(this);
            Set<PackageImport, Namespace, PackageImportPolicy> m_packageImports = Set<PackageImport, Namespace, PackageImportPolicy>(this);
            ReadOnlyIndexableSet<PackageableElement, Namespace> m_importedMembers = ReadOnlyIndexableSet<PackageableElement, Namespace>(this);
            Namespace(std::size_t elementType, AbstractManager& manager);
        public:
            void setName(const std::string& name) override;
            ReadOnlyIndexableSet<NamedElement, Namespace>& getMembers();
            ReadOnlyIndexableSet<NamedElement, Namespace>& getOwnedMembers();
            Set<Constraint, Namespace>& getOwnedRules();
            ReadOnlyIndexableSet<PackageableElement, Namespace>& getImportedMembers();
            Set<ElementImport, Namespace, ElementImportPolicy>& getElementImports();
            Set<PackageImport, Namespace, PackageImportPolicy>& getPackageImports();
            typedef TypeInfo<std::tuple<NamedElement>, Namespace> Info;
    };

    template <>
    struct ElementInfo<Namespace> : public DefaultInfo {
        static constexpr std::string_view name {"Namespace"};
        static SetList sets(Namespace& el) {
            return SetList{
                std::make_pair<std::string, AbstractSet*>("members", &el.getMembers()),
                std::make_pair<std::string, AbstractSet*>("ownedMembers", &el.getOwnedMembers()),
                std::make_pair<std::string, AbstractSet*>("ownedRules", &el.getOwnedRules()),
                std::make_pair<std::string, AbstractSet*>("elementImports", &el.getElementImports()),
                std::make_pair<std::string, AbstractSet*>("packageImports", &el.getPackageImports())
            };
        }
    };
}
