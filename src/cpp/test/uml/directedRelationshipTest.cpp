#include "gtest/gtest.h"
#include "uml/directedRelationship.h"

using namespace UML;

class DirectedRelationshipTest : public ::testing::Test {
   
};

TEST_F(DirectedRelationshipTest, isSubClassOfRelationshipTest) {
    DirectedRelationship dr;
    ASSERT_TRUE(dr.isSubClassOf(ElementType::DIRECTED_RELATIONSHIP));
    ASSERT_FALSE(dr.isSubClassOf(ElementType::NAMED_ELEMENT));
}

TEST_F(DirectedRelationshipTest, sourceAndTargetTest) {
    DirectedRelationship dr;
    Element a;
    Element b;
    dr.getSources().add(a);
    dr.getRelatedElements().add(a);
    dr.getTargets().add(b);
    dr.getRelatedElements().add(b);
    a.getRelationships().add(dr);
    b.getRelationships().add(dr);
    ASSERT_TRUE(dr.getSources().size() == 1);
    ASSERT_TRUE(dr.getSources().front() == &a);
    ASSERT_TRUE(dr.getTargets().size() == 1);
    ASSERT_TRUE(dr.getTargets().front() == &b);
}