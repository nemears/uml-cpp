#include "gtest/gtest.h"
#include "cpp-parsers/cppParser.h"
#include "uml/class.h"
#include "uml/operation.h"
#include "uml/instanceSpecification.h"

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
    EXPECT_EQ(pckg->getPackagedElements().size(), 2);
    ASSERT_EQ(pckg->getPackagedElements().front()->getElementType(), ElementType::PACKAGE);
    Package& fooNamespace = dynamic_cast<Package&>(*pckg->getPackagedElements().front());
    ASSERT_EQ(fooNamespace.getName(), "FOO");
    ASSERT_EQ(fooNamespace.getAppliedStereotypes().size(), 1);
    ASSERT_EQ(fooNamespace.getAppliedStereotypes().front()->getClassifier()->getName(), "C++ Namespace");
    ASSERT_EQ(pckg->getPackagedElements().get(1)->getElementType(), ElementType::CLASS);
    Class& testClass = dynamic_cast<Class&>(*pckg->getPackagedElements().get(1));
    ASSERT_EQ(testClass.getName(), "test");
    ASSERT_EQ(testClass.getOperations().size(), 1);
    Operation& constructor = *testClass.getOperations().front();
    ASSERT_EQ(constructor.getName(), "test");
}