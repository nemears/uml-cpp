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

    class AddRelationshipFunctor : public AbstractSequenceFunctor {
        public:
            AddRelationshipFunctor(Element* me) : AbstractSequenceFunctor(me) {};
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

    class AddDirectedRelationshipFunctor : public AbstractSequenceFunctor {
        public:
            AddDirectedRelationshipFunctor(Element* me) : AbstractSequenceFunctor(me) {};
            void operator()(Element& el) const override;
    };

    class RemoveDirectedRelationshipFunctor : public AbstractSequenceFunctor {
        public:
            RemoveDirectedRelationshipFunctor(Element* me) : AbstractSequenceFunctor(me) {};
            void operator()(Element& el) const override;
    };

    class AddOwnedCommentFunctor : public AbstractSequenceFunctor {
        public:
            AddOwnedCommentFunctor(Element* me) : AbstractSequenceFunctor(me) {};
            void operator()(Element& el) const override;
    };

    class RemoveOwnedCommentFunctor : public AbstractSequenceFunctor {
        public:
            RemoveOwnedCommentFunctor(Element* me) : AbstractSequenceFunctor(me) {};
            void operator()(Element& el) const override;
    };
}

#endif