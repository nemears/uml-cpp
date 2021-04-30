#include "gtest/gtest.h"
#include "uml/association.h"
#include "umlTestUtil.h"

using namespace UML;

class AssociationTest : public ::testing::Test {
   
};

TEST_F(AssociationTest, addMemberEndFunctorTest) {
    Property p;
    Association a;
    ASSERT_NO_THROW(a.getMemberEnds().add(p));
    ASSERT_TRUE(a.getMemberEnds().size() == 1);
    ASSERT_TRUE(a.getMemberEnds().front() == &p);
    ASSERT_TRUE(a.getMembers().size() == 1);
    ASSERT_TRUE(a.getMembers().front() == &p);

    ASSERT_TRUE(p.getAssociation() == &a);
    ASSERT_TRUE(p.getMemberNamespace().size() == 1);
    ASSERT_TRUE(p.getMemberNamespace().front() == &a);
}

TEST_F(AssociationTest, setAssociationTest) {
    Property p;
    Association a;
    ASSERT_NO_THROW(p.setAssociation(&a));
    ASSERT_TRUE(a.getMemberEnds().size() == 1);
    ASSERT_TRUE(a.getMemberEnds().front() == &p);
    ASSERT_TRUE(a.getMembers().size() == 1);
    ASSERT_TRUE(a.getMembers().front() == &p);

    ASSERT_TRUE(p.getAssociation() == &a);
    ASSERT_TRUE(p.getMemberNamespace().size() == 1);
    ASSERT_TRUE(p.getMemberNamespace().front() == &a);
}

TEST_F(AssociationTest, removeMemberEndFunctor) {
    Property p;
    Association a;
    ASSERT_NO_THROW(p.setAssociation(&a));
    ASSERT_NO_THROW(a.getMemberEnds().remove(p));
    ASSERT_TRUE(a.getMemberEnds().size() == 0);
    ASSERT_TRUE(a.getMembers().size() == 0);
    ASSERT_TRUE(p.getAssociation() == 0);
    ASSERT_TRUE(p.getMemberNamespace().size() == 0);
}

TEST_F(AssociationTest, overwriteAssociationW_NullTest) {
    Property p;
    Association a;
    ASSERT_NO_THROW(a.getMemberEnds().add(p));
    ASSERT_NO_THROW(p.setAssociation(0));
    ASSERT_TRUE(a.getMemberEnds().size() == 0);
    ASSERT_TRUE(a.getMembers().size() == 0);
    ASSERT_TRUE(p.getAssociation() == 0);
    ASSERT_TRUE(p.getMemberNamespace().size() == 0);
}

TEST_F(AssociationTest, overwriteAssociationW_OtherTest) {
    Property p;
    Association a;
    Association a2;
    ASSERT_NO_THROW(a.getMemberEnds().add(p));
    ASSERT_NO_THROW(p.setAssociation(&a2));
    ASSERT_TRUE(a2.getMemberEnds().size() == 1);
    ASSERT_TRUE(a2.getMemberEnds().front() == &p);
    ASSERT_TRUE(a2.getMembers().size() == 1);
    ASSERT_TRUE(a2.getMembers().front() == &p);

    ASSERT_TRUE(p.getAssociation() == &a2);
    ASSERT_TRUE(p.getMemberNamespace().size() == 1);
    ASSERT_TRUE(p.getMemberNamespace().front() == &a2);

    ASSERT_TRUE(a.getMemberEnds().size() == 0);
    ASSERT_TRUE(a.getMembers().size() == 0);
}