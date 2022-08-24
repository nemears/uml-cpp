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
            Set<NamedElement, Namespace> m_members = Set<NamedElement, Namespace>(this);
            Set<NamedElement, Namespace> m_ownedMembers = Set<NamedElement, Namespace>(this);
            Set<Constraint, Namespace> m_ownedRules = Set<Constraint, Namespace>(this);
            Set<ElementImport, Namespace> m_elementImports = Set<ElementImport, Namespace>(this);
            Set<PackageImport, Namespace> m_packageImports = Set<PackageImport, Namespace>(this);
            Set<PackageableElement, Namespace> m_importedMembers = Set<PackageableElement, Namespace>(this);
            class AddElementImportFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    AddElementImportFunctor(Element* el) : SetFunctor(el) {};
            };
            class RemoveElementImportFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    RemoveElementImportFunctor(Element* el) : SetFunctor(el) {};
            };
            class AddPackageImportFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    AddPackageImportFunctor(Element* el) : SetFunctor(el) {};
            };
            class RemovePackageImportFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    RemovePackageImportFunctor(Element* el) : SetFunctor(el) {};
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(ID id, std::string newName) override;
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