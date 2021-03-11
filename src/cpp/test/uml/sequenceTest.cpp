#include "gtest/gtest.h"
#include "uml/sequence.h"

using namespace UML;

class SequenceTest : public ::testing::Test {
   
};

TEST_F(SequenceTest, addGetAndRemoveElementTest) {
    Sequence seq;
    ASSERT_TRUE(seq.size() == 0);
    Element e;
    seq.add(e);
    ASSERT_TRUE(seq.get(e.uuid) == &e);
    ASSERT_TRUE(seq.size() == 1);
    seq.remove(e);
    ASSERT_TRUE(seq.size() == 0);
}

TEST_F(SequenceTest, sequenceIteratorTest) {
    Sequence seq;
    Element e;
    Element f;
    seq.add(e);
    seq.add(f);
    ASSERT_TRUE(seq.size() == 2);
    SequenceIterator it = seq.iterator();
    ASSERT_TRUE(it.hasNext());
    Element* first = it.getNext();
    ASSERT_TRUE(first == &e);
    ASSERT_TRUE(it.hasNext());
    Element* second = it.getNext();
    ASSERT_TRUE(second == &f);
    ASSERT_FALSE(it.hasNext());
}