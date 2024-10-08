#include "gtest/gtest.h"
#include "uml/uml-stable.h"

using namespace UML;

class DirectedRelationshipTest : public ::testing::Test {
   
};

TEST_F(DirectedRelationshipTest, sourceAndTargetTest) {
    UmlManager m;
    PackageMerge& dr = *m.create<PackageMerge>();
    Package& a = *m.create<Package>();
    Package& b = *m.create<Package>();
    dr.setReceivingPackage(a);
    dr.setMergedPackage(b);
    ASSERT_TRUE(dr.getSources().size() == 1);
    ASSERT_TRUE(dr.getSources().front() == &a);
    ASSERT_TRUE(dr.getTargets().size() == 1);
    ASSERT_TRUE(dr.getTargets().front() == &b);
}

TEST_F(DirectedRelationshipTest, addTargetFunctorTest) {
    UmlManager m;
    PackageMerge& dr = *m.create<PackageMerge>();
    Package& a = *m.create<Package>();
    Package& b = *m.create<Package>();
    dr.setReceivingPackage(a);
    dr.setMergedPackage(b);
    ASSERT_EQ(dr.getRelatedElements().size(), 2);
    ASSERT_TRUE(dr.getRelatedElements().contains(a.getID()));
    ASSERT_TRUE(dr.getRelatedElements().contains(b.getID()));
}

TEST_F(DirectedRelationshipTest, removeRelationshipFunctorTest) {
    UmlManager m;
    PackageMerge& dr = *m.create<PackageMerge>();
    Package& a = *m.create<Package>();
    Package& b = *m.create<Package>(); 
    a.getPackageMerge().add(dr);
    dr.setMergedPackage(&b);
    dr.setMergedPackage(0);
    a.getPackageMerge().remove(dr);
    ASSERT_FALSE(dr.getTargets().contains(a.getID()));
    ASSERT_FALSE(dr.getRelatedElements().contains(a.getID()));
    
    ASSERT_FALSE(dr.getSources().contains(b.getID()));
    ASSERT_FALSE(dr.getRelatedElements().contains(b.getID()));
}
