#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/operation.h"
#include "uml/packageMerge.h"
#include "uml/package.h"
#include "uml/class.h"
#include "uml/parameter.h"
#include "uml/primitiveType.h"
#include "uml/opaqueBehavior.h"
#include "test/umlTestUtil.h"

using namespace std;
using namespace UML;

class OperationParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(OperationParserTest, properExceptions) {
    Element* el;
    UmlManager m;
    ASSERT_THROW(el = m.parse(ymlPath + "operationTests/invalidBehavior.yml"), Parsers::UmlParserException);
    ASSERT_THROW(el = m.parse(ymlPath + "operationTests/invalidMethodList.yml"), Parsers::UmlParserException);
}

TEST_F(OperationParserTest, basicParamTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "operationTests/basicParameter.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackageMerge().size() == 1);
    PrimitiveType* b = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().front());
    PrimitiveType* i = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get(1));
    PrimitiveType* r = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get(2));
    PrimitiveType* s = dynamic_cast<PrimitiveType*>(&pckg->getPackageMerge().front().getMergedPackage()->getPackagedElements().get(3));

    ASSERT_TRUE(pckg->getPackagedElements().size() == 1);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(&pckg->getPackagedElements().front());
    ASSERT_TRUE(c->getOwnedOperations().size() == 1);
    Operation* o =&c->getOwnedOperations().front();
    ASSERT_TRUE(o->getOwnedParameters().size() == 1);
    Parameter* p = &o->getOwnedParameters().front();
    ASSERT_TRUE(p->getName().compare("testInt") == 0);
    ASSERT_TRUE(p->getType() == i);
    ASSERT_TRUE(p->getDirection() == ParameterDirectionKind::IN);
}

TEST_F(OperationParserTest, mountAndEditOperationTest) {
    UmlManager m;
    Class& clazz = m.create<Class>();
    OpaqueBehavior& bhv = m.create<OpaqueBehavior>();
    Operation& op = m.create<Operation>();
    Parameter& param = m.create<Parameter>();
    Parameter& bhvParam = m.create<Parameter>();
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
    ASSERT_TRUE(op2.hasClass());
    ASSERT_EQ(op2.getClassRef(), clazz);
    ASSERT_TRUE(op2.hasFeaturingClassifier());
    ASSERT_EQ(op2.getFeaturingClassifierRef(), clazz);
    ASSERT_TRUE(op2.hasNamespace());
    ASSERT_EQ(op2.getNamespaceRef(), clazz);
    ASSERT_TRUE(op2.getMemberNamespace().count(clazzID));
    ASSERT_TRUE(op2.hasOwner());
    ASSERT_EQ(op2.getOwnerRef(), clazz);
    ASSERT_EQ(op2.getMethods().size(), 1);
    ASSERT_EQ(op2.getMethods().front(), bhv);
    ASSERT_TRUE(bhv.hasSpecification());
    ASSERT_EQ(bhv.getSpecificationRef(), op2);
    ASSERT_EQ(op2.getOwnedParameters().size(), 1);
    ASSERT_EQ(op2.getOwnedParameters().front(), param);
    ASSERT_EQ(op2.getOwnedMembers().size(), 1);
    ASSERT_EQ(op2.getOwnedMembers().front(), param);
    ASSERT_EQ(op2.getMembers().size(), 1);
    ASSERT_EQ(op2.getMembers().front(), param);
    ASSERT_EQ(op2.getOwnedElements().size(), 1);
    ASSERT_EQ(op2.getOwnedElements().front(), param);

    ID paramID = param.getID();
    m.release(param);
    Parameter& param2 = m.aquire(paramID)->as<Parameter>();
    ASSERT_TRUE(param2.hasOperation());
    ASSERT_EQ(param2.getOperationRef(), op2);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(param2, op2));

    m.release(param2, op2);
    ASSERT_FALSE(m.loaded(paramID));
    ASSERT_FALSE(m.loaded(opID));
    Operation& op3 = m.aquire(opID)->as<Operation>();
    ASSERT_FALSE(m.loaded(paramID));
    Parameter& param3 = m.aquire(paramID)->as<Parameter>();
    ASSERT_TRUE(param2.hasOperation());
    ASSERT_EQ(param2.getOperationRef(), op3);
    ASSERT_EQ(op3.getOwnedParameters().size(), 1);
    ASSERT_TRUE(op3.getOwnedParameters().count(paramID));
    ASSERT_EQ(op3.getOwnedParameters().front(), param3);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(param2, op3));

    ID bhvID = bhv.getID();
    m.release(bhv);
    OpaqueBehavior& bhv2 = m.aquire(bhvID)->as<OpaqueBehavior>();
    ASSERT_TRUE(bhv2.hasSpecification());
    ASSERT_EQ(bhv2.getSpecificationRef(), op3);
    ASSERT_TRUE(bhv2.hasBehavioredClassifier());
    ASSERT_EQ(bhv2.getBehavioredClassifierRef(), clazz);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(bhv2, clazz));

    m.release(bhv2, op3);
    ASSERT_FALSE(m.loaded(bhvID));
    ASSERT_FALSE(m.loaded(opID));
    OpaqueBehavior& bhv3 = m.aquire(bhvID)->as<OpaqueBehavior>();
    ASSERT_FALSE(m.loaded(opID));
    Operation& op4 = m.aquire(opID)->as<Operation>();
    ASSERT_TRUE(bhv3.hasSpecification());
    ASSERT_EQ(bhv3.getSpecificationRef(), op4);
    ASSERT_TRUE(bhv3.hasBehavioredClassifier());
    ASSERT_EQ(bhv3.getBehavioredClassifierRef(), clazz);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(bhv3, clazz));

    ID bhvParamID = bhvParam.getID();
    m.release(bhvParam);
    Parameter& bhvParam2 = m.aquire(bhvParamID)->as<Parameter>();
    
}