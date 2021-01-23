#include "gtest/gtest.h"
#include "../../../cpp/uml/headers/class.h"
#include "../../../cpp/uml/headers/operation.h"
#include "../../../cpp/uml/headers/opaqueBehavior.h"
#include "../../../cpp/uml/headers/parameter.h"

using namespace UML;

class ClassTest : public ::testing::Test {
   
};

TEST_F(ClassTest, BasicOperationTest) {
    Class c;
    Operation o;
    OpaqueBehavior oB;
    Parameter p;
    ASSERT_NO_THROW(o.parameters.push_back(&p));
    ASSERT_NO_THROW(oB.bodies.push_back("return true"));
    ASSERT_NO_THROW(o.methods.push_back(&oB));
    ASSERT_NO_THROW(c.operations.push_back(&o));
}