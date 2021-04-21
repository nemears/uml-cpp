#ifndef NAMESPACEH
#define NAMESPACEH
#include <iostream>
#include "namedElement.h"
#include "sequence.h"
using namespace std;

namespace UML{
    /**
     * A Namespace is an Element in a model that contains a set of NamedElements that can be identified by name
     **/
    class Namespace : virtual public NamedElement {
        protected:
            Sequence<NamedElement>* m_members;
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
        public:
            Namespace();
            ~Namespace();
            Namespace(const Namespace& nmspc);
            Sequence<NamedElement>& getMembers();
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}
#endif