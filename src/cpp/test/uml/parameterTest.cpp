#include "gtest/gtest.h"
#include "uml/parameter.h"
#include "uml/activity.h"
#include "uml/operation.h"

using namespace UML;

class ParameterTest : public ::testing::Test {};

TEST_F(ParameterTest, TestSetDirection) {
    UmlManager m;
    Parameter p = m.create<Parameter>();
    p.setDirection(ParameterDirectionKind::IN);
    ASSERT_TRUE(p.getDirection() == ParameterDirectionKind::IN);
}

TEST_F(ParameterTest, TestGetNotSetDirection) {
    UmlManager m;
    Parameter p = m.create<Parameter>();
    ASSERT_TRUE(p.getDirection() == ParameterDirectionKind::NONE);
}

TEST_F(ParameterTest, reindexID_ownerTest) {
    UmlManager m;
    Activity a = m.create<Activity>();
    Parameter p = m.create<Parameter>();
    a.getParameters().add(p);
    p.setNamespace(&a);
    a.getMembers().add(p);
    p.setID("190d1cb9_13dc_44e6_a064_1268");
    ASSERT_NO_THROW(a.getParameters().get(p.getID()));
    ASSERT_NO_THROW(a.getMembers().get(p.getID()));
    ASSERT_NO_THROW(a.getOwnedElements().get(p.getID()));
}

// // TODO rethink this test
// TEST_F(ParameterTest, reindexID_operationTest) {
//     Activity a1;
//     Activity a2;
//     Parameter p1;
//     Parameter p2;
//     Class c;
//     Operation o;
//     a1.getParameters().add(p1);
//     a1.getParameters().add(p2);
//     a2.getParameters().add(p1);
//     o.getMethods().add(a1);
//     o.getMethods().add(a2);
//     c.getOperations().add(o);
//     p1.setName("190d1cb9-13dc-44e6-a064-126891ae0033");
//     p2.setID("eb092018-0bef-4ad6-b80f-05fa124f98c3");
//     ASSERT_TRUE(a1.getParameters().get(p1.getID()));
//     ASSERT_TRUE(a1.getParameters().get(p2.getID()));
//     ASSERT_TRUE(a2.getParameters().get(p1.getID()));
//     ASSERT_TRUE(o.getOwnedElements().get(a1.getID()));
//     ASSERT_TRUE(o.getOwnedElements().get(a2.getID()));
// }

// TEST_F(ParameterTest, reindexNameOwnerTest) {
//     Activity a;
//     Parameter p;
//     a.getParameters().add(p);
//     p.setName("test");
//     ASSERT_TRUE(a.getParameters().get("test"));
//     //ASSERT_TRUE(a.getMembers().get("test"));
//     // ASSERT_TRUE(a.getOwnedElements().get("test"));
// }

// TEST_F(ParameterTest, reindexNameOperationTest) {
//     Activity a1;
//     Activity a2;
//     Parameter p1;
//     Parameter p2;
//     Class c;
//     Operation o;
//     a1.getParameters().add(p1);
//     a1.getParameters().add(p2);
//     a2.getParameters().add(p1);
//     a1.setSpecification(&o);
//     a2.setSpecification(&o);
//     c.getOperations().add(o);
//     p1.setName("test1");
//     p2.setName("test2");
//     ASSERT_TRUE(a1.getParameters().get("test1"));
//     ASSERT_TRUE(a1.getParameters().get("test2"));
//     ASSERT_TRUE(a2.getParameters().get("test1"));
//     // ASSERT_TRUE(o.getOwnedElements().get("test1"));
//     // ASSERT_TRUE(o.getOwnedElements().get("test2"));
// }

TEST_F(ParameterTest, copyParameterTest) {
    UmlManager m;
    Parameter& p = m.create<Parameter>();
    Classifier& t = m.create<Classifier>();
    Operation& o = m.create<Operation>();
    p.setDirection(ParameterDirectionKind::IN);
    p.setType(&t);
    p.setOperation(&o);
    Parameter p2 = p;
    ASSERT_TRUE(p2.getDirection() == ParameterDirectionKind::IN);
    ASSERT_TRUE(p2.getType() == &t);
    ASSERT_TRUE(p2.getOperation() == &o);
}