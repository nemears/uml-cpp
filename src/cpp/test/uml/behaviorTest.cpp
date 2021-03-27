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