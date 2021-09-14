#ifndef DEPENDENCY_H
#define DEPENDENCY_H

#include "packageableElement.h"
#include "directedRelationship.h"

namespace UML {
    class Dependency : public DirectedRelationship, public PackageableElement {
        friend class UmlManager;
        protected:
            Sequence<NamedElement> m_client = Sequence<NamedElement>(this);
            Sequence<NamedElement> m_supplier = Sequence<NamedElement>(this);
            class AddClientFunctor : public TemplateAbstractSequenceFunctor<NamedElement,Dependency> {
                public:
                    AddClientFunctor(Dependency* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(NamedElement& el) const override;
            };
            class RemoveClientFunctor : public TemplateAbstractSequenceFunctor<NamedElement,Dependency> {
                public:
                    RemoveClientFunctor(Dependency* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(NamedElement& el) const override;
            };
            class AddSupplierFunctor : public TemplateAbstractSequenceFunctor<NamedElement,Dependency> {
                public:
                    AddSupplierFunctor(Dependency* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(NamedElement& el) const override;
            };
            class RemoveSupplierFunctor : public TemplateAbstractSequenceFunctor<NamedElement,Dependency> {
                public:
                    RemoveSupplierFunctor(Dependency* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(NamedElement& el) const override;
            };
            void setManager(UmlManager* manager) override;
            void reindexID(ID oldID, ID newID) override;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
        public:
            Dependency();
            Dependency(const Dependency& dependency);
            virtual ~Dependency();
            Sequence<NamedElement>& getClient();
            Sequence<NamedElement>& getSupplier();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DEPENDENCY;
            };
    };
}

#endif