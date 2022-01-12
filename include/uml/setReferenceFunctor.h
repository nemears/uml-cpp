#ifndef _UML_SET_REFERENCE_FUNCTOR_H_
#define _UML_SET_REFERENCE_FUNCTOR_H_

#include "set.h"

namespace UML {
    class SetReferenceFunctor : public SetFunctor {
        public:
            SetReferenceFunctor(Element* them) : SetFunctor(them) {};
            void operator()(Element& el) const override;
    };
    class RemoveReferenceFunctor : public SetFunctor {
        public:
            RemoveReferenceFunctor(Element* them) : SetFunctor(them) {};
            void operator()(Element& el) const override;
    };
}

#endif