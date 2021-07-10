#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/comment.h"

using namespace std;
using namespace UML;

class CommentParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(CommentParserTest, testBasicComment) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "commentTests/comment.yml"));
    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package& pckg = *dynamic_cast<Package*>(el);
    ASSERT_EQ(pckg.getOwnedComments().size(), 1);
    Comment& comment = *pckg.getOwnedComments().front();
    ASSERT_EQ(comment.getBody(), "i am a comment!");
}