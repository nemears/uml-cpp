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
        protected:
            Sequence<NamedElement> m_members;
            Sequence<NamedElement> m_ownedMembers;
            class AddMemberFunctor : public AbstractSequenceFunctor {
                public:
                    AddMemberFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveMemberFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveMemberFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class AddOwnedMemberFunctor : public AbstractSequenceFunctor {
                public:
                    AddOwnedMemberFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveOwnedMemberFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveOwnedMemberFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
        public:
            Namespace();
            ~Namespace();
            Namespace(const Namespace& nmspc);
            Sequence<NamedElement>& getMembers();
            Sequence<NamedElement>& getOwnedMembers();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
    };
}
#endif