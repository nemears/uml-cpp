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

    class ReadOnlySequenceFunctor : public AbstractSequenceFunctor {
        private:
            std::string m_name;
        public:
            ReadOnlySequenceFunctor(Element* me, std::string sequenceName) : AbstractSequenceFunctor(me) {
                m_name = sequenceName;
            };
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

    class AddAppliedStereotypeFunctor : public AbstractSequenceFunctor {
        public:
            AddAppliedStereotypeFunctor(Element* me) : AbstractSequenceFunctor(me) {};
            void operator()(Element& el) const override;
    };

    class RemoveAppliedStereotypeFunctor : public AbstractSequenceFunctor {
        public:
            RemoveAppliedStereotypeFunctor(Element* me) : AbstractSequenceFunctor(me) {};
            void operator()(Element& el) const override;
    };

    class CheckAppliedStereotypeFunctor : public AbstractSequenceFunctor {
        public:
            CheckAppliedStereotypeFunctor(Element* me) : AbstractSequenceFunctor(me) {};
            void operator()(Element& el) const override;
    };

    class InvalidAppliedStereotypeException : public std::exception {
        public:
            virtual const char* what() const throw() {
                return "tried to apply instance as stereotype that does not have a classifier set to a stereotype!";
            };
    };
}

#endif