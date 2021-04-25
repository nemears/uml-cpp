#include "gtest/gtest.h"
#include "uml/dataType.h"

using namespace UML;

class DataTypeTest : public ::testing::Test {};

TEST_F(DataTypeTest, addOwnedAttributeTest) {
    DataType d;
    Property p;
    d.getOwnedAttribute().add(p);
    ASSERT_TRUE(d.getOwnedAttribute().size() == 1);
    ASSERT_TRUE(d.getOwnedAttribute().front() == &p);
    ASSERT_TRUE(d.getAttributes().size() == 1);
    ASSERT_TRUE(d.getAttributes().front() == &p);
    ASSERT_TRUE(d.getMembers().size() == 1);
    ASSERT_TRUE(d.getMembers().front() == &p);
    ASSERT_TRUE(d.getOwnedMembers().size() == 1);
    ASSERT_TRUE(d.getOwnedMembers().front() == &p);
    ASSERT_TRUE(d.getFeatures().size() == 1);
    ASSERT_TRUE(d.getFeatures().front() == &p);
    ASSERT_TRUE(d.getOwnedElements().size() == 1);
    ASSERT_TRUE(d.getOwnedElements().front() == &p);

    ASSERT_TRUE(p.getDataType() == &d);
    ASSERT_TRUE(p.getClassifier() == &d);
    ASSERT_TRUE(p.getFeaturingClassifier() == &d);
    ASSERT_TRUE(p.getMemberNamespace().size() == 1);
    ASSERT_TRUE(p.getMemberNamespace().front() == &d);
    ASSERT_TRUE(p.getNamespace() == &d);
    ASSERT_TRUE(p.getOwner() == &d);
}

TEST_F(DataTypeTest, setDataTypeTest) {
    DataType d;
    Property p;
    p.setDataType(&d);
    ASSERT_TRUE(d.getOwnedAttribute().size() == 1);
    ASSERT_TRUE(d.getOwnedAttribute().front() == &p);
    ASSERT_TRUE(d.getAttributes().size() == 1);
    ASSERT_TRUE(d.getAttributes().front() == &p);
    ASSERT_TRUE(d.getMembers().size() == 1);
    ASSERT_TRUE(d.getMembers().front() == &p);
    ASSERT_TRUE(d.getOwnedMembers().size() == 1);
    ASSERT_TRUE(d.getOwnedMembers().front() == &p);
    ASSERT_TRUE(d.getFeatures().size() == 1);
    ASSERT_TRUE(d.getFeatures().front() == &p);
    ASSERT_TRUE(d.getOwnedElements().size() == 1);
    ASSERT_TRUE(d.getOwnedElements().front() == &p);

    ASSERT_TRUE(p.getDataType() == &d);
    ASSERT_TRUE(p.getClassifier() == &d);
    ASSERT_TRUE(p.getFeaturingClassifier() == &d);
    ASSERT_TRUE(p.getMemberNamespace().size() == 1);
    ASSERT_TRUE(p.getMemberNamespace().front() == &d);
    ASSERT_TRUE(p.getNamespace() == &d);
    ASSERT_TRUE(p.getOwner() == &d);
}

TEST_F(DataTypeTest, removeOwnedAttributeFunctorTest) {
    DataType d;
    Property p;
    Property p2;
    d.getOwnedAttribute().add(p);
    d.getOwnedAttribute().add(p2);
    ASSERT_NO_THROW(d.getOwnedAttribute().remove(p2));
    
    ASSERT_TRUE(d.getOwnedAttribute().size() == 1);
    ASSERT_TRUE(d.getOwnedAttribute().front() == &p);
    ASSERT_TRUE(d.getAttributes().size() == 1);
    ASSERT_TRUE(d.getAttributes().front() == &p);
    ASSERT_TRUE(d.getMembers().size() == 1);
    ASSERT_TRUE(d.getMembers().front() == &p);
    ASSERT_TRUE(d.getOwnedMembers().size() == 1);
    ASSERT_TRUE(d.getOwnedMembers().front() == &p);
    ASSERT_TRUE(d.getFeatures().size() == 1);
    ASSERT_TRUE(d.getFeatures().front() == &p);
    ASSERT_TRUE(d.getOwnedElements().size() == 1);
    ASSERT_TRUE(d.getOwnedElements().front() == &p);

    ASSERT_TRUE(p.getDataType() == &d);
    ASSERT_TRUE(p.getClassifier() == &d);
    ASSERT_TRUE(p.getFeaturingClassifier() == &d);
    ASSERT_TRUE(p.getMemberNamespace().size() == 1);
    ASSERT_TRUE(p.getMemberNamespace().front() == &d);
    ASSERT_TRUE(p.getNamespace() == &d);
    ASSERT_TRUE(p.getOwner() == &d);

    ASSERT_TRUE(p2.getDataType() == 0);
    ASSERT_TRUE(p2.getClassifier() == 0);
    ASSERT_TRUE(p2.getFeaturingClassifier() == 0);
    ASSERT_TRUE(p2.getMemberNamespace().size() == 0);
    ASSERT_TRUE(p2.getNamespace() == 0);
    ASSERT_TRUE(p2.getOwner() == 0);
}

