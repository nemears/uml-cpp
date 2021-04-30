#ifndef UML_TEST_UTIL_H
#define UML_TEST_UTIL_H

#include "uml/element.h"
#include "uml/sequence.h"
#include "gtest/gtest.h"

using namespace UML;

void AssertElementInSequence(Element& e, Sequence<>& s) {
    ASSERT_TRUE(s.size() == 1);
    ASSERT_TRUE(s.front() == &e);
}

#endif