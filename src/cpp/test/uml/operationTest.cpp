#include "gtest/gtest.h"
#include "uml/operation.h"
#include "uml/opaqueBehavior.h"
#include "uml/parameter.h"

using namespace UML;

class OperationTest : public ::testing::Test {};

TEST_F(OperationTest, SetAndGetTypeTest) {
    UmlManager m;
    Class& c = m.create<Class>();
    Operation& o = m.create<Operation>();
    ASSERT_NO_THROW(o.setType(&c));
    ASSERT_TRUE(o.getType()->getID() == c.getID());
}

TEST_F(OperationTest, reIndexID_Test) {
    UmlManager m;
    Class& c = m.create<Class>();
    Operation& o = m.create<Operation>();
    o.setClass(&c);
    o.setID("c3rcWoyTgxLfFl5jf2Ms6CMa_sWe");
    ASSERT_NO_THROW(c.getOwnedOperations().get(o.getID()));
    ASSERT_NO_THROW(c.getOwnedElements().get(o.getID()));
}

// TEST_F(OperationTest, reIndexNameTest) {
//     Class c;
//     Operation o;
//     o.setClass(&c);
//     o.setName("test");
//     ASSERT_TRUE(c.getOperations().get("test") != NULL);
//     ASSERT_TRUE(c.getOwnedElements().get("test") != NULL);
// }

TEST_F(OperationTest, AddMethodFunctorTest) {
    UmlManager mm;
    Operation& o = mm.create<Operation>();
    OpaqueBehavior& m = mm.create<OpaqueBehavior>();
    o.getMethods().add(m);
    ASSERT_TRUE(o.getMethods().size() == 1);
    ASSERT_TRUE(&o.getMethods().front() == &m);
    ASSERT_TRUE(m.getSpecification() == & o);
    // ASSERT_TRUE(o.getMembers().size() == 1);
    // ASSERT_TRUE(o.getMembers().front() == &m);
    // ASSERT_TRUE(m.getNamespace() == &o);
    // ASSERT_TRUE(o.getOwnedElements().size() == 1);
    // ASSERT_TRUE(o.getOwnedElements().front() == &m);
    // ASSERT_TRUE(m.getOwner() == &o);
}

TEST_F(OperationTest, SetSpecificationTest) {
    UmlManager mm;
    Operation& o = mm.create<Operation>();
    OpaqueBehavior& m = mm.create<OpaqueBehavior>();
    m.setSpecification(&o);
    ASSERT_TRUE(o.getMethods().size() == 1);
    ASSERT_TRUE(&o.getMethods().front() == &m);
    ASSERT_TRUE(m.getSpecification() == & o);
    // ASSERT_TRUE(o.getMembers().size() == 1);
    // ASSERT_TRUE(o.getMembers().front() == &m);
    // ASSERT_TRUE(m.getNamespace() == &o);
    // ASSERT_TRUE(o.getOwnedElements().size() == 1);
    // ASSERT_TRUE(o.getOwnedElements().front() == &m);
    // ASSERT_TRUE(m.getOwner() == &o);
}

TEST_F(OperationTest, AddParameterFunctorForAbstractOperationTest) {
    UmlManager mm;
    Operation& o = mm.create<Operation>();
    Parameter& p = mm.create<Parameter>();
    o.getOwnedParameters().add(p);
    ASSERT_TRUE(o.getOwnedParameters().size() == 1);
    ASSERT_TRUE(&o.getOwnedParameters().front() == &p);
    ASSERT_TRUE(p.getOperation() == &o);
    ASSERT_TRUE(o.getMembers().size() == 1);
    ASSERT_TRUE(&o.getMembers().front() == &p);
    ASSERT_TRUE(p.getNamespace() == &o);
    ASSERT_TRUE(o.getOwnedElements().size() == 1);
    ASSERT_TRUE(&o.getOwnedElements().front() == &p);
    ASSERT_TRUE(p.getOwner() == &o);
}

TEST_F(OperationTest, SetOperationAbstractOperationTest) {
    UmlManager mm;
    Operation& o = mm.create<Operation>();
    Parameter& p = mm.create<Parameter>();
    p.setOperation(&o);
    ASSERT_TRUE(o.getOwnedParameters().size() == 1);
    ASSERT_TRUE(&o.getOwnedParameters().front() == &p);
    ASSERT_TRUE(p.getOperation() == &o);
    ASSERT_TRUE(o.getMembers().size() == 1);
    ASSERT_TRUE(&o.getMembers().front() == &p);
    ASSERT_TRUE(p.getNamespace() == &o);
    ASSERT_TRUE(o.getOwnedElements().size() == 1);
    ASSERT_TRUE(&o.getOwnedElements().front() == &p);
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
//     ASSERT_TRUE(b.getOwnedParameters().size() == 1);
// }

TEST_F(OperationTest, checkParameterFunctorTest) {
    UmlManager mm;
    Operation& o = mm.create<Operation>();
    Parameter& p = mm.create<Parameter>();
    Parameter& p2 = mm.create<Parameter>();
    p.setDirection(ParameterDirectionKind::RETURN);
    p2.setDirection(ParameterDirectionKind::RETURN);
    ASSERT_NO_THROW(o.getOwnedParameters().add(p));
    ASSERT_THROW(o.getOwnedParameters().add(p2), ReturnParameterException);
    ASSERT_TRUE(o.getOwnedParameters().size() == 1);
    ASSERT_TRUE(&o.getOwnedParameters().front() == &p);
    ASSERT_TRUE(o.getOwnedMembers().size() == 1);
    ASSERT_TRUE(&o.getOwnedMembers().front() == &p);
    ASSERT_TRUE(o.getMembers().size() == 1);
    ASSERT_TRUE(&o.getMembers().front() == &p);
    ASSERT_TRUE(o.getOwnedElements().size() == 1);
    ASSERT_TRUE(&o.getOwnedElements().front() == &p);

    ASSERT_TRUE(p.getOperation() == &o);
    ASSERT_TRUE(p.getNamespace() == &o);
    ASSERT_TRUE(p.getMemberNamespace().size() == 1);
    ASSERT_TRUE(&p.getMemberNamespace().front() == &o);
    ASSERT_TRUE(p.getOwner() == &o);

    ASSERT_TRUE(p2.getOperation() == 0);
    ASSERT_TRUE(p2.getNamespace() == 0);
    ASSERT_TRUE(p2.getMemberNamespace().size() == 0);
    ASSERT_TRUE(p2.getOwner() == 0);
}

TEST_F(OperationTest, setOperationReturnParameterExceptionTest) {
    UmlManager mm;
    Operation& o = mm.create<Operation>();
    Parameter& p = mm.create<Parameter>();
    Parameter& p2 = mm.create<Parameter>();
    p.setDirection(ParameterDirectionKind::RETURN);
    p2.setDirection(ParameterDirectionKind::RETURN);
    ASSERT_NO_THROW(p.setOperation(&o));
    ASSERT_THROW(p2.setOperation(&o), ReturnParameterException);

    ASSERT_TRUE(o.getOwnedParameters().size() == 1);
    ASSERT_TRUE(&o.getOwnedParameters().front() == &p);
    ASSERT_TRUE(o.getOwnedMembers().size() == 1);
    ASSERT_TRUE(&o.getOwnedMembers().front() == &p);
    ASSERT_TRUE(o.getMembers().size() == 1);
    ASSERT_TRUE(&o.getMembers().front() == &p);
    ASSERT_TRUE(o.getOwnedElements().size() == 1);
    ASSERT_TRUE(&o.getOwnedElements().front() == &p);

    ASSERT_TRUE(p.getOperation() == &o);
    ASSERT_TRUE(p.getNamespace() == &o);
    ASSERT_TRUE(p.getMemberNamespace().size() == 1);
    ASSERT_TRUE(&p.getMemberNamespace().front() == &o);
    ASSERT_TRUE(p.getOwner() == &o);

    ASSERT_TRUE(p2.getOperation() == 0);
    ASSERT_TRUE(p2.getNamespace() == 0);
    ASSERT_TRUE(p2.getMemberNamespace().size() == 0);
    ASSERT_TRUE(p2.getOwner() == 0);
}

TEST_F(OperationTest, removeOwnedParameterFunctorTest) {
    UmlManager mm;
    Operation& o = mm.create<Operation>();
    Parameter& p = mm.create<Parameter>();
    o.getOwnedParameters().add(p);
    ASSERT_NO_THROW(o.getOwnedParameters().remove(p));
    ASSERT_TRUE(o.getOwnedParameters().size() == 0);
    ASSERT_TRUE(o.getOwnedMembers().size() == 0);
    ASSERT_TRUE(o.getMembers().size() == 0);
    ASSERT_TRUE(o.getOwnedElements().size() == 0);
    
    ASSERT_TRUE(p.getOperation() == 0);
    ASSERT_TRUE(p.getNamespace() == 0);
    ASSERT_TRUE(p.getMemberNamespace().size() == 0);
    ASSERT_TRUE(p.getOwner() == 0);
}

TEST_F(OperationTest, removeOwnedParameterW_NullOperationTest) {
    UmlManager mm;
    Operation& o = mm.create<Operation>();
    Parameter& p = mm.create<Parameter>();
    o.getOwnedParameters().add(p);
    ASSERT_NO_THROW(p.setOperation(0));
    ASSERT_TRUE(o.getOwnedParameters().size() == 0);
    ASSERT_TRUE(o.getOwnedMembers().size() == 0);
    ASSERT_TRUE(o.getMembers().size() == 0);
    ASSERT_TRUE(o.getOwnedElements().size() == 0);
    
    ASSERT_TRUE(p.getOperation() == 0);
    ASSERT_TRUE(p.getNamespace() == 0);
    ASSERT_TRUE(p.getMemberNamespace().size() == 0);
    ASSERT_TRUE(p.getOwner() == 0);
}

TEST_F(OperationTest, overrideParameterOperationTest) {
    UmlManager mm;
    Operation& o = mm.create<Operation>();
    Parameter& p = mm.create<Parameter>();
    Operation& o2 = mm.create<Operation>();
    o.getOwnedParameters().add(p);
    ASSERT_NO_THROW(p.setOperation(&o2));

    ASSERT_TRUE(o.getOwnedParameters().size() == 0);
    ASSERT_TRUE(o.getOwnedMembers().size() == 0);
    ASSERT_TRUE(o.getMembers().size() == 0);
    ASSERT_TRUE(o.getOwnedElements().size() == 0);

    ASSERT_TRUE(o2.getOwnedParameters().size() == 1);
    ASSERT_TRUE(&o2.getOwnedParameters().front() == &p);
    ASSERT_TRUE(o2.getOwnedMembers().size() == 1);
    ASSERT_TRUE(&o2.getOwnedMembers().front() == &p);
    ASSERT_TRUE(o2.getMembers().size() == 1);
    ASSERT_TRUE(&o2.getMembers().front() == &p);
    ASSERT_TRUE(o2.getOwnedElements().size() == 1);
    ASSERT_TRUE(&o2.getOwnedElements().front() == &p);

    ASSERT_TRUE(p.getOperation() == &o2);
    ASSERT_TRUE(p.getNamespace() == &o2);
    ASSERT_TRUE(p.getMemberNamespace().size() == 1);
    ASSERT_TRUE(&p.getMemberNamespace().front() == &o2);
    ASSERT_TRUE(p.getOwner() == &o2);
}

TEST_F(OperationTest, reindexParameterDirectionTest) {
    UmlManager mm;
    Operation& o = mm.create<Operation>();
    Parameter& p = mm.create<Parameter>();
    Parameter& p2 = mm.create<Parameter>();
    o.getOwnedParameters().add(p);
    o.getOwnedParameters().add(p2);
    ASSERT_NO_THROW(p.setDirection(ParameterDirectionKind::RETURN));
    ASSERT_THROW(p2.setDirection(ParameterDirectionKind::RETURN), ReturnParameterException);
    ASSERT_TRUE(o.getOwnedParameters().size() == 2);
    ASSERT_TRUE(&o.getOwnedParameters().front() == &p);
    ASSERT_TRUE(&o.getOwnedParameters().back() == &p2);
    ASSERT_TRUE(o.getOwnedMembers().size() == 2);
    ASSERT_TRUE(&o.getOwnedMembers().front() == &p);
    ASSERT_TRUE(&o.getOwnedMembers().back() == &p2);
    ASSERT_TRUE(o.getMembers().size() == 2);
    ASSERT_TRUE(&o.getMembers().front() == &p);
    ASSERT_TRUE(&o.getMembers().back() == &p2);
    ASSERT_TRUE(o.getOwnedElements().size() == 2);
    ASSERT_TRUE(&o.getOwnedElements().front() == &p);
    ASSERT_TRUE(&o.getOwnedElements().back() == &p2);

    ASSERT_TRUE(p.getDirection() == ParameterDirectionKind::RETURN);
    ASSERT_TRUE(p.getOperation() == &o);
    ASSERT_TRUE(p.getNamespace() == &o);
    ASSERT_TRUE(p.getMemberNamespace().size() == 1);
    ASSERT_TRUE(&p.getMemberNamespace().front() == &o);
    ASSERT_TRUE(p.getOwner() == &o);
    
    ASSERT_TRUE(p2.getDirection() == ParameterDirectionKind::NONE);
    ASSERT_TRUE(p2.getOperation() == &o);
    ASSERT_TRUE(p2.getNamespace() == &o);
    ASSERT_TRUE(p2.getMemberNamespace().size() == 1);
    ASSERT_TRUE(&p2.getMemberNamespace().front() == &o);
    ASSERT_TRUE(p2.getOwner() == &o);
}

TEST_F(OperationTest, RemoveMethodFunctorTest) {
    UmlManager mm;
    Operation& o = mm.create<Operation>();
    OpaqueBehavior& m = mm.create<OpaqueBehavior>();
    o.getMethods().add(m);
    ASSERT_NO_THROW(o.getMethods().remove(m));
    ASSERT_TRUE(o.getMethods().size() == 0);

    ASSERT_TRUE(m.getSpecification() == 0);
}

TEST_F(OperationTest, SetNullSpecificationTest) {
    UmlManager mm;
    Operation& o = mm.create<Operation>();
    OpaqueBehavior& m = mm.create<OpaqueBehavior>();
    o.getMethods().add(m);
    ASSERT_NO_THROW(m.setSpecification(0));
    ASSERT_TRUE(o.getMethods().size() == 0);

    ASSERT_TRUE(m.getSpecification() == 0);
}

TEST_F(OperationTest, OverrideSpecificationTest) {
    UmlManager mm;
    Operation& o = mm.create<Operation>();
    OpaqueBehavior& m = mm.create<OpaqueBehavior>();
    Operation& o2 = mm.create<Operation>();
    m.setSpecification(&o);
    ASSERT_NO_THROW(m.setSpecification(&o2));
    ASSERT_TRUE(o.getMethods().size() == 0);
    ASSERT_TRUE(o2.getMethods().size() == 1);
    ASSERT_TRUE(&o2.getMethods().front() == &m);
    ASSERT_TRUE(m.getSpecification() == &o2);
}