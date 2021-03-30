#ifndef ELEMENT_FUNCTORS_H
#define ELEMENT_FUNCTORS_H

#include "sequence.h"
#include "relationship.h"

namespace UML {
    class SetOwnerFunctor : public AbstractSequenceFunctor {
        public:
            SetOwnerFunctor(Element* me) : AbstractSequenceFunctor(me) {};
            void operator()(Element& el) const override {
                if (el.getOwner() != m_el) {
                    el.setOwner(m_el);
                }
            };
    };

    class RemoveRelationshipFunctor : public AbstractSequenceFunctor {
        public:
            RemoveRelationshipFunctor(Element* me) : AbstractSequenceFunctor(me) {};
            void operator()(Element& el) const override {
                if (dynamic_cast<Relationship&>(el).getRelatedElements().count(m_el->getID())) {
                    dynamic_cast<Relationship&>(el).getRelatedElements().remove(*m_el);
                }
            };
    };
}

#endif