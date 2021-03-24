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
    dr.getTargets().add(b);
    ASSERT_TRUE(dr.getSources().size() == 1);
    ASSERT_TRUE(dr.getSources().front() == &a);
    ASSERT_TRUE(dr.getTargets().size() == 1);
    ASSERT_TRUE(dr.getTargets().front() == &b);
}

TEST_F(DirectedRelationshipTest, addTargetFunctorTest) {
    DirectedRelationship dr;
    Element a;
    Element b;
    dr.getTargets().add(a);
    dr.getSources().add(b);
    ASSERT_TRUE(dr.getRelatedElements().size() == 2);
    ASSERT_TRUE(dr.getRelatedElements().front() == &a);
    ASSERT_TRUE(dr.getRelatedElements().back() == &b);
    ASSERT_TRUE(a.getRelationships().size() == 1);
    ASSERT_TRUE(a.getRelationships().front() == &dr);
    ASSERT_TRUE(b.getRelationships().size() == 1);
    ASSERT_TRUE(b.getRelationships().front() == &dr);
}

TEST_F(DirectedRelationshipTest, duplicateRelationshipExceptionTest) {
    DirectedRelationship dr;
    Element a;
    Element b;
    dr.getTargets().add(a);
    dr.getSources().add(b);
    ASSERT_THROW(dr.getRelatedElements().add(a), DuplicateRelationshipException);
}