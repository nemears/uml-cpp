#ifndef NAMED_ELEMENT_FUNCTORS_H
#define NAMED_ELEMENT_FUNCTORS_H

#include "sequence.h"
#include  "namedElement.h"

namespace UML {
    class AddMemberNamespaceFunctor : public AbstractSequenceFunctor {
        public:
            AddMemberNamespaceFunctor(Element* me) : AbstractSequenceFunctor(me) {};
            void operator()(Element& el) const override;
    };
    class RemoveMemberNamespaceFunctor : public AbstractSequenceFunctor {
        public:
            RemoveMemberNamespaceFunctor(Element* me) : AbstractSequenceFunctor(me) {};
            void operator()(Element& el) const override;
    };
}

#endif