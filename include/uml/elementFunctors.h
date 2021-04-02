#ifndef ELEMENT_FUNCTORS_H
#define ELEMENT_FUNCTORS_H

#include "sequence.h"
#include "relationship.h"

namespace UML {
    class SetOwnerFunctor : public AbstractSequenceFunctor {
        public:
            SetOwnerFunctor(Element* me) : AbstractSequenceFunctor(me) {};
            void operator()(Element& el) const override;
    };

    class RemoveOwnerFunctor : public AbstractSequenceFunctor {
        public:
            RemoveOwnerFunctor(Element* me) : AbstractSequenceFunctor(me) {};
            void operator()(Element& el) const override;
    };

    class RemoveRelationshipFunctor : public AbstractSequenceFunctor {
        public:
            RemoveRelationshipFunctor(Element* me) : AbstractSequenceFunctor(me) {};
            void operator()(Element& el) const override;
    };

    class ReadOnlyOwnedElementsFunctor : public AbstractSequenceFunctor {
        public:
            ReadOnlyOwnedElementsFunctor(Element* me) : AbstractSequenceFunctor(me) {};
            void operator()(Element& el) const override;
    };
}

#endif