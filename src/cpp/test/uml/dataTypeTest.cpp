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