#include "gtest/gtest.h"
#include "uml/dataType.h"
#include "uml/operation.h"

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

TEST_F(DataTypeTest, OverridePropertyDataTypeW_NullTest) {
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

TEST_F(DataTypeTest, OverridePropertyDataTypeW_OtherTest) {
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

TEST_F(DataTypeTest, addOwnedOperationTest) {
    DataType d;
    Operation o;
    ASSERT_NO_THROW(d.getOwnedOperation().add(o));
    ASSERT_TRUE(d.getOwnedOperation().size() == 1);
    ASSERT_TRUE(d.getOwnedOperation().front() == &o);
    ASSERT_TRUE(d.getMembers().size() == 1);
    ASSERT_TRUE(d.getMembers().front() == &o);
    ASSERT_TRUE(d.getOwnedMembers().size() == 1);
    ASSERT_TRUE(d.getOwnedMembers().front() == &o);
    ASSERT_TRUE(d.getOwnedElements().size() == 1);

    ASSERT_TRUE(o.getDataType() == &d);
    ASSERT_TRUE(o.getFeaturingClassifier() == &d);
    ASSERT_TRUE(o.getNamespace() == &d);
    ASSERT_TRUE(o.getMemberNamespace().size() == 1);
    ASSERT_TRUE(o.getMemberNamespace().front() == &d);
    ASSERT_TRUE(o.getOwner() == &d);
}

TEST_F(DataTypeTest, OperationSetDataTypeTest) {
    DataType d;
    Operation o;
    ASSERT_NO_THROW(o.setDataType(&d));
    ASSERT_TRUE(d.getOwnedOperation().size() == 1);
    ASSERT_TRUE(d.getOwnedOperation().front() == &o);
    ASSERT_TRUE(d.getMembers().size() == 1);
    ASSERT_TRUE(d.getMembers().front() == &o);
    ASSERT_TRUE(d.getOwnedMembers().size() == 1);
    ASSERT_TRUE(d.getOwnedMembers().front() == &o);
    ASSERT_TRUE(d.getOwnedElements().size() == 1);

    ASSERT_TRUE(o.getDataType() == &d);
    ASSERT_TRUE(o.getFeaturingClassifier() == &d);
    ASSERT_TRUE(o.getNamespace() == &d);
    ASSERT_TRUE(o.getMemberNamespace().size() == 1);
    ASSERT_TRUE(o.getMemberNamespace().front() == &d);
    ASSERT_TRUE(o.getOwner() == &d);
}

TEST_F(DataTypeTest, RemoveOwnedOperationFunctorTest) {
    DataType d;
    Operation o;
    Operation o2;
    d.getOwnedOperation().add(o);
    d.getOwnedOperation().add(o2);
    ASSERT_NO_THROW(d.getOwnedOperation().remove(o2));

    ASSERT_TRUE(d.getOwnedOperation().size() == 1);
    ASSERT_TRUE(d.getOwnedOperation().front() == &o);
    ASSERT_TRUE(d.getMembers().size() == 1);
    ASSERT_TRUE(d.getMembers().front() == &o);
    ASSERT_TRUE(d.getOwnedMembers().size() == 1);
    ASSERT_TRUE(d.getOwnedMembers().front() == &o);
    ASSERT_TRUE(d.getOwnedElements().size() == 1);

    ASSERT_TRUE(o.getDataType() == &d);
    ASSERT_TRUE(o.getFeaturingClassifier() == &d);
    ASSERT_TRUE(o.getNamespace() == &d);
    ASSERT_TRUE(o.getMemberNamespace().size() == 1);
    ASSERT_TRUE(o.getMemberNamespace().front() == &d);
    ASSERT_TRUE(o.getOwner() == &d);

    ASSERT_TRUE(o2.getDataType() == 0);
    ASSERT_TRUE(o2.getFeaturingClassifier() == 0);
    ASSERT_TRUE(o2.getNamespace() == 0);
    ASSERT_TRUE(o2.getMemberNamespace().size() == 0);
    ASSERT_TRUE(o2.getOwner() == 0);
}

TEST_F(DataTypeTest, overwriteOperationDataTypeW_NullTest) {
    DataType d;
    Operation o;
    d.getOwnedOperation().add(o);
    ASSERT_NO_THROW(o.setDataType(0));
    ASSERT_TRUE(d.getOwnedOperation().size() == 0);
    ASSERT_TRUE(d.getFeatures().size() == 0);
    ASSERT_TRUE(d.getOwnedMembers().size() == 0);
    ASSERT_TRUE(d.getMembers().size() == 0);
    ASSERT_TRUE(d.getOwnedElements().size() == 0);

    ASSERT_TRUE(o.getDataType() == 0);
    ASSERT_TRUE(o.getFeaturingClassifier() == 0);
    ASSERT_TRUE(o.getNamespace() == 0);
    ASSERT_TRUE(o.getMemberNamespace().size() == 0);
    ASSERT_TRUE(o.getOwner() == 0);
}

TEST_F(DataTypeTest, overwriteOperationDataTypeW_OtherOperationTest) {
    DataType d;
    Operation o;
    DataType d2;
    d.getOwnedOperation().add(o);
    ASSERT_NO_THROW(o.setDataType(&d2));
    ASSERT_TRUE(d.getOwnedOperation().size() == 0);
    ASSERT_TRUE(d.getFeatures().size() == 0);
    ASSERT_TRUE(d.getOwnedMembers().size() == 0);
    ASSERT_TRUE(d.getMembers().size() == 0);
    ASSERT_TRUE(d.getOwnedElements().size() == 0);

    ASSERT_TRUE(d2.getOwnedOperation().size() == 1);
    ASSERT_TRUE(d2.getOwnedOperation().front() == &o);
    ASSERT_TRUE(d2.getFeatures().size() == 1);
    ASSERT_TRUE(d2.getFeatures().front() == &o);
    ASSERT_TRUE(d2.getOwnedMembers().size() == 1);
    ASSERT_TRUE(d2.getOwnedMembers().front() == &o);
    ASSERT_TRUE(d2.getMembers().size() == 1);
    ASSERT_TRUE(d2.getMembers().front() == &o);
    ASSERT_TRUE(d2.getOwnedElements().size() == 1);
    ASSERT_TRUE(d2.getOwnedElements().front() == &o);

    ASSERT_TRUE(o.getDataType() == &d2);
    ASSERT_TRUE(o.getFeaturingClassifier() == &d2);
    ASSERT_TRUE(o.getNamespace() == &d2);
    ASSERT_TRUE(o.getMemberNamespace().size() == 1);
    ASSERT_TRUE(o.getMemberNamespace().front() == &d2);
    ASSERT_TRUE(o.getOwner() == &d2);
}

TEST_F(DataTypeTest, reindexOwnedAttributeIDTest) {
    DataType d;
    Property p;
    d.getOwnedAttribute().add(p);
    boost::uuids::uuid id = boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d");
    p.setID(id);
    ASSERT_TRUE(d.getOwnedAttribute().get(id) == &p);
    ASSERT_TRUE(d.getAttributes().get(id) == &p);
    ASSERT_TRUE(d.getFeatures().get(id) == &p);
    ASSERT_TRUE(d.getMembers().get(id) == &p);
    ASSERT_TRUE(d.getOwnedMembers().get(id) == &p);
    ASSERT_TRUE(d.getOwnedElements().get(id) == &p);
}

TEST_F(DataTypeTest, reindexOwnedAttributeNameTest) {
    DataType d;
    Property p;
    p.setName("1");
    d.getOwnedAttribute().add(p);
    p.setName("2");
    ASSERT_TRUE(d.getOwnedAttribute().get("2") == &p);
    ASSERT_TRUE(d.getAttributes().get("2") == &p);
    ASSERT_TRUE(d.getFeatures().get("2") == &p);
    ASSERT_TRUE(d.getMembers().get("2") == &p);
    ASSERT_TRUE(d.getOwnedMembers().get("2") == &p);
    ASSERT_TRUE(d.getOwnedElements().get("2") == &p);
}

TEST_F(DataTypeTest, reindexOwnedOperationIDTest) {
    DataType d;
    Operation o;
    d.getOwnedOperation().add(o);
    boost::uuids::uuid id = boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d");
    o.setID(id);
    ASSERT_TRUE(d.getOwnedOperation().get(id) == &o);
    ASSERT_TRUE(d.getFeatures().get(id) == &o);
    ASSERT_TRUE(d.getMembers().get(id) == &o);
    ASSERT_TRUE(d.getOwnedMembers().get(id) == &o);
    ASSERT_TRUE(d.getOwnedElements().get(id) == &o);
}

TEST_F(DataTypeTest, reindexOwnedOperationNameTest) {
    DataType d;
    Operation o;
    o.setName("1");
    d.getOwnedOperation().add(o);
    o.setName("2");
    ASSERT_TRUE(d.getOwnedOperation().get("2") == &o);
    ASSERT_TRUE(d.getFeatures().get("2") == &o);
    ASSERT_TRUE(d.getMembers().get("2") == &o);
    ASSERT_TRUE(d.getOwnedMembers().get("2") == &o);
    ASSERT_TRUE(d.getOwnedElements().get("2") == &o);
}