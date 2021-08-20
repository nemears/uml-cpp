#include "uml/umlManager.h"
#include "uml/sequence.h"

namespace UML {
namespace {

template <class T = Element, class U = Element> void ASSERT_COPY_CORRECTLY(T& og, T& copy, Sequence<U>& (T::*seq)()) {
    ASSERT_EQ((og.*seq)().size(), (copy.*seq)().size());
    ASSERT_EQ((og.*seq)().back().getID(), (copy.*seq)().back().getID());
};

// recursive killer
template <class T = Element, class U = Element> void ASSERT_COPY_CORRECTLY(T& og, T& copy) {
    return;
};

}

template <class T = Element, class U = Element, typename S, typename... Ss>
void ASSERT_COPY_CORRECTLY(T& og, T& copy, S nextSequence, Ss... sequences){
    ASSERT_EQ((og.*nextSequence)().size(), (copy.*nextSequence)().size());
    ASSERT_EQ((og.*nextSequence)().back().getID(), (copy.*nextSequence)().back().getID());
    ASSERT_COPY_CORRECTLY<T, U>(og, copy, sequences...);
};

}