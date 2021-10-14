#ifndef NAMESPACEH
#define NAMESPACEH

#include <iostream>
#include "namedElement.h"
#include "sequence.h"

namespace UML{
    /**
     * A Namespace is an Element in a model that contains a set of NamedElements that can be identified by name
     **/
    class Namespace : virtual public NamedElement {
        friend class UmlManager;
        protected:
            Sequence<NamedElement> m_members = Sequence<NamedElement>(this);
            Sequence<NamedElement> m_ownedMembers = Sequence<NamedElement>(this);
            class AddMemberFunctor : public TemplateAbstractSequenceFunctor<NamedElement,Namespace> {
                public:
                    AddMemberFunctor(Namespace* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(NamedElement& el) const override;
                    void operator()(ID id) const override;
            };
            class RemoveMemberFunctor : public TemplateAbstractSequenceFunctor<NamedElement,Namespace> {
                public:
                    RemoveMemberFunctor(Namespace* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(NamedElement& el) const override;
            };
            class AddOwnedMemberFunctor : public TemplateAbstractSequenceFunctor<NamedElement,Namespace> {
                public:
                    AddOwnedMemberFunctor(Namespace* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(NamedElement& el) const override;
                    void operator()(ID id) const override;
            };
            class RemoveOwnedMemberFunctor : public TemplateAbstractSequenceFunctor<NamedElement,Namespace> {
                public:
                    RemoveOwnedMemberFunctor(Namespace* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(NamedElement& el) const override;
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Namespace();
        public:
            virtual ~Namespace();
            Namespace(const Namespace& nmspc);
            Sequence<NamedElement>& getMembers();
            Sequence<NamedElement>& getOwnedMembers();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::NAMESPACE;
            };
    };
}
#endif