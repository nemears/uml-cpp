#include "gtest/gtest.h"
#include "uml/association.h"
#include "uml/class.h"
#include "uml/property.h"

using namespace UML;

class AssociationTest : public ::testing::Test {
   
};

TEST_F(AssociationTest, addMemberEndFunctorTest) {
    Property p;
    Class c;
    Association a;
    p.setType(&c);
    ASSERT_NO_THROW(a.getMemberEnds().add(p));
    ASSERT_TRUE(a.getMemberEnds().size() == 1);
    ASSERT_TRUE(a.getMemberEnds().front() == &p);
    ASSERT_TRUE(a.getMembers().size() == 1);
    ASSERT_TRUE(a.getMembers().front() == &p);
    ASSERT_TRUE(a.getEndType().size() == 1);
    ASSERT_TRUE(a.getEndType().front() == &c);

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

TEST_F(AssociationTest, addOwnedEndTest) {
    Property p;
    Association a;
    ASSERT_NO_THROW(a.getOwnedEnds().add(p));
    ASSERT_TRUE(a.getOwnedEnds().size() == 1);
    ASSERT_TRUE(a.getOwnedEnds().front() == &p);
    ASSERT_TRUE(a.getOwnedMembers().size() == 1);
    ASSERT_TRUE(a.getOwnedMembers().front() == &p);
    ASSERT_TRUE(a.getOwnedElements().size() == 1);
    ASSERT_TRUE(a.getOwnedElements().front() == &p);
    ASSERT_TRUE(a.getFeatures().size() == 1);
    ASSERT_TRUE(a.getFeatures().front() == &p);
    ASSERT_TRUE(a.getMemberEnds().size() == 1);
    ASSERT_TRUE(a.getMemberEnds().front() == &p);

    ASSERT_TRUE(p.getOwningAssociation() == &a);
    ASSERT_TRUE(p.getAssociation() == &a);
    ASSERT_TRUE(p.getFeaturingClassifier() == &a);
    ASSERT_TRUE(p.getNamespace() == &a);
    ASSERT_TRUE(p.getMemberNamespace().size() == 1);
    ASSERT_TRUE(p.getMemberNamespace().front() == &a);
    ASSERT_TRUE(p.getOwner() == &a);
}

TEST_F(AssociationTest, setOwningAssociationTest) {
    Property p;
    Association a;
    ASSERT_NO_THROW(p.setOwningAssociation(&a));
    ASSERT_TRUE(a.getOwnedEnds().size() == 1);
    ASSERT_TRUE(a.getOwnedEnds().front() == &p);
    ASSERT_TRUE(a.getOwnedMembers().size() == 1);
    ASSERT_TRUE(a.getOwnedMembers().front() == &p);
    ASSERT_TRUE(a.getOwnedElements().size() == 1);
    ASSERT_TRUE(a.getOwnedElements().front() == &p);
    ASSERT_TRUE(a.getFeatures().size() == 1);
    ASSERT_TRUE(a.getFeatures().front() == &p);
    ASSERT_TRUE(a.getMemberEnds().size() == 1);
    ASSERT_TRUE(a.getMemberEnds().front() == &p);

    ASSERT_TRUE(p.getOwningAssociation() == &a);
    ASSERT_TRUE(p.getAssociation() == &a);
    ASSERT_TRUE(p.getFeaturingClassifier() == &a);
    ASSERT_TRUE(p.getNamespace() == &a);
    ASSERT_TRUE(p.getMemberNamespace().size() == 1);
    ASSERT_TRUE(p.getMemberNamespace().front() == &a);
    ASSERT_TRUE(p.getOwner() == &a);
}

TEST_F(AssociationTest, removeOwnedEndTest) {
    Property p;
    Association a;
    a.getOwnedEnds().add(p);
    ASSERT_NO_THROW(a.getOwnedEnds().remove(p));
    ASSERT_TRUE(a.getOwnedEnds().size() == 0);
    ASSERT_TRUE(a.getMemberEnds().size() == 0);
    ASSERT_TRUE(a.getFeatures().size() == 0);
    ASSERT_TRUE(a.getOwnedMembers().size() == 0);
    ASSERT_TRUE(a.getMembers().size() == 0);
    ASSERT_TRUE(a.getOwnedElements().size() == 0);

    ASSERT_TRUE(p.getOwningAssociation() == 0);
    ASSERT_TRUE(p.getAssociation() == 0);
    ASSERT_TRUE(p.getFeaturingClassifier() == 0);
    ASSERT_TRUE(p.getNamespace() == 0);
    ASSERT_TRUE(p.getMemberNamespace().size() == 0);
    ASSERT_TRUE(p.getOwner() == 0);
}

TEST_F(AssociationTest, setOwningAssociationW_NullTest) {
    Property p;
    Association a;
    a.getOwnedEnds().add(p);
    ASSERT_NO_THROW(p.setOwningAssociation(0));
    ASSERT_TRUE(a.getOwnedEnds().size() == 0);
    ASSERT_TRUE(a.getMemberEnds().size() == 0);
    ASSERT_TRUE(a.getFeatures().size() == 0);
    ASSERT_TRUE(a.getOwnedMembers().size() == 0);
    ASSERT_TRUE(a.getMembers().size() == 0);
    ASSERT_TRUE(a.getOwnedElements().size() == 0);

    ASSERT_TRUE(p.getOwningAssociation() == 0);
    ASSERT_TRUE(p.getAssociation() == 0);
    ASSERT_TRUE(p.getFeaturingClassifier() == 0);
    ASSERT_TRUE(p.getNamespace() == 0);
    ASSERT_TRUE(p.getMemberNamespace().size() == 0);
    ASSERT_TRUE(p.getOwner() == 0);
}

TEST_F(AssociationTest, overwriteOwningAssociationTest) {
    Property p;
    Association a;
    Association a2;
    a.getOwnedEnds().add(p);
    ASSERT_NO_THROW(p.setOwningAssociation(&a2));
    ASSERT_TRUE(a.getOwnedEnds().size() == 0);
    ASSERT_TRUE(a.getMemberEnds().size() == 0);
    ASSERT_TRUE(a.getFeatures().size() == 0);
    ASSERT_TRUE(a.getOwnedMembers().size() == 0);
    ASSERT_TRUE(a.getMembers().size() == 0);
    ASSERT_TRUE(a.getOwnedElements().size() == 0);

    ASSERT_TRUE(a2.getOwnedEnds().size() == 1);
    ASSERT_TRUE(a2.getOwnedEnds().front() == &p);
    ASSERT_TRUE(a2.getOwnedMembers().size() == 1);
    ASSERT_TRUE(a2.getOwnedMembers().front() == &p);
    ASSERT_TRUE(a2.getOwnedElements().size() == 1);
    ASSERT_TRUE(a2.getOwnedElements().front() == &p);
    ASSERT_TRUE(a2.getFeatures().size() == 1);
    ASSERT_TRUE(a2.getFeatures().front() == &p);
    ASSERT_TRUE(a2.getMemberEnds().size() == 1);
    ASSERT_TRUE(a2.getMemberEnds().front() == &p);

    ASSERT_TRUE(p.getOwningAssociation() == &a2);
    ASSERT_TRUE(p.getAssociation() == &a2);
    ASSERT_TRUE(p.getFeaturingClassifier() == &a2);
    ASSERT_TRUE(p.getNamespace() == &a2);
    ASSERT_TRUE(p.getMemberNamespace().size() == 1);
    ASSERT_TRUE(p.getMemberNamespace().front() == &a2);
    ASSERT_TRUE(p.getOwner() == &a2);
}

TEST_F(AssociationTest, addNavigableOwnedEndTest) {
    Property p;
    Association a;
    ASSERT_NO_THROW(a.getNavigableOwnedEnds().add(p));
    ASSERT_TRUE(a.getNavigableOwnedEnds().size() == 1);
    ASSERT_TRUE(a.getNavigableOwnedEnds().front() == &p);
    ASSERT_TRUE(a.getOwnedEnds().size() == 1);
    ASSERT_TRUE(a.getOwnedEnds().front() == &p);
    ASSERT_TRUE(a.getOwnedMembers().size() == 1);
    ASSERT_TRUE(a.getOwnedMembers().front() == &p);
    ASSERT_TRUE(a.getOwnedElements().size() == 1);
    ASSERT_TRUE(a.getOwnedElements().front() == &p);
    ASSERT_TRUE(a.getFeatures().size() == 1);
    ASSERT_TRUE(a.getFeatures().front() == &p);
    ASSERT_TRUE(a.getMemberEnds().size() == 1);
    ASSERT_TRUE(a.getMemberEnds().front() == &p);

    ASSERT_TRUE(p.getOwningAssociation() == &a);
    ASSERT_TRUE(p.getAssociation() == &a);
    ASSERT_TRUE(p.getFeaturingClassifier() == &a);
    ASSERT_TRUE(p.getNamespace() == &a);
    ASSERT_TRUE(p.getMemberNamespace().size() == 1);
    ASSERT_TRUE(p.getMemberNamespace().front() == &a);
    ASSERT_TRUE(p.getOwner() == &a);
}

TEST_F(AssociationTest, removeNavigableOwnedEndTest) {
    Property p;
    Association a;
    a.getNavigableOwnedEnds().add(p);
    ASSERT_NO_THROW(a.getNavigableOwnedEnds().remove(p));
    ASSERT_TRUE(a.getNavigableOwnedEnds().size() == 0);
    ASSERT_TRUE(a.getOwnedEnds().size() == 0);
    ASSERT_TRUE(a.getMemberEnds().size() == 0);
    ASSERT_TRUE(a.getFeatures().size() == 0);
    ASSERT_TRUE(a.getOwnedMembers().size() == 0);
    ASSERT_TRUE(a.getMembers().size() == 0);
    ASSERT_TRUE(a.getOwnedElements().size() == 0);

    ASSERT_TRUE(p.getOwningAssociation() == 0);
    ASSERT_TRUE(p.getAssociation() == 0);
    ASSERT_TRUE(p.getFeaturingClassifier() == 0);
    ASSERT_TRUE(p.getNamespace() == 0);
    ASSERT_TRUE(p.getMemberNamespace().size() == 0);
    ASSERT_TRUE(p.getOwner() == 0);
}

TEST_F(AssociationTest, navigableOwnedEndSetOwningAssociationW_NullTest) {
    Property p;
    Association a;
    a.getNavigableOwnedEnds().add(p);
    ASSERT_NO_THROW(p.setOwningAssociation(0));
    ASSERT_TRUE(a.getNavigableOwnedEnds().size() == 0);
    ASSERT_TRUE(a.getOwnedEnds().size() == 0);
    ASSERT_TRUE(a.getMemberEnds().size() == 0);
    ASSERT_TRUE(a.getFeatures().size() == 0);
    ASSERT_TRUE(a.getOwnedMembers().size() == 0);
    ASSERT_TRUE(a.getMembers().size() == 0);
    ASSERT_TRUE(a.getOwnedElements().size() == 0);

    ASSERT_TRUE(p.getOwningAssociation() == 0);
    ASSERT_TRUE(p.getAssociation() == 0);
    ASSERT_TRUE(p.getFeaturingClassifier() == 0);
    ASSERT_TRUE(p.getNamespace() == 0);
    ASSERT_TRUE(p.getMemberNamespace().size() == 0);
    ASSERT_TRUE(p.getOwner() == 0);
}

TEST_F(AssociationTest, navigableOwnedEndOverwriteOwningAssociationTest) {
    Property p;
    Association a;
    Association a2;
    a.getNavigableOwnedEnds().add(p);
    ASSERT_NO_THROW(p.setOwningAssociation(&a2));
    ASSERT_TRUE(a.getNavigableOwnedEnds().size() == 0);
    ASSERT_TRUE(a.getOwnedEnds().size() == 0);
    ASSERT_TRUE(a.getMemberEnds().size() == 0);
    ASSERT_TRUE(a.getFeatures().size() == 0);
    ASSERT_TRUE(a.getOwnedMembers().size() == 0);
    ASSERT_TRUE(a.getMembers().size() == 0);
    ASSERT_TRUE(a.getOwnedElements().size() == 0);

    ASSERT_TRUE(a2.getNavigableOwnedEnds().size() == 0);
    ASSERT_TRUE(a2.getOwnedEnds().size() == 1);
    ASSERT_TRUE(a2.getOwnedEnds().front() == &p);
    ASSERT_TRUE(a2.getOwnedMembers().size() == 1);
    ASSERT_TRUE(a2.getOwnedMembers().front() == &p);
    ASSERT_TRUE(a2.getOwnedElements().size() == 1);
    ASSERT_TRUE(a2.getOwnedElements().front() == &p);
    ASSERT_TRUE(a2.getFeatures().size() == 1);
    ASSERT_TRUE(a2.getFeatures().front() == &p);
    ASSERT_TRUE(a2.getMemberEnds().size() == 1);
    ASSERT_TRUE(a2.getMemberEnds().front() == &p);

    ASSERT_TRUE(p.getOwningAssociation() == &a2);
    ASSERT_TRUE(p.getAssociation() == &a2);
    ASSERT_TRUE(p.getFeaturingClassifier() == &a2);
    ASSERT_TRUE(p.getNamespace() == &a2);
    ASSERT_TRUE(p.getMemberNamespace().size() == 1);
    ASSERT_TRUE(p.getMemberNamespace().front() == &a2);
    ASSERT_TRUE(p.getOwner() == &a2);
}

TEST_F(AssociationTest, reindexTypeTest) {
    Property p;
    Class c;
    Association a;
    a.getMemberEnds().add(p);
    ASSERT_NO_THROW(p.setType(&c));

    ASSERT_TRUE(a.getMemberEnds().size() == 1);
    ASSERT_TRUE(a.getMemberEnds().front() == &p);
    ASSERT_TRUE(a.getMembers().size() == 1);
    ASSERT_TRUE(a.getMembers().front() == &p);
    ASSERT_TRUE(a.getEndType().size() == 1);
    ASSERT_TRUE(a.getEndType().front() == &c);

    ASSERT_TRUE(p.getAssociation() == &a);
    ASSERT_TRUE(p.getMemberNamespace().size() == 1);
    ASSERT_TRUE(p.getMemberNamespace().front() == &a);
}