TEST_F(DataTypeTest, OverrideDataTypeW_NullTest) {
    DataType d;
    Property p;
    Property p2;
    d.getOwnedAttribute().add(p);
    d.getOwnedAttribute().add(p2);
    ASSERT_NO_THROW(p2.setDataType(0));
    
    ASSERT_TRUE(d.getOwnedAttribute().size() == 1);
    ASSERT_TRUE(d.getOwnedAttribute().front() == &p);
    ASSERT_TRUE(d.getAttributes().size() == 1);
    ASSERT_TRUE(d.getAttributes().front() == &p);
    ASSERT_TRUE(d.getMembers().size() == 1);
    ASSERT_TRUE(d.getMembers().front() == &p);
    ASSERT_TRUE(d.getOwnedMembers().size() == 1);
    ASSERT_TRUE(d.getOwnedMembers().front() == &p);
    ASSERT_TRUE(d.getFeatures().size() == 1);
    ASSERT_TRUE(d.getFeatures().front() == &p);
    ASSERT_TRUE(d.getOwnedElements().size() == 1);
    ASSERT_TRUE(d.getOwnedElements().front() == &p);

    ASSERT_TRUE(p.getDataType() == &d);
    ASSERT_TRUE(p.getClassifier() == &d);
    ASSERT_TRUE(p.getFeaturingClassifier() == &d);
    ASSERT_TRUE(p.getMemberNamespace().size() == 1);
    ASSERT_TRUE(p.getMemberNamespace().front() == &d);
    ASSERT_TRUE(p.getNamespace() == &d);
    ASSERT_TRUE(p.getOwner() == &d);

    ASSERT_TRUE(p2.getDataType() == 0);
    ASSERT_TRUE(p2.getClassifier() == 0);
    ASSERT_TRUE(p2.getFeaturingClassifier() == 0);
    ASSERT_TRUE(p2.getMemberNamespace().size() == 0);
    ASSERT_TRUE(p2.getNamespace() == 0);
    ASSERT_TRUE(p2.getOwner() == 0);
}

TEST_F(DataTypeTest, OverrideDataTypeW_OtherTest) {
    DataType d;
    Property p;
    Property p2;
    DataType d2;
    d.getOwnedAttribute().add(p);
    d.getOwnedAttribute().add(p2);
    ASSERT_NO_THROW(p2.setDataType(&d2));
    
    ASSERT_TRUE(d.getOwnedAttribute().size() == 1);
    ASSERT_TRUE(d.getOwnedAttribute().front() == &p);
    ASSERT_TRUE(d.getAttributes().size() == 1);
    ASSERT_TRUE(d.getAttributes().front() == &p);
    ASSERT_TRUE(d.getMembers().size() == 1);
    ASSERT_TRUE(d.getMembers().front() == &p);
    ASSERT_TRUE(d.getOwnedMembers().size() == 1);
    ASSERT_TRUE(d.getOwnedMembers().front() == &p);
    ASSERT_TRUE(d.getFeatures().size() == 1);
    ASSERT_TRUE(d.getFeatures().front() == &p);
    ASSERT_TRUE(d.getOwnedElements().size() == 1);
    ASSERT_TRUE(d.getOwnedElements().front() == &p);

    ASSERT_TRUE(p.getDataType() == &d);
    ASSERT_TRUE(p.getClassifier() == &d);
    ASSERT_TRUE(p.getFeaturingClassifier() == &d);
    ASSERT_TRUE(p.getMemberNamespace().size() == 1);
    ASSERT_TRUE(p.getMemberNamespace().front() == &d);
    ASSERT_TRUE(p.getNamespace() == &d);
    ASSERT_TRUE(p.getOwner() == &d);

    ASSERT_TRUE(d2.getOwnedAttribute().size() == 1);
    ASSERT_TRUE(d2.getOwnedAttribute().front() == &p2);
    ASSERT_TRUE(d2.getAttributes().size() == 1);
    ASSERT_TRUE(d2.getAttributes().front() == &p2);
    ASSERT_TRUE(d2.getMembers().size() == 1);
    ASSERT_TRUE(d2.getMembers().front() == &p2);
    ASSERT_TRUE(d2.getOwnedMembers().size() == 1);
    ASSERT_TRUE(d2.getOwnedMembers().front() == &p2);
    ASSERT_TRUE(d2.getFeatures().size() == 1);
    ASSERT_TRUE(d2.getFeatures().front() == &p2);
    ASSERT_TRUE(d2.getOwnedElements().size() == 1);
    ASSERT_TRUE(d2.getOwnedElements().front() == &p2);

    ASSERT_TRUE(p2.getDataType() == &d2);
    ASSERT_TRUE(p2.getClassifier() == &d2);
    ASSERT_TRUE(p2.getFeaturingClassifier() == &d2);
    ASSERT_TRUE(p2.getMemberNamespace().size() == 1);
    ASSERT_TRUE(p2.getMemberNamespace().front() == &d2);
    ASSERT_TRUE(p2.getNamespace() == &d2);
    ASSERT_TRUE(p2.getOwner() == &d2);
}