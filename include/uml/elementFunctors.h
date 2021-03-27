#ifndef ELEMENT_FUNCTORS_H
#define ELEMENT_FUNCTORS_H

#include "sequence.h"

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
}

#endif