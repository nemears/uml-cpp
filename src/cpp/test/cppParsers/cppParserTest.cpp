#include "gtest/gtest.h"
#include "uml/cpp/cppParser.h"
#include "uml/class.h"
#include "uml/operation.h"
#include "uml/instanceSpecification.h"
#include "uml/literalInt.h"
#include "uml/literalReal.h"

using namespace std;
using namespace UML;
using namespace CPP;

class CppParserTest : public ::testing::Test {
    public:
    const string profilePath = "../../../../../src/yml/uml/cpp/";
    const string includePath = "../../../../../include/";
    const string testPath = "../../../../../src/cpp/test/cppParsers/testHeaders/";
};

TEST_F(CppParserTest, parseBasicHeaderTest) {
    UmlManager m;
    // add cpp profile to memory
    m.parse(profilePath + "cppProfile.yml");
    Package* pckg;
    ASSERT_NO_THROW(pckg = parseHeader(testPath + "test.hpp", m));
    ASSERT_TRUE(pckg != 0);
    ASSERT_EQ(pckg->getName(), "test.hpp");
    EXPECT_EQ(pckg->getPackagedElements().size(), 3);
    ASSERT_EQ(pckg->getPackagedElements().front()->getElementType(), ElementType::ARTIFACT);
    ASSERT_EQ(pckg->getPackagedElements().front()->getName(), "test.hpp");
    ASSERT_EQ(pckg->getPackagedElements().get(1)->getElementType(), ElementType::PACKAGE);
    Package& fooNamespace = dynamic_cast<Package&>(*pckg->getPackagedElements().get(1));
    ASSERT_EQ(fooNamespace.getName(), "FOO");
    ASSERT_EQ(fooNamespace.getAppliedStereotypes().size(), 1);
    ASSERT_EQ(fooNamespace.getAppliedStereotypes().front()->getClassifier()->getName(), "C++ Namespace");
    ASSERT_EQ(fooNamespace.getPackagedElements().size(), 4);
    ASSERT_EQ(fooNamespace.getPackagedElements().front()->getName(), "c");
    ASSERT_EQ(fooNamespace.getPackagedElements().front()->getElementType(), ElementType::LITERAL_INT);
    LiteralInt& cChar = dynamic_cast<LiteralInt&>(*fooNamespace.getPackagedElements().front());
    ASSERT_EQ(cChar.getType()->getID(), ID::fromString("C_char_bvN6xdQ&&LaR7MU_F_9uR"));
    ASSERT_EQ(fooNamespace.getPackagedElements().get(1)->getElementType(), ElementType::LITERAL_INT);
    LiteralInt& cInt = dynamic_cast<LiteralInt&>(*fooNamespace.getPackagedElements().get(1));
    ASSERT_EQ(cInt.getName(), "i");
    ASSERT_EQ(cInt.getType()->getID(), ID::fromString("C_int_ZvgWKuxGtKtjRQPMNTXjic"));
    ASSERT_EQ(fooNamespace.getPackagedElements().get(2)->getElementType(), ElementType::LITERAL_REAL);
    LiteralReal& cFloat = dynamic_cast<LiteralReal&>(*fooNamespace.getPackagedElements().get(2));
    ASSERT_EQ(cFloat.getName(), "f");
    ASSERT_EQ(cFloat.getType()->getID(), ID::fromString("C_float_FRQyo8d1KEQQLOnnPPn6"));
    ASSERT_EQ(fooNamespace.getPackagedElements().get(3)->getElementType(), ElementType::LITERAL_REAL);
    LiteralReal& cDouble = dynamic_cast<LiteralReal&>(*fooNamespace.getPackagedElements().get(3));
    ASSERT_EQ(cDouble.getName(), "d");
    ASSERT_EQ(cDouble.getType()->getID(), ID::fromString("C_double_HM2asoTiFmoWEK8ZuAE"));
    ASSERT_EQ(pckg->getPackagedElements().get(2)->getElementType(), ElementType::CLASS);
    Class& testClass = dynamic_cast<Class&>(*pckg->getPackagedElements().get(2));
    ASSERT_EQ(testClass.getName(), "test");
    ASSERT_EQ(testClass.getOperations().size(), 1);
    Operation& constructor = *testClass.getOperations().front();
    ASSERT_EQ(constructor.getName(), "test");
}