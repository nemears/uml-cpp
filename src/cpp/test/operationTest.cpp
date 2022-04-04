#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"
#include "test/uml-cpp-paths.h"

using namespace UML;

UML_SET_INTEGRATION_TEST(OperationOwnedParameter, Parameter, Operation, &Operation::getOwnedParameters);
UML_SINGLETON_INTEGRATION_TEST(ParameterOperation, Operation, Parameter, &Parameter::getOperation, &Parameter::setOperation);
// TODO type and constraints and stuff

class OperationTest : public ::testing::Test {
    public:
        std::string ymlPath = YML_FILES_PATH;
};

TEST_F(OperationTest, SetAndGetTypeTest) {
    UmlManager m;
    Class& c = *m.create<Class>();
    Operation& o = *m.create<Operation>();
    ASSERT_NO_THROW(o.setType(&c));
    ASSERT_TRUE(o.getType()->getID() == c.getID());
}

TEST_F(OperationTest, reIndexID_Test) {
    UmlManager m;
    Class& c = *m.create<Class>();
    Operation& o = *m.create<Operation>();
    o.setClass(&c);
    o.setID("c3rcWoyTgxLfFl5jf2Ms6CMa_sWe");
    ASSERT_NO_THROW(c.getOwnedOperations().get(o.getID()));
    ASSERT_NO_THROW(c.getOwnedElements().get(o.getID()));
}

TEST_F(OperationTest, reIndexNameTest) {
    UmlManager m;
    ClassPtr c = m.create<Class>();
    OperationPtr o = m.create<Operation>();
    o->setClass(*c);
    o->setName("test");
    ASSERT_EQ(c->getOwnedOperations().get("test"), *o);
    ASSERT_EQ(c->getOwnedElements().get("test"), *o);
}

TEST_F(OperationTest, AddMethodFunctorTest) {
    UmlManager mm;
    Operation& o = *mm.create<Operation>();
    OpaqueBehavior& m = *mm.create<OpaqueBehavior>();
    o.getMethods().add(m);
    ASSERT_TRUE(o.getMethods().size() == 1);
    ASSERT_TRUE(&o.getMethods().front() == &m);
    ASSERT_TRUE(m.getSpecification() == &o);
    // ASSERT_TRUE(o.getMembers().size() == 1);
    // ASSERT_TRUE(o.getMembers().front() == &m);
    // ASSERT_TRUE(m.getNamespace() == &o);
    // ASSERT_TRUE(o.getOwnedElements().size() == 1);
    // ASSERT_TRUE(o.getOwnedElements().front() == &m);
    // ASSERT_TRUE(m.getOwner() == &o);
}

TEST_F(OperationTest, SetSpecificationTest) {
    UmlManager mm;
    Operation& o = *mm.create<Operation>();
    OpaqueBehavior& m = *mm.create<OpaqueBehavior>();
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
    Operation& o = *mm.create<Operation>();
    Parameter& p = *mm.create<Parameter>();
    o.getOwnedParameters().add(p);
    ASSERT_TRUE(o.getOwnedParameters().size() == 1);
    ASSERT_TRUE(&o.getOwnedParameters().front() == &p);
    ASSERT_TRUE(p.getOperation() == &o);
    ASSERT_TRUE(o.getMembers().size() == 1);
    ASSERT_TRUE(&o.getMembers().front() == &p);
    ASSERT_TRUE(p.getNamespace() == &o);
    ASSERT_TRUE(o.getOwnedElements().size() == 1);
    ASSERT_TRUE(&o.getOwnedElements().get(p.getID()) == &p);
    ASSERT_TRUE(p.getOwner() == &o);
}

TEST_F(OperationTest, SetOperationAbstractOperationTest) {
    UmlManager mm;
    Operation& o = *mm.create<Operation>();
    Parameter& p = *mm.create<Parameter>();
    p.setOperation(o);
    ASSERT_TRUE(o.getOwnedParameters().size() == 1);
    ASSERT_TRUE(&o.getOwnedParameters().front() == &p);
    ASSERT_TRUE(p.getOperation() == &o);
    ASSERT_TRUE(o.getMembers().size() == 1);
    ASSERT_TRUE(&o.getMembers().front() == &p);
    ASSERT_TRUE(p.getNamespace() == &o);
    ASSERT_TRUE(o.getOwnedElements().size() == 1);
    ASSERT_TRUE(&o.getOwnedElements().get(p.getID()) == &p);
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

// TEST_F(OperationTest, checkParameterFunctorTest) {
//     UmlManager mm;
//     Operation& o = mm.create<Operation>();
//     Parameter& p = mm.create<Parameter>();
//     Parameter& p2 = mm.create<Parameter>();
//     p.setDirection(ParameterDirectionKind::RETURN);
//     p2.setDirection(ParameterDirectionKind::RETURN);
//     ASSERT_NO_THROW(o.getOwnedParameters().add(p));
//     ASSERT_THROW(o.getOwnedParameters().add(p2), ReturnParameterException);
//     ASSERT_TRUE(o.getOwnedParameters().size() == 1);
//     ASSERT_TRUE(&o.getOwnedParameters().front() == &p);
//     ASSERT_TRUE(o.getOwnedMembers().size() == 1);
//     ASSERT_TRUE(&o.getOwnedMembers().front() == &p);
//     ASSERT_TRUE(o.getMembers().size() == 1);
//     ASSERT_TRUE(&o.getMembers().front() == &p);
//     ASSERT_TRUE(o.getOwnedElements().size() == 1);
//     ASSERT_TRUE(&o.getOwnedElements().get(p.getID()) == &p);

//     ASSERT_TRUE(p.getOperation() == &o);
//     ASSERT_TRUE(p.getNamespace() == &o);
//     ASSERT_TRUE(p.getOwner() == &o);

//     ASSERT_TRUE(p2.getOperation() == 0);
//     ASSERT_TRUE(p2.getNamespace() == 0);
//     ASSERT_TRUE(p2.getOwner() == 0);
// }

// TEST_F(OperationTest, setOperationReturnParameterExceptionTest) {
//     UmlManager mm;
//     Operation& o = mm.create<Operation>();
//     Parameter& p = mm.create<Parameter>();
//     Parameter& p2 = mm.create<Parameter>();
//     p.setDirection(ParameterDirectionKind::RETURN);
//     p2.setDirection(ParameterDirectionKind::RETURN);
//     ASSERT_NO_THROW(p.setOperation(&o));
//     ASSERT_THROW(p2.setOperation(&o), ReturnParameterException);

//     ASSERT_TRUE(o.getOwnedParameters().size() == 1);
//     ASSERT_TRUE(&o.getOwnedParameters().front() == &p);
//     ASSERT_TRUE(o.getOwnedMembers().size() == 1);
//     ASSERT_TRUE(&o.getOwnedMembers().front() == &p);
//     ASSERT_TRUE(o.getMembers().size() == 1);
//     ASSERT_TRUE(&o.getMembers().front() == &p);
//     ASSERT_TRUE(o.getOwnedElements().size() == 1);
//     ASSERT_TRUE(&o.getOwnedElements().get(p.getID()) == &p);

//     ASSERT_TRUE(p.getOperation() == &o);
//     ASSERT_TRUE(p.getNamespace() == &o);
//     ASSERT_TRUE(p.getOwner() == &o);

//     ASSERT_TRUE(p2.getOperation() == 0);
//     ASSERT_TRUE(p2.getNamespace() == 0);
//     ASSERT_TRUE(p2.getOwner() == 0);
// }

TEST_F(OperationTest, removeOwnedParameterFunctorTest) {
    UmlManager mm;
    Operation& o = *mm.create<Operation>();
    Parameter& p = *mm.create<Parameter>();
    o.getOwnedParameters().add(p);
    ASSERT_NO_THROW(o.getOwnedParameters().remove(p));
    ASSERT_TRUE(o.getOwnedParameters().size() == 0);
    ASSERT_TRUE(o.getOwnedMembers().size() == 0);
    ASSERT_TRUE(o.getMembers().size() == 0);
    ASSERT_TRUE(o.getOwnedElements().size() == 0);
    
    ASSERT_FALSE(p.getOperation());
    ASSERT_FALSE(p.getNamespace());
    ASSERT_FALSE(p.getOwner());
}

TEST_F(OperationTest, removeOwnedParameterW_NullOperationTest) {
    UmlManager mm;
    Operation& o = *mm.create<Operation>();
    Parameter& p = *mm.create<Parameter>();
    o.getOwnedParameters().add(p);
    ASSERT_NO_THROW(p.setOperation(0));
    ASSERT_TRUE(o.getOwnedParameters().size() == 0);
    ASSERT_TRUE(o.getOwnedMembers().size() == 0);
    ASSERT_TRUE(o.getMembers().size() == 0);
    ASSERT_TRUE(o.getOwnedElements().size() == 0);
    
    ASSERT_FALSE(p.getOperation());
    ASSERT_FALSE(p.getNamespace());
    ASSERT_FALSE(p.getOwner());
}

TEST_F(OperationTest, overrideParameterOperationTest) {
    UmlManager mm;
    Operation& o = *mm.create<Operation>();
    Parameter& p = *mm.create<Parameter>();
    Operation& o2 = *mm.create<Operation>();
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
    ASSERT_TRUE(&o2.getOwnedElements().get(p.getID()) == &p);

    ASSERT_TRUE(*p.getOperation() == o2);
    ASSERT_TRUE(*p.getNamespace() == o2);
    ASSERT_TRUE(*p.getOwner() == o2);
}

TEST_F(OperationTest, reindexParameterDirectionTest) {
    UmlManager mm;
    Operation& o = *mm.create<Operation>();
    Parameter& p = *mm.create<Parameter>();
    Parameter& p2 = *mm.create<Parameter>();
    o.getOwnedParameters().add(p);
    o.getOwnedParameters().add(p2);
    ASSERT_NO_THROW(p.setDirection(ParameterDirectionKind::RETURN));
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
    ASSERT_TRUE(&o.getOwnedElements().get(p.getID()) == &p);
    ASSERT_TRUE(&o.getOwnedElements().get(p2.getID()) == &p2);

    ASSERT_TRUE(p.getDirection() == ParameterDirectionKind::RETURN);
    ASSERT_TRUE(p.getOperation() == &o);
    ASSERT_TRUE(p.getNamespace() == &o);
    ASSERT_TRUE(p.getOwner() == &o);
    
    ASSERT_TRUE(p2.getDirection() == ParameterDirectionKind::NONE);
    ASSERT_TRUE(*p2.getOperation() == o);
    ASSERT_TRUE(*p2.getNamespace() == o);
    ASSERT_TRUE(*p2.getOwner() == o);
}

TEST_F(OperationTest, RemoveMethodFunctorTest) {
    UmlManager mm;
    Operation& o = *mm.create<Operation>();
    OpaqueBehavior& m = *mm.create<OpaqueBehavior>();
    o.getMethods().add(m);
    ASSERT_NO_THROW(o.getMethods().remove(m));
    ASSERT_TRUE(o.getMethods().size() == 0);

    ASSERT_FALSE(m.getSpecification());
}

TEST_F(OperationTest, SetNullSpecificationTest) {
    UmlManager mm;
    Operation& o = *mm.create<Operation>();
    OpaqueBehavior& m = *mm.create<OpaqueBehavior>();
    o.getMethods().add(m);
    ASSERT_NO_THROW(m.setSpecification(0));
    ASSERT_TRUE(o.getMethods().size() == 0);

    ASSERT_FALSE(m.getSpecification());
}

TEST_F(OperationTest, OverrideSpecificationTest) {
    UmlManager mm;
    Operation& o = *mm.create<Operation>();
    OpaqueBehavior& m = *mm.create<OpaqueBehavior>();
    Operation& o2 = *mm.create<Operation>();
    m.setSpecification(o);
    ASSERT_NO_THROW(m.setSpecification(&o2));
    ASSERT_TRUE(o.getMethods().size() == 0);
    ASSERT_TRUE(o2.getMethods().size() == 1);
    ASSERT_TRUE(&o2.getMethods().front() == &m);
    ASSERT_TRUE(m.getSpecification() == &o2);
}

TEST_F(OperationTest, properExceptions) {
    Element* el;
    UmlManager m;
    ASSERT_THROW(el = m.parse(ymlPath + "operationTests/invalidBehavior.yml").ptr(), Parsers::UmlParserException);
    ASSERT_THROW(el = m.parse(ymlPath + "operationTests/invalidMethodList.yml").ptr(), Parsers::UmlParserException);
}

TEST_F(OperationTest, basicParamTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "operationTests/basicParameter.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackageMerge().size() == 1);
    PrimitiveTypePtr b = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get(ID::fromString("bool_bzkcabSy3CiFd&HmJOtnVRK")));
    PrimitiveTypePtr i = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get(ID::fromString("int_r9nNbBukx47IomXrT2raqtc4")));
    PrimitiveTypePtr r = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get(ID::fromString("real_aZG&w6yl61bXVWutgeyScN9")));
    PrimitiveTypePtr s = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get(ID::fromString("string_L&R5eAEq6f3LUNtUmzHzT")));

    ASSERT_TRUE(pckg->getPackagedElements().size() == 1);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(&pckg->getPackagedElements().front());
    ASSERT_TRUE(c->getOwnedOperations().size() == 1);
    Operation* o =&c->getOwnedOperations().front();
    ASSERT_TRUE(o->getOwnedParameters().size() == 1);
    Parameter* p = &o->getOwnedParameters().front();
    ASSERT_TRUE(p->getName().compare("testInt") == 0);
    ASSERT_TRUE(p->getType() == i);
    ASSERT_TRUE(p->getDirection() == ParameterDirectionKind::IN_UML);
}

TEST_F(OperationTest, mountAndEditOperationTest) {
    UmlManager m;
    Class& clazz = *m.create<Class>();
    OpaqueBehavior& bhv = *m.create<OpaqueBehavior>();
    Operation& op = *m.create<Operation>();
    Parameter& param = *m.create<Parameter>();
    Parameter& bhvParam = *m.create<Parameter>();
    op.getOwnedParameters().add(param);
    bhv.getOwnedParameters().add(bhvParam);
    op.getMethods().add(bhv);
    clazz.getOwnedBehaviors().add(bhv);
    clazz.getOwnedOperations().add(op);
    m.setRoot(&clazz);
    m.mount(ymlPath + "operationTests");

    ID opID = op.getID();
    ID clazzID = clazz.getID();
    m.release(op);
    Operation& op2 = m.aquire(opID)->as<Operation>();
    ASSERT_TRUE(op2.getClass());
    ASSERT_EQ(*op2.getClass(), clazz);
    ASSERT_TRUE(op2.getFeaturingClassifier());
    ASSERT_EQ(*op2.getFeaturingClassifier(), clazz);
    ASSERT_TRUE(op2.getNamespace());
    ASSERT_EQ(*op2.getNamespace(), clazz);
    ASSERT_TRUE(op2.getOwner());
    ASSERT_EQ(*op2.getOwner(), clazz);
    ASSERT_EQ(op2.getMethods().size(), 1);
    ASSERT_EQ(op2.getMethods().front(), bhv);
    ASSERT_TRUE(bhv.getSpecification());
    ASSERT_EQ(*bhv.getSpecification(), op2);
    ASSERT_EQ(op2.getOwnedParameters().size(), 1);
    ASSERT_EQ(op2.getOwnedParameters().front(), param);
    ASSERT_EQ(op2.getOwnedMembers().size(), 1);
    ASSERT_EQ(op2.getOwnedMembers().front(), param);
    ASSERT_EQ(op2.getMembers().size(), 1);
    ASSERT_EQ(op2.getMembers().front(), param);
    ASSERT_EQ(op2.getOwnedElements().size(), 1);
    ASSERT_EQ(*op2.getOwnedElements().begin(), param);

    ID paramID = param.getID();
    m.release(param);
    Parameter& param2 = m.aquire(paramID)->as<Parameter>();
    ASSERT_TRUE(param2.getOperation());
    ASSERT_EQ(*param2.getOperation(), op2);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(param2, op2));

    m.release(param2, op2);
    ASSERT_FALSE(m.loaded(paramID));
    ASSERT_FALSE(m.loaded(opID));
    Operation& op3 = m.aquire(opID)->as<Operation>();
    ASSERT_FALSE(m.loaded(paramID));
    Parameter& param3 = m.aquire(paramID)->as<Parameter>();
    ASSERT_TRUE(param3.getOperation());
    ASSERT_EQ(*param3.getOperation(), op3);
    ASSERT_EQ(op3.getOwnedParameters().size(), 1);
    ASSERT_TRUE(op3.getOwnedParameters().count(paramID));
    ASSERT_EQ(op3.getOwnedParameters().front(), param3);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(param3, op3));

    ID bhvID = bhv.getID();
    m.release(bhv);
    OpaqueBehavior& bhv2 = m.aquire(bhvID)->as<OpaqueBehavior>();
    ASSERT_TRUE(bhv2.getSpecification());
    ASSERT_EQ(*bhv2.getSpecification(), op3);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(bhv2, clazz));

    m.release(bhv2, op3);
    ASSERT_FALSE(m.loaded(bhvID));
    ASSERT_FALSE(m.loaded(opID));
    OpaqueBehavior& bhv3 = m.aquire(bhvID)->as<OpaqueBehavior>();
    ASSERT_FALSE(m.loaded(opID));
    Operation& op4 = m.aquire(opID)->as<Operation>();
    ASSERT_TRUE(bhv3.getSpecification());
    ASSERT_EQ(*bhv3.getSpecification(), op4);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(bhv3, clazz));

    ID bhvParamID = bhvParam.getID();
    m.release(bhvParam);
    Parameter& bhvParam2 = m.aquire(bhvParamID)->as<Parameter>();
    // ASSERT_TRUE(bhvParam2.hasBehavior());
    // ASSERT_EQ(bhvParam2.getBehaviorRef(), bhv3);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(bhvParam2, bhv3));

    m.release(bhvParam2, bhv3);
    ASSERT_FALSE(m.loaded(bhvID));
    ASSERT_FALSE(m.loaded(bhvParamID));
    Behavior& bhv4 = m.aquire(bhvID)->as<Behavior>();
    ASSERT_FALSE(m.loaded(bhvParamID));
    Parameter& bhvParam3 = m.aquire(bhvParamID)->as<Parameter>();
    // ASSERT_TRUE(bhvParam3.hasBehavior());
    // ASSERT_EQ(bhvParam3.getBehaviorRef(), bhv4);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(bhvParam3, bhv4));
}