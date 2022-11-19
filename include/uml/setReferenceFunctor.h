#ifndef _UML_SET_REFERENCE_FUNCTOR_H_
#define _UML_SET_REFERENCE_FUNCTOR_H_

namespace UML {
    template <class T, class U>
    class SetReferencePolicy {
        public:
            void apply(T& el, U& me);
    };
    template<class T, class U>
    class RemoveReferencePolicy {
        public:
            void apply(T& el, U& me);
    };
}

#endif