#include "gtest/gtest.h"
#include "uml/operation.h"
#include "uml/opaqueBehavior.h"

using namespace UML;

class OperationTest : public ::testing::Test {};

TEST_F(OperationTest, SetAndGetTypeTest) {
    Class c;
    Operation o;
    ASSERT_NO_THROW(o.setType(&c));
    ASSERT_TRUE(o.getType()->getID() == c.getID());
}

TEST_F(OperationTest, reIndexID_Test) {
    Class c;
    Operation o;
    o.setClass(&c);
    o.setID("190d1cb9-13dc-44e6-a064-126891ae0033");
    ASSERT_TRUE(c.getOperations().get(o.getID()) != NULL);
    ASSERT_TRUE(c.getOwnedElements().get(o.getID()) != NULL);
}

TEST_F(OperationTest, reIndexNameTest) {
    Class c;
    Operation o;
    o.setClass(&c);
    o.setName("test");
    ASSERT_TRUE(c.getOperations().get("test") != NULL);
    ASSERT_TRUE(c.getOwnedElements().get("test") != NULL);
}

TEST_F(OperationTest, AddMethodFunctorTest) {
    Operation o;
    Behavior m;
    o.getMethods().add(m);
    ASSERT_TRUE(o.getMethods().size() == 1);
    ASSERT_TRUE(o.getMethods().front() == &m);
    ASSERT_TRUE(m.getSpecification() == & o);
    // ASSERT_TRUE(o.getMembers().size() == 1);
    // ASSERT_TRUE(o.getMembers().front() == &m);
    // ASSERT_TRUE(m.getNamespace() == &o);
    // ASSERT_TRUE(o.getOwnedElements().size() == 1);
    // ASSERT_TRUE(o.getOwnedElements().front() == &m);
    // ASSERT_TRUE(m.getOwner() == &o);
}

TEST_F(OperationTest, SetSpecificationTest) {
    Operation o;
    Behavior m;
    m.setSpecification(&o);
    ASSERT_TRUE(o.getMethods().size() == 1);
    ASSERT_TRUE(o.getMethods().front() == &m);
    ASSERT_TRUE(m.getSpecification() == & o);
    // ASSERT_TRUE(o.getMembers().size() == 1);
    // ASSERT_TRUE(o.getMembers().front() == &m);
    // ASSERT_TRUE(m.getNamespace() == &o);
    // ASSERT_TRUE(o.getOwnedElements().size() == 1);
    // ASSERT_TRUE(o.getOwnedElements().front() == &m);
    // ASSERT_TRUE(m.getOwner() == &o);
}

TEST_F(OperationTest, AddParameterFunctorForAbstractOperationTest) {
    Operation o;
    Parameter p;
    o.getOwnedParameters().add(p);
    ASSERT_TRUE(o.getOwnedParameters().size() == 1);
    ASSERT_TRUE(o.getOwnedParameters().front() == &p);
    ASSERT_TRUE(p.getOperation() == &o);
    ASSERT_TRUE(o.getMembers().size() == 1);
    ASSERT_TRUE(o.getMembers().front() == &p);
    ASSERT_TRUE(p.getNamespace() == &o);
    ASSERT_TRUE(o.getOwnedElements().size() == 1);
    ASSERT_TRUE(o.getOwnedElements().front() == &p);
    ASSERT_TRUE(p.getOwner() == &o);
}

TEST_F(OperationTest, SetOperationAbstractOperationTest) {
    Operation o;
    Parameter p;
    p.setOperation(&o);
    ASSERT_TRUE(o.getOwnedParameters().size() == 1);
    ASSERT_TRUE(o.getOwnedParameters().front() == &p);
    ASSERT_TRUE(p.getOperation() == &o);
    ASSERT_TRUE(o.getMembers().size() == 1);
    ASSERT_TRUE(o.getMembers().front() == &p);
    ASSERT_TRUE(p.getNamespace() == &o);
    ASSERT_TRUE(o.getOwnedElements().size() == 1);
    ASSERT_TRUE(o.getOwnedElements().front() == &p);
    ASSERT_TRUE(p.getOwner() == &o);
}

// TEST_F(OperationTest, AddParameterW_MethodTest) {
//     Operation o;
//     Parameter p;
//     OpaqueBehavior b;
//     o.getMethods().add(b);
//     o.getOwnedParameters().add(p);
//     ASSERT_TRUE(o.getOwnedParameters().size() == 1);
//     ASSERT_TRUE(o.getOwnedParameters().front() == &p);
//     ASSERT_TRUE(p.getOperation() == &o);
//     ASSERT_TRUE(o.getMembers().size() == 1);
//     ASSERT_TRUE(o.getMembers().front() == &p);
//     ASSERT_TRUE(p.getNamespace() == &o);
//     ASSERT_TRUE(o.getOwnedElements().size() == 1);
//     ASSERT_TRUE(o.getOwnedElements().front() == &p);
//     ASSERT_TRUE(p.getOwner() == &o);

//     // TODO thinking about exact implementatin
//     ASSERT_TRUE(b.getSpecification() == &o);
//     ASSERT_TRUE(o.getMethods().size() == 1);
//     ASSERT_TRUE(o.getMethods().front() == &b);
//     ASSERT_TRUE(b.getParameters().size() == 1);
// }

TEST_F(OperationTest, checkParameterFunctorTest) {
    Operation o;
    Parameter p;
    Parameter p2;
    p.setDirection(ParameterDirectionKind::RETURN);
    p2.setDirection(ParameterDirectionKind::RETURN);
    ASSERT_NO_THROW(o.getOwnedParameters().add(p));
    ASSERT_THROW(o.getOwnedParameters().add(p2), ReturnParameterException);
    ASSERT_TRUE(o.getOwnedParameters().size() == 1);
    ASSERT_TRUE(o.getOwnedParameters().front() == &p);
    ASSERT_TRUE(o.getOwnedMembers().size() == 1);
    ASSERT_TRUE(o.getOwnedMembers().front() == &p);
    ASSERT_TRUE(o.getMembers().size() == 1);
    ASSERT_TRUE(o.getMembers().front() == &p);
    ASSERT_TRUE(o.getOwnedElements().size() == 1);
    ASSERT_TRUE(o.getOwnedElements().front() == &p);

    ASSERT_TRUE(p.getOperation() == &o);
    ASSERT_TRUE(p.getNamespace() == &o);
    ASSERT_TRUE(p.getMemberNamespace().size() == 1);
    ASSERT_TRUE(p.getMemberNamespace().front() == &o);
    ASSERT_TRUE(p.getOwner() == &o);

    ASSERT_TRUE(p2.getOperation() == 0);
    ASSERT_TRUE(p2.getNamespace() == 0);
    ASSERT_TRUE(p2.getMemberNamespace().size() == 0);
    ASSERT_TRUE(p2.getOwner() == 0);
}

TEST_F(OperationTest, setOperationReturnParameterExceptionTest) {
    Operation o;
    Parameter p;
    Parameter p2;
    p.setDirection(ParameterDirectionKind::RETURN);
    p2.setDirection(ParameterDirectionKind::RETURN);
    ASSERT_NO_THROW(p.setOperation(&o));
    ASSERT_THROW(p2.setOperation(&o), ReturnParameterException);

    ASSERT_TRUE(o.getOwnedParameters().size() == 1);
    ASSERT_TRUE(o.getOwnedParameters().front() == &p);
    ASSERT_TRUE(o.getOwnedMembers().size() == 1);
    ASSERT_TRUE(o.getOwnedMembers().front() == &p);
    ASSERT_TRUE(o.getMembers().size() == 1);
    ASSERT_TRUE(o.getMembers().front() == &p);
    ASSERT_TRUE(o.getOwnedElements().size() == 1);
    ASSERT_TRUE(o.getOwnedElements().front() == &p);

    ASSERT_TRUE(p.getOperation() == &o);
    ASSERT_TRUE(p.getNamespace() == &o);
    ASSERT_TRUE(p.getMemberNamespace().size() == 1);
    ASSERT_TRUE(p.getMemberNamespace().front() == &o);
    ASSERT_TRUE(p.getOwner() == &o);

    ASSERT_TRUE(p2.getOperation() == 0);
    ASSERT_TRUE(p2.getNamespace() == 0);
    ASSERT_TRUE(p2.getMemberNamespace().size() == 0);
    ASSERT_TRUE(p2.getOwner() == 0);
}