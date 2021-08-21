#include "uml/umlManager.h"
#include "uml/sequence.h"

namespace UML {
namespace {

template <class T = Element, class U = Element> void ASSERT_COPY_SEQUENCE_CORRECTLY(T& og, T& copy) {};
template <class T = Element, class U = Element> void ASSERT_COPY_SINGLETON_CORRECTLY(T& og, T& copy) {};

}

template <class T = Element, class U = Element, typename S, typename... Ss>
void ASSERT_COPY_SEQUENCE_CORRECTLY(T& og, T& copy, S seq, Ss... sequences){
    ASSERT_EQ((og.*seq)().size(), (copy.*seq)().size());
    for (size_t i = 0; i < (og.*seq)().size(); i++) {
        ASSERT_EQ((og.*seq)().get(i).getID(), (copy.*seq)().get(i).getID());
    }
    ASSERT_COPY_SEQUENCE_CORRECTLY<T, U>(og, copy, sequences...);
};

template <class T = Element, class U = Element, typename F, typename... Fs>
void ASSERT_COPY_SINGLETON_CORRECTLY(T& og, T& copy, F func, Fs... funcs) {
    if ((og.*func)()) {
        ASSERT_TRUE((copy.*func)());
        ASSERT_EQ((og.*func)()->getID(), (copy.*func)()->getID());
    } else {
        ASSERT_TRUE((copy.*func)() == 0);
    }
    ASSERT_COPY_SINGLETON_CORRECTLY(og, copy, funcs...);
};

}