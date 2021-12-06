#include "gtest/gtest.h"
#include "uml/uml-stable.h"

using namespace UML;

class DataTypeTest : public ::testing::Test {};

TEST_F(DataTypeTest, addOwnedAttributeTest) {
    UmlManager m;
    DataType& d = m.create<DataType>();
    Property& p = m.create<Property>();
    d.getOwnedAttributes().add(p);
    ASSERT_TRUE(d.getOwnedAttributes().size() == 1);
    ASSERT_TRUE(&d.getOwnedAttributes().front() == &p);
    ASSERT_TRUE(d.getAttributes().size() == 1);
    ASSERT_TRUE(&d.getAttributes().front() == &p);
    ASSERT_TRUE(d.getMembers().size() == 1);
    ASSERT_TRUE(&d.getMembers().front() == &p);
    ASSERT_TRUE(d.getOwnedMembers().size() == 1);
    ASSERT_TRUE(&d.getOwnedMembers().front() == &p);
    ASSERT_TRUE(d.getFeatures().size() == 1);
    ASSERT_TRUE(&d.getFeatures().front() == &p);
    ASSERT_TRUE(d.getOwnedElements().size() == 1);
    ASSERT_TRUE(*d.getOwnedElements().begin() == p);

    ASSERT_TRUE(p.getDataType() == &d);
    ASSERT_TRUE(p.getFeaturingClassifier() == &d);
    ASSERT_TRUE(p.getNamespace() == &d);
    ASSERT_TRUE(p.getOwner() == &d);
}

TEST_F(DataTypeTest, setDataTypeTest) {
    UmlManager m;
    DataType& d = m.create<DataType>();
    Property& p = m.create<Property>();
    p.setDataType(&d);
    ASSERT_TRUE(d.getOwnedAttributes().size() == 1);
    ASSERT_TRUE(&d.getOwnedAttributes().front() == &p);
    ASSERT_TRUE(d.getAttributes().size() == 1);
    ASSERT_TRUE(&d.getAttributes().front() == &p);
    ASSERT_TRUE(d.getMembers().size() == 1);
    ASSERT_TRUE(&d.getMembers().front() == &p);
    ASSERT_TRUE(d.getOwnedMembers().size() == 1);
    ASSERT_TRUE(&d.getOwnedMembers().front() == &p);
    ASSERT_TRUE(d.getFeatures().size() == 1);
    ASSERT_TRUE(&d.getFeatures().front() == &p);
    ASSERT_TRUE(d.getOwnedElements().size() == 1);
    ASSERT_TRUE(*d.getOwnedElements().begin() == p);

    ASSERT_TRUE(p.getDataType() == &d);
    ASSERT_TRUE(p.getFeaturingClassifier() == &d);
    ASSERT_TRUE(p.getNamespace() == &d);
    ASSERT_TRUE(p.getOwner() == &d);
}

TEST_F(DataTypeTest, removeOwnedAttributeFunctorTest) {
    UmlManager m;
    DataType& d = m.create<DataType>();
    Property& p = m.create<Property>();
    Property& p2 = m.create<Property>();
    d.getOwnedAttributes().add(p);
    d.getOwnedAttributes().add(p2);
    ASSERT_NO_THROW(d.getOwnedAttributes().remove(p2));
    
    ASSERT_TRUE(d.getOwnedAttributes().size() == 1);
    ASSERT_TRUE(&d.getOwnedAttributes().front() == &p);
    ASSERT_TRUE(d.getAttributes().size() == 1);
    ASSERT_TRUE(&d.getAttributes().front() == &p);
    ASSERT_TRUE(d.getMembers().size() == 1);
    ASSERT_TRUE(&d.getMembers().front() == &p);
    ASSERT_TRUE(d.getOwnedMembers().size() == 1);
    ASSERT_TRUE(&d.getOwnedMembers().front() == &p);
    ASSERT_TRUE(d.getFeatures().size() == 1);
    ASSERT_TRUE(&d.getFeatures().front() == &p);
    ASSERT_TRUE(d.getOwnedElements().size() == 1);
    ASSERT_TRUE(*d.getOwnedElements().begin() == p);

    ASSERT_TRUE(p.getDataType() == &d);
    ASSERT_TRUE(p.getFeaturingClassifier() == &d);
    ASSERT_TRUE(p.getNamespace() == &d);
    ASSERT_TRUE(p.getOwner() == &d);

    ASSERT_TRUE(p2.getDataType() == 0);
    ASSERT_TRUE(p2.getFeaturingClassifier() == 0);
    ASSERT_TRUE(p2.getNamespace() == 0);
    ASSERT_TRUE(p2.getOwner() == 0);
}

TEST_F(DataTypeTest, OverridePropertyDataTypeW_NullTest) {
    UmlManager m;
    DataType& d = m.create<DataType>();
    Property& p = m.create<Property>();
    Property& p2 = m.create<Property>();
    d.getOwnedAttributes().add(p);
    d.getOwnedAttributes().add(p2);
    ASSERT_NO_THROW(p2.setDataType(0));
    
    ASSERT_TRUE(d.getOwnedAttributes().size() == 1);
    ASSERT_TRUE(&d.getOwnedAttributes().front() == &p);
    ASSERT_TRUE(d.getAttributes().size() == 1);
    ASSERT_TRUE(&d.getAttributes().front() == &p);
    ASSERT_TRUE(d.getMembers().size() == 1);
    ASSERT_TRUE(&d.getMembers().front() == &p);
    ASSERT_TRUE(d.getOwnedMembers().size() == 1);
    ASSERT_TRUE(&d.getOwnedMembers().front() == &p);
    ASSERT_TRUE(d.getFeatures().size() == 1);
    ASSERT_TRUE(&d.getFeatures().front() == &p);
    ASSERT_TRUE(d.getOwnedElements().size() == 1);
    ASSERT_TRUE(*d.getOwnedElements().begin() == p);

    ASSERT_TRUE(p.getDataType() == &d);
    ASSERT_TRUE(p.getFeaturingClassifier() == &d);
    ASSERT_TRUE(p.getNamespace() == &d);
    ASSERT_TRUE(p.getOwner() == &d);

    ASSERT_TRUE(p2.getDataType() == 0);
    ASSERT_TRUE(p2.getFeaturingClassifier() == 0);
    ASSERT_TRUE(p2.getNamespace() == 0);
    ASSERT_TRUE(p2.getOwner() == 0);
}

TEST_F(DataTypeTest, OverridePropertyDataTypeW_OtherTest) {
    UmlManager m;
    DataType& d = m.create<DataType>();
    Property& p = m.create<Property>();
    Property& p2 = m.create<Property>();
    DataType& d2 = m.create<DataType>();
    d.getOwnedAttributes().add(p);
    d.getOwnedAttributes().add(p2);
    ASSERT_NO_THROW(p2.setDataType(&d2));
    
    ASSERT_TRUE(d.getOwnedAttributes().size() == 1);
    ASSERT_TRUE(&d.getOwnedAttributes().front() == &p);
    ASSERT_TRUE(d.getAttributes().size() == 1);
    ASSERT_TRUE(&d.getAttributes().front() == &p);
    ASSERT_TRUE(d.getMembers().size() == 1);
    ASSERT_TRUE(&d.getMembers().front() == &p);
    ASSERT_TRUE(d.getOwnedMembers().size() == 1);
    ASSERT_TRUE(&d.getOwnedMembers().front() == &p);
    ASSERT_TRUE(d.getFeatures().size() == 1);
    ASSERT_TRUE(&d.getFeatures().front() == &p);
    ASSERT_TRUE(d.getOwnedElements().size() == 1);
    ASSERT_TRUE(*d.getOwnedElements().begin() == p);

    ASSERT_TRUE(p.getDataType() == &d);
    ASSERT_TRUE(p.getFeaturingClassifier() == &d);
    ASSERT_TRUE(p.getNamespace() == &d);
    ASSERT_TRUE(p.getOwner() == &d);

    ASSERT_TRUE(d2.getOwnedAttributes().size() == 1);
    ASSERT_TRUE(&d2.getOwnedAttributes().front() == &p2);
    ASSERT_TRUE(d2.getAttributes().size() == 1);
    ASSERT_TRUE(&d2.getAttributes().front() == &p2);
    ASSERT_TRUE(d2.getMembers().size() == 1);
    ASSERT_TRUE(&d2.getMembers().front() == &p2);
    ASSERT_TRUE(d2.getOwnedMembers().size() == 1);
    ASSERT_TRUE(&d2.getOwnedMembers().front() == &p2);
    ASSERT_TRUE(d2.getFeatures().size() == 1);
    ASSERT_TRUE(&d2.getFeatures().front() == &p2);
    ASSERT_TRUE(d2.getOwnedElements().size() == 1);
    ASSERT_TRUE(*d2.getOwnedElements().begin() == p2);

    ASSERT_TRUE(p2.getDataType() == &d2);
    ASSERT_TRUE(p2.getFeaturingClassifier() == &d2);
    ASSERT_TRUE(p2.getNamespace() == &d2);
    ASSERT_TRUE(p2.getOwner() == &d2);
}

TEST_F(DataTypeTest, addOwnedOperationTest) {
    UmlManager m;
    DataType& d = m.create<DataType>();
    Operation& o = m.create<Operation>();
    ASSERT_NO_THROW(d.getOwnedOperations().add(o));
    ASSERT_TRUE(d.getOwnedOperations().size() == 1);
    ASSERT_TRUE(&d.getOwnedOperations().front() == &o);
    ASSERT_TRUE(d.getMembers().size() == 1);
    ASSERT_TRUE(&d.getMembers().front() == &o);
    ASSERT_TRUE(d.getOwnedMembers().size() == 1);
    ASSERT_TRUE(&d.getOwnedMembers().front() == &o);
    ASSERT_TRUE(d.getOwnedElements().size() == 1);

    ASSERT_TRUE(o.getDataType() == &d);
    ASSERT_TRUE(o.getFeaturingClassifier() == &d);
    ASSERT_TRUE(o.getNamespace() == &d);
    ASSERT_TRUE(o.getOwner() == &d);
}

TEST_F(DataTypeTest, OperationSetDataTypeTest) {
    UmlManager m;
    DataType& d = m.create<DataType>();
    Operation& o = m.create<Operation>();
    ASSERT_NO_THROW(o.setDataType(&d));
    ASSERT_TRUE(d.getOwnedOperations().size() == 1);
    ASSERT_TRUE(&d.getOwnedOperations().front() == &o);
    ASSERT_TRUE(d.getMembers().size() == 1);
    ASSERT_TRUE(&d.getMembers().front() == &o);
    ASSERT_TRUE(d.getOwnedMembers().size() == 1);
    ASSERT_TRUE(&d.getOwnedMembers().front() == &o);
    ASSERT_TRUE(d.getOwnedElements().size() == 1);

    ASSERT_TRUE(o.getDataType() == &d);
    ASSERT_TRUE(o.getFeaturingClassifier() == &d);
    ASSERT_TRUE(o.getNamespace() == &d);
    ASSERT_TRUE(o.getOwner() == &d);
}

TEST_F(DataTypeTest, RemoveOwnedOperationFunctorTest) {
    UmlManager m;
    DataType& d = m.create<DataType>();
    Operation& o = m.create<Operation>();
    Operation& o2 = m.create<Operation>();
    d.getOwnedOperations().add(o);
    d.getOwnedOperations().add(o2);
    ASSERT_NO_THROW(d.getOwnedOperations().remove(o2));

    ASSERT_TRUE(d.getOwnedOperations().size() == 1);
    ASSERT_TRUE(&d.getOwnedOperations().front() == &o);
    ASSERT_TRUE(d.getMembers().size() == 1);
    ASSERT_TRUE(&d.getMembers().front() == &o);
    ASSERT_TRUE(d.getOwnedMembers().size() == 1);
    ASSERT_TRUE(&d.getOwnedMembers().front() == &o);
    ASSERT_TRUE(d.getOwnedElements().size() == 1);

    ASSERT_TRUE(o.getDataType() == &d);
    ASSERT_TRUE(o.getFeaturingClassifier() == &d);
    ASSERT_TRUE(o.getNamespace() == &d);
    ASSERT_TRUE(o.getOwner() == &d);

    ASSERT_TRUE(o2.getDataType() == 0);
    ASSERT_TRUE(o2.getFeaturingClassifier() == 0);
    ASSERT_TRUE(o2.getNamespace() == 0);
    ASSERT_TRUE(o2.getOwner() == 0);
}

TEST_F(DataTypeTest, overwriteOperationDataTypeW_NullTest) {
    UmlManager m;
    DataType& d = m.create<DataType>();
    Operation& o = m.create<Operation>();
    d.getOwnedOperations().add(o);
    ASSERT_NO_THROW(o.setDataType(0));
    ASSERT_TRUE(d.getOwnedOperations().size() == 0);
    ASSERT_TRUE(d.getFeatures().size() == 0);
    ASSERT_TRUE(d.getOwnedMembers().size() == 0);
    ASSERT_TRUE(d.getMembers().size() == 0);
    ASSERT_TRUE(d.getOwnedElements().size() == 0);

    ASSERT_TRUE(o.getDataType() == 0);
    ASSERT_TRUE(o.getFeaturingClassifier() == 0);
    ASSERT_TRUE(o.getNamespace() == 0);
    ASSERT_TRUE(o.getOwner() == 0);
}

TEST_F(DataTypeTest, overwriteOperationDataTypeW_OtherOperationTest) {
    UmlManager m;
    DataType& d = m.create<DataType>();
    Operation& o = m.create<Operation>();
    DataType& d2 = m.create<DataType>();
    d.getOwnedOperations().add(o);
    ASSERT_NO_THROW(o.setDataType(&d2));
    ASSERT_TRUE(d.getFeatures().size() == 0);
    ASSERT_TRUE(d.getOwnedMembers().size() == 0);
    ASSERT_TRUE(d.getMembers().size() == 0);
    ASSERT_TRUE(d.getOwnedElements().size() == 0);

    ASSERT_TRUE(d2.getOwnedOperations().size() == 1);
    ASSERT_TRUE(&d2.getOwnedOperations().front() == &o);
    ASSERT_TRUE(d2.getFeatures().size() == 1);
    ASSERT_TRUE(&d2.getFeatures().front() == &o);
    ASSERT_TRUE(d2.getOwnedMembers().size() == 1);
    ASSERT_TRUE(&d2.getOwnedMembers().front() == &o);
    ASSERT_TRUE(d2.getMembers().size() == 1);
    ASSERT_TRUE(&d2.getMembers().front() == &o);
    ASSERT_TRUE(d2.getOwnedElements().size() == 1);
    ASSERT_TRUE(*d2.getOwnedElements().begin() == o);

    ASSERT_TRUE(o.getDataType() == &d2);
    ASSERT_TRUE(o.getFeaturingClassifier() == &d2);
    ASSERT_TRUE(o.getNamespace() == &d2);
    ASSERT_TRUE(o.getOwner() == &d2);
}

TEST_F(DataTypeTest, reindexOwnedAttributeIDTest) {
    UmlManager m;
    DataType d = m.create<DataType>();
    Property p = m.create<Property>();
    d.getOwnedAttributes().add(p);
    ID id = ID::fromString("16c345b4_5ae2_41ca_a0e7_a9c3");
    p.setID(id);
    ASSERT_EQ(d.getOwnedAttributes().get(id), p);
    ASSERT_EQ(d.getAttributes().get(id), p);
    ASSERT_EQ(d.getFeatures().get(id), p);
    ASSERT_EQ(d.getMembers().get(id) , p);
    ASSERT_EQ(d.getOwnedMembers().get(id), p);
    ASSERT_EQ(d.getOwnedElements().get(id), p);
}

// TEST_F(DataTypeTest, reindexOwnedAttributeNameTest) {
//     DataType d;
//     Property p;
//     p.setName("1");
//     d.getOwnedAttributes().add(p);
//     p.setName("2");
//     ASSERT_TRUE(d.getOwnedAttributes().get("2") == &p);
//     ASSERT_TRUE(d.getAttributes().get("2") == &p);
//     ASSERT_TRUE(d.getFeatures().get("2") == &p);
//     ASSERT_TRUE(d.getMembers().get("2") == &p);
//     ASSERT_TRUE(d.getOwnedMembers().get("2") == &p);
//     ASSERT_TRUE(d.getOwnedElements().get("2") == &p);
// }

TEST_F(DataTypeTest, reindexOwnedOperationIDTest) {
    UmlManager m;
    DataType& d = m.create<DataType>();
    Operation& o = m.create<Operation>();
    d.getOwnedOperations().add(o);
    ID id = ID::fromString("16c345b4_5ae2_41ca_a0e7_a9c3");
    o.setID(id);
    ASSERT_TRUE(&d.getOwnedOperations().get(id) == &o);
    ASSERT_TRUE(&d.getFeatures().get(id) == &o);
    ASSERT_TRUE(&d.getMembers().get(id) == &o);
    ASSERT_TRUE(&d.getOwnedMembers().get(id) == &o);
    ASSERT_TRUE(&d.getOwnedElements().get(id) == &o);
}

// TEST_F(DataTypeTest, reindexOwnedOperationNameTest) {
//     DataType d;
//     Operation o;
//     o.setName("1");
//     d.getOwnedOperation().add(o);
//     o.setName("2");
//     ASSERT_TRUE(d.getOwnedOperation().get("2") == &o);
//     ASSERT_TRUE(d.getFeatures().get("2") == &o);
//     ASSERT_TRUE(d.getMembers().get("2") == &o);
//     ASSERT_TRUE(d.getOwnedMembers().get("2") == &o);
//     ASSERT_TRUE(d.getOwnedElements().get("2") == &o);
// }

TEST_F(DataTypeTest, copyDataTypeTest) {
    UmlManager m;
    DataType& d = m.create<DataType>();
    Operation& o = m.create<Operation>();
    Property& p = m.create<Property>();
    Package& pkg = m.create<Package>();
    d.setName("test");
    ID id = d.getID();
    d.getOwnedOperations().add(o);
    d.getOwnedAttributes().add(p);
    d.setOwningPackage(&pkg);
    DataType d2 = d;
    ASSERT_TRUE(d2.getID() == id);
    ASSERT_TRUE(d2.getName().compare("test") == 0);
    ASSERT_TRUE(d2.getOwnedAttributes().size() == 1);
    ASSERT_TRUE(&d2.getOwnedAttributes().front() == &p);
    ASSERT_TRUE(d2.getAttributes().size() == 1);
    ASSERT_TRUE(&d2.getAttributes().front() == &p);
    ASSERT_TRUE(d2.getOwnedOperations().size() == 1);
    ASSERT_TRUE(&d2.getOwnedOperations().front() == &o);
    ASSERT_TRUE(d2.getFeatures().size() == 2);
    ASSERT_TRUE(&d2.getFeatures().front() == &o);
    ASSERT_TRUE(&d2.getFeatures().back() == &p);
    ASSERT_TRUE(d2.getOwnedMembers().size() == 2);
    ASSERT_TRUE(&d2.getOwnedMembers().front() == &o);
    ASSERT_TRUE(&d2.getOwnedMembers().back() == &p);
    ASSERT_TRUE(d2.getMembers().size() == 2);
    ASSERT_TRUE(&d2.getMembers().front() == &o);
    ASSERT_TRUE(&d2.getMembers().back() == &p);
    ASSERT_TRUE(d2.getOwnedElements().size() == 2);
    ASSERT_TRUE(*d2.getOwnedElements().begin() == o);
    ASSERT_TRUE(*(d2.getOwnedElements().begin()++) == p);
    ASSERT_TRUE(d2.getOwningPackage() == &pkg);
    ASSERT_TRUE(d2.getNamespace() == &pkg);
    ASSERT_TRUE(d2.getOwner() == &pkg);
}