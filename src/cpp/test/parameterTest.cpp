#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"
#include "test/uml-cpp-paths.h"

using namespace UML;

class ParameterTest : public ::testing::Test {
    public:
        std::string ymlPath = YML_FILES_PATH;
};

TEST_F(ParameterTest, TestSetDirection) {
    UmlManager m;
    Parameter& p = *m.create<Parameter>();
    p.setDirection(ParameterDirectionKind::IN_UML);
    ASSERT_TRUE(p.getDirection() == ParameterDirectionKind::IN_UML);
}

TEST_F(ParameterTest, TestGetNotSetDirection) {
    UmlManager m;
    Parameter& p = *m.create<Parameter>();
    ASSERT_TRUE(p.getDirection() == ParameterDirectionKind::NONE);
}

TEST_F(ParameterTest, reindexID_ownerTest) {
    UmlManager m;
    OpaqueBehavior& a = *m.create<OpaqueBehavior>();
    Parameter& p = *m.create<Parameter>();
    a.getOwnedParameters().add(p);
    p.setID("190d1cb9_13dc_44e6_a064_1268");
    ASSERT_NO_THROW(a.getOwnedParameters().get(p.getID()));
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
//     a1.getOwnedParameters().add(p1);
//     a1.getOwnedParameters().add(p2);
//     a2.getOwnedParameters().add(p1);
//     o.getMethods().add(a1);
//     o.getMethods().add(a2);
//     c.getOperations().add(o);
//     p1.setName("190d1cb9-13dc-44e6-a064-126891ae0033");
//     p2.setID("eb092018-0bef-4ad6-b80f-05fa124f98c3");
//     ASSERT_TRUE(a1.getOwnedParameters().get(p1.getID()));
//     ASSERT_TRUE(a1.getOwnedParameters().get(p2.getID()));
//     ASSERT_TRUE(a2.getOwnedParameters().get(p1.getID()));
//     ASSERT_TRUE(o.getOwnedElements().get(a1.getID()));
//     ASSERT_TRUE(o.getOwnedElements().get(a2.getID()));
// }

// TEST_F(ParameterTest, reindexNameOwnerTest) {
//     Activity a;
//     Parameter p;
//     a.getOwnedParameters().add(p);
//     p.setName("test");
//     ASSERT_TRUE(a.getOwnedParameters().get("test"));
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
//     a1.getOwnedParameters().add(p1);
//     a1.getOwnedParameters().add(p2);
//     a2.getOwnedParameters().add(p1);
//     a1.setSpecification(&o);
//     a2.setSpecification(&o);
//     c.getOperations().add(o);
//     p1.setName("test1");
//     p2.setName("test2");
//     ASSERT_TRUE(a1.getOwnedParameters().get("test1"));
//     ASSERT_TRUE(a1.getOwnedParameters().get("test2"));
//     ASSERT_TRUE(a2.getOwnedParameters().get("test1"));
//     // ASSERT_TRUE(o.getOwnedElements().get("test1"));
//     // ASSERT_TRUE(o.getOwnedElements().get("test2"));
// }

TEST_F(ParameterTest, properExceptions) {
    Element* el;
    UmlManager m;
    ASSERT_THROW(el = m.parse(ymlPath + "parameterTests/invalidDirection.yml").ptr(), Parsers::UmlParserException);
    ASSERT_THROW(el = m.parse(ymlPath + "parameterTests/invalidDirection2.yml").ptr(), Parsers::UmlParserException);
}

TEST_F(ParameterTest, emitParameterWMultiplicityTest) {
    UmlManager m;
    OpaqueBehavior& b = *m.create<OpaqueBehavior>();
    Parameter& p = *m.create<Parameter>();
    b.setID("SeJ_0hSPaIa4EYap3sXgRQm4LuSn");
    p.setID("QzBhVkqNSRAZMlkHU6cQ3d_Wm8FT");
    LiteralInt& low = *m.create<LiteralInt>();
    LiteralInt& high = *m.create<LiteralInt>();
    low.setID("K2_2cpL966uyziy9e2pGb60cCR5X");
    low.setValue(0);
    high.setID("sGGXJFNinAvKyKVSgc&JAlMWsbw5");
    high.setValue(5);
    p.setLowerValue(&low);
    p.setUpperValue(&high);
    b.getOwnedParameters().add(p);
    std::string expectedEmit = R""""(opaqueBehavior:
  id: SeJ_0hSPaIa4EYap3sXgRQm4LuSn
  ownedParameters:
    - parameter:
        id: QzBhVkqNSRAZMlkHU6cQ3d_Wm8FT
        lowerValue:
          literalInt:
            id: K2_2cpL966uyziy9e2pGb60cCR5X
            value: 0
        upperValue:
          literalInt:
            id: sGGXJFNinAvKyKVSgc&JAlMWsbw5
            value: 5)"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(b));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}