#ifndef DEPENDENCY_H
#define DEPENDENCY_H

#include "packageableElement.h"
#include "directedRelationship.h"

namespace UML {
    class Dependency : public DirectedRelationship, public PackageableElement {
        friend class UmlManager;
        protected:
            Sequence<NamedElement> m_client;
            Sequence<NamedElement> m_supplier;
            class AddClientFunctor : public AbstractSequenceFunctor {
                public:
                    AddClientFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveClientFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveClientFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class AddSupplierFunctor : public AbstractSequenceFunctor {
                public:
                    AddSupplierFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveSupplierFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveSupplierFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            void setManager(UmlManager* manager) override;
            void reindexID(ID oldID, ID newID) override;
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