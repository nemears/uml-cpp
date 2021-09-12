#include "gtest/gtest.h"
#include "uml/directedRelationship.h"
#include "uml/package.h"
#include "uml/packageMerge.h"

using namespace UML;

class DirectedRelationshipTest : public ::testing::Test {
   
};

TEST_F(DirectedRelationshipTest, sourceAndTargetTest) {
    UmlManager m;
    PackageMerge dr = m.create<PackageMerge>();
    Package a = m.create<Package>();
    Package b = m.create<Package>();
    dr.getSources().add(a);
    dr.getTargets().add(b);
    ASSERT_TRUE(dr.getSources().size() == 1);
    ASSERT_TRUE(&dr.getSources().front() == &a);
    ASSERT_TRUE(dr.getTargets().size() == 1);
    ASSERT_TRUE(&dr.getTargets().front() == &b);
}

TEST_F(DirectedRelationshipTest, addTargetFunctorTest) {
    UmlManager m;
    PackageMerge dr = m.create<PackageMerge>();
    Package a = m.create<Package>();
    Package b = m.create<Package>();
    dr.getTargets().add(a);
    dr.getSources().add(b);
    ASSERT_TRUE(dr.getRelatedElements().size() == 2);
    ASSERT_TRUE(&dr.getRelatedElements().front() == &a);
    ASSERT_TRUE(&dr.getRelatedElements().back() == &b);
    ASSERT_TRUE(a.getRelationships().size() == 1);
    ASSERT_TRUE(&a.getRelationships().front() == &dr);
    ASSERT_TRUE(b.getRelationships().size() == 1);
    ASSERT_TRUE(&b.getRelationships().front() == &dr);
}

TEST_F(DirectedRelationshipTest, duplicateRelationshipExceptionTest) {
    UmlManager m;
    PackageMerge dr = m.create<PackageMerge>();
    Package a = m.create<Package>();
    Package b = m.create<Package>();
    dr.getTargets().add(a);
    dr.getSources().add(b);
    ASSERT_THROW(dr.getRelatedElements().add(a), DuplicateRelatedElementException);
}

TEST_F(DirectedRelationshipTest, removeRelationshipFunctorTest) {
    UmlManager m;
    PackageMerge& dr = m.create<PackageMerge>();
    Package& a = m.create<Package>();
    Package& b = m.create<Package>(); 
    a.getPackageMerge().add(dr);
    dr.setMergedPackage(&b);
    dr.setMergedPackage(0);
    a.getPackageMerge().remove(dr);
    ASSERT_FALSE(dr.getTargets().count(a.getID()));
    ASSERT_FALSE(dr.getRelatedElements().count(a.getID()));
    ASSERT_FALSE(a.getRelationships().count(dr.getID()));
    
    ASSERT_FALSE(dr.getSources().count(b.getID()));
    ASSERT_FALSE(dr.getRelatedElements().count(b.getID()));
    ASSERT_FALSE(b.getRelationships().count(dr.getID()));
}