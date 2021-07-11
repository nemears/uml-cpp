#include "gtest/gtest.h"
#include "cpp-parsers/cppParser.h"

using namespace std;
using namespace UML;
using namespace CPP;

class CppParserTest : public ::testing::Test {
    public:
    const string includePath = "../../../../../include/";
    const string testPath = "../../../../../src/cpp/test/cppParsers/testHeaders/";
};

TEST_F(CppParserTest, parseBasicHeaderTest) {
    UmlManager m;
    Package* pckg;
    ASSERT_NO_THROW(pckg = parseHeader(testPath + "test.hpp", m));
    ASSERT_TRUE(pckg != 0);
    ASSERT_EQ(pckg->getPackagedElements().size(), 2);
}