#include "gtest/gtest.h"
#include "uml/behavior.h"
#include "uml/operation.h"

using namespace UML;

class BehaviorTest : public ::testing::Test {
   
};

TEST_F(BehaviorTest, addParameterFunctorTest) {
    Behavior b;
    Parameter p;
    Operation o;
    b.setSpecification(&o);
    b.getParameters().add(p);
    ASSERT_TRUE(b.getParameters().size() == 1);
    ASSERT_TRUE(b.getParameters().front() == &p);
    ASSERT_TRUE(b.getOwnedElements().size() == 1);
    ASSERT_TRUE(b.getOwnedElements().front() == &p);
    ASSERT_TRUE(b.getSpecification() == &o);
    ASSERT_TRUE(p.getOperation() == &o);
}

TEST_F(BehaviorTest, removeParameterFunctorTest) {
    Behavior b;
    Parameter p;
    Operation o;
    b.setSpecification(&o);
    b.getParameters().add(p);
    b.getParameters().remove(p);
    ASSERT_TRUE(b.getParameters().size() == 0);
    ASSERT_TRUE(b.getOwnedElements().size() == 0);
    ASSERT_TRUE(!p.getOperation());
    ASSERT_TRUE(!p.getOwner());
}

TEST_F(BehaviorTest, removeParameterFunctorMultiMethodOpTest) {
    Behavior b1;
    Behavior b2;
    // so this may be false in uml? TODO check
    // specifically an operation having behaviors with different parameters
    // i think it is valid, so allow it
    Parameter p;
    Operation o;
    o.getMethods().add(b1);
    o.getMethods().add(b2);
    b1.getParameters().add(p);
    ASSERT_TRUE(p.getOwner() == &b1);
    ASSERT_TRUE(b1.getOwnedElements().size() == 1);
    ASSERT_TRUE(b1.getOwnedElements().front() == &p);
    ASSERT_TRUE(p.getOperation() == &o);
    b2.getParameters().add(p);
    ASSERT_NO_THROW(b1.getParameters().remove(p));
    ASSERT_TRUE(b1.getParameters().size() == 0);
    ASSERT_TRUE(b1.getOwnedElements().size() == 0);
    ASSERT_TRUE(b1.getSpecification() == &o);
    ASSERT_TRUE(b2.getParameters().size() == 1);
    ASSERT_TRUE(b2.getParameters().front() == &p);
    ASSERT_TRUE(b2.getOwnedElements().size() == 1);
    ASSERT_TRUE(b2.getSpecification() == &o);
    ASSERT_TRUE(o.getMethods().size() == 2);
    ASSERT_TRUE(o.getMethods().front() == &b1);
    ASSERT_TRUE(o.getMethods().back() == &b2);
    ASSERT_TRUE(p.getOperation() == &o);
}