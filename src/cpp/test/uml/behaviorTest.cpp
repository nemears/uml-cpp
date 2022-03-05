#include "gtest/gtest.h"
#include "uml/uml-stable.h"

using namespace UML;

class BehaviorTest : public ::testing::Test {
   
};

TEST_F(BehaviorTest, addParameterFunctorTest) {

    /** TODO: I think this implementation is wrong, double check with uml spec**/

    // Behavior b;
    // Parameter p;
    // Operation o;
    // b.setSpecification(&o);
    // b.getOwnedParameters().add(p);
    // ASSERT_TRUE(b.getOwnedParameters().size() == 1);
    // ASSERT_TRUE(&b.getOwnedParameters().front() == &p);
    // ASSERT_TRUE(b.getOwnedElements().size() == 1);
    // ASSERT_TRUE(&b.getOwnedElements().front() == &p);
    // ASSERT_TRUE(b.getSpecification() == &o);
    // ASSERT_TRUE(p.getOperation() == &o);
}

TEST_F(BehaviorTest, removeParameterFunctorTest) {
    UmlManager m;
    OpaqueBehavior& b = *m.create<OpaqueBehavior>();
    Parameter& p = *m.create<Parameter>();
    Operation& o = *m.create<Operation>();
    b.setSpecification(o);
    b.getOwnedParameters().add(p);
    b.getOwnedParameters().remove(p);
    ASSERT_TRUE(b.getOwnedParameters().size() == 0);
    ASSERT_TRUE(b.getOwnedElements().size() == 0);
    ASSERT_TRUE(!p.getOperation());
    ASSERT_TRUE(!p.getOwner());
}

// TEST_F(BehaviorTest, removeParameterFunctorMultiMethodOpTest) {
//     Behavior b1;
//     Behavior b2;
//     // so this may be false in uml? TODO check
//     // specifically an operation having behaviors with different parameters
//     // i think it is valid, so allow it
//     Parameter p;
//     Operation o;
//     o.getMethods().add(b1);
//     o.getMethods().add(b2);
//     b1.getOwnedParameters().add(p);
//     ASSERT_TRUE(p.getOwner() == &b1);
//     ASSERT_TRUE(b1.getOwnedElements().size() == 1);
//     ASSERT_TRUE(b1.getOwnedElements().front() == &p);
//     ASSERT_TRUE(p.getOperation() == &o);
//     b2.getOwnedParameters().add(p);
//     ASSERT_NO_THROW(b1.getOwnedParameters().remove(p));
//     ASSERT_TRUE(b1.getOwnedParameters().size() == 0);
//     ASSERT_TRUE(b1.getOwnedElements().size() == 0);
//     ASSERT_TRUE(b1.getSpecification() == &o);
//     ASSERT_TRUE(b2.getOwnedParameters().size() == 1);
//     ASSERT_TRUE(b2.getOwnedParameters().front() == &p);
//     ASSERT_TRUE(b2.getOwnedElements().size() == 1);
//     ASSERT_TRUE(b2.getSpecification() == &o);
//     ASSERT_TRUE(o.getMethods().size() == 2);
//     ASSERT_TRUE(o.getMethods().front() == &b1);
//     ASSERT_TRUE(o.getMethods().back() == &b2);
//     ASSERT_TRUE(p.getOperation() == &o);
// }

TEST_F(BehaviorTest, reindexBehaviorID_Test) {
    UmlManager m;
    OpaqueBehavior& behavior = *m.create<OpaqueBehavior>();
    Parameter& param = *m.create<Parameter>();
    Class& owner = *m.create<Class>();
    Operation& specification = *m.create<Operation>();
    behavior.getOwnedParameters().add(param);
    owner.getOwnedBehaviors().add(behavior);
    owner.getOwnedOperations().add(specification);
    specification.getMethods().add(behavior);
    ID id = ID::fromString("paTTKz60cn0lguUMWAMLUNO0EoXS");
    behavior.setID(id);
    ASSERT_EQ(*param.getNamespace(), behavior);
    ASSERT_EQ(*param.getOwner(), behavior);
    ASSERT_EQ(owner.getOwnedBehaviors().get(id), behavior);
    ASSERT_EQ(owner.getOwnedMembers().get(id), behavior);
    ASSERT_EQ(owner.getMembers().get(id), behavior);
    ASSERT_EQ(owner.getOwnedElements().get(id), behavior);
    ASSERT_EQ(specification.getMethods().get(id), behavior);
}