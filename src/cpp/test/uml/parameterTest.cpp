#include "gtest/gtest.h"
#include "uml/parameter.h"
#include "uml/activity.h"
#include "uml/operation.h"

using namespace UML;

class ParameterTest : public ::testing::Test {

    public:
        Parameter* pSetDirection;
        Parameter* pGetSetDirection;
        Parameter* pGetNullDirection;
    
    protected:
        ParameterTest() {
            pSetDirection = new Parameter;
            pGetSetDirection = new Parameter;
            pGetNullDirection= new Parameter;
            pGetSetDirection->setDirection(ParameterDirectionKind::IN);
        }


};

TEST_F(ParameterTest, TestSetDirection) {
    pSetDirection->setDirection(ParameterDirectionKind::IN);
    ASSERT_TRUE(pSetDirection->getDirection() == ParameterDirectionKind::IN);
}

TEST_F(ParameterTest, TestGetSetDirection) {
    ASSERT_TRUE(pGetSetDirection->getDirection() == ParameterDirectionKind::IN);
}

TEST_F(ParameterTest, TestGetNotSetDirection) {
    ASSERT_TRUE(pGetNullDirection->getDirection() == ParameterDirectionKind::NONE);
}

TEST_F(ParameterTest, reindexID_ownerTest) {
    Activity a;
    Parameter p;
    a.getParameters().add(p);
    p.setOwner(&a);
    a.getOwnedElements().add(p);
    p.setNamespace(&a);
    a.getMembers().add(p);
    p.setID("190d1cb9-13dc-44e6-a064-126891ae0033");
    ASSERT_TRUE(a.getParameters().get(p.getID()));
    ASSERT_TRUE(a.getMembers().get(p.getID()));
    ASSERT_TRUE(a.getOwnedElements().get(p.getID()));
}

TEST_F(ParameterTest, reindexID_operationTest) {
    Activity a1;
    Activity a2;
    Parameter p1;
    Parameter p2;
    Class c;
    Operation o;
    a1.getParameters().add(p1);
    a1.getParameters().add(p2);
    a2.getParameters().add(p1);
    a1.setOwner(&o);
    a2.setOwner(&o);
    p1.setOwner(&o);
    p2.setOwner(&o);
    p1.setOperation(&o);
    p2.setOperation(&o);
    o.getMethods().add(a1);
    o.getMethods().add(a2);
    c.getOperations().add(o);
    p1.setName("190d1cb9-13dc-44e6-a064-126891ae0033");
    p2.setID("eb092018-0bef-4ad6-b80f-05fa124f98c3");
    ASSERT_TRUE(a1.getParameters().get(p1.getID()));
    ASSERT_TRUE(a1.getParameters().get(p2.getID()));
    ASSERT_TRUE(a2.getParameters().get(p1.getID()));
    ASSERT_TRUE(o.getOwnedElements().get(p1.getID()));
    ASSERT_TRUE(o.getOwnedElements().get(p2.getID()));
}

TEST_F(ParameterTest, reindexNameOwnerTest) {
    Activity a;
    Parameter p;
    a.getParameters().add(p);
    p.setOwner(&a);
    a.getOwnedElements().add(p);
    p.setNamespace(&a);
    a.getMembers().add(p);
    p.setName("test");
    ASSERT_TRUE(a.getParameters().get("test"));
    ASSERT_TRUE(a.getMembers().get("test"));
    ASSERT_TRUE(a.getOwnedElements().get("test"));
}

TEST_F(ParameterTest, reindexNameOperationTest) {
    Activity a1;
    Activity a2;
    Parameter p1;
    Parameter p2;
    Class c;
    Operation o;
    a1.getParameters().add(p1);
    a1.getParameters().add(p2);
    a2.getParameters().add(p1);
    a1.setOwner(&o);
    a2.setOwner(&o);
    p1.setOwner(&o);
    p2.setOwner(&o);
    o.getOwnedElements().add(a1);
    o.getOwnedElements().add(a2);
    o.getOwnedElements().add(p1);
    o.getOwnedElements().add(p2);
    p1.setOperation(&o);
    p2.setOperation(&o);
    a1.setSpecification(&o);
    a2.setSpecification(&o);
    o.getMethods().add(a1);
    o.getMethods().add(a2);
    c.getOperations().add(o);
    p1.setName("test1");
    p2.setName("test2");
    ASSERT_TRUE(a1.getParameters().get("test1"));
    ASSERT_TRUE(a1.getParameters().get("test2"));
    ASSERT_TRUE(a2.getParameters().get("test1"));
    ASSERT_TRUE(o.getOwnedElements().get("test1"));
    ASSERT_TRUE(o.getOwnedElements().get("test2"));
}

TEST_F(ParameterTest, copyParameterTest) {
    Parameter p;
    Classifier t;
    Operation o;
    p.setDirection(ParameterDirectionKind::IN);
    p.setType(&t);
    p.setOperation(&o);
    Parameter p2 = p;
    ASSERT_TRUE(p2.getDirection() == ParameterDirectionKind::IN);
    ASSERT_TRUE(p2.getType() == &t);
    ASSERT_TRUE(p2.getOperation() == &o);
}