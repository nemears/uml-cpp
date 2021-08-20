#include "uml/umlManager.h"
#include "uml/sequence.h"

namespace UML {
namespace {

template <class T = Element, class U = Element> void ASSERT_COPY_CORRECTLY(T& og, T& copy, Sequence<U>& (T::*seq)()) {
    ASSERT_EQ((og.*seq)().size(), (copy.*seq)().size());
    for (size_t i = 0; i < (og.*seq)().size(); i++) {
        ASSERT_EQ((og.*seq)().get(i).getID(), (copy.*seq)().get(i).getID());
    }
};

// recursive killer
template <class T = Element, class U = Element> void ASSERT_COPY_CORRECTLY(T& og, T& copy) {
    return;
};

}

template <class T = Element, class U = Element, typename S, typename... Ss>
void ASSERT_COPY_CORRECTLY(T& og, T& copy, S seq, Ss... sequences){
    ASSERT_EQ((og.*seq)().size(), (copy.*seq)().size());
    for (size_t i = 0; i < (og.*seq)().size(); i++) {
        ASSERT_EQ((og.*seq)().get(i).getID(), (copy.*seq)().get(i).getID());
    }
    ASSERT_COPY_CORRECTLY<T, U>(og, copy, sequences...);
};

}