#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/comment.h"
#include "uml/package.h"

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

TEST_F(CommentParserTest, commentEmitTest) {
    UmlManager m;
    Package& pckg = m.create<Package>();
    Comment& comment = m.create<Comment>();
    pckg.setID("zN&UM2AHrXX07rAiNxTmmMwLYI1O");
    comment.setID("FqaulNq6bCe_8J5M0Ff2oCCaQD05");
    pckg.getOwnedComments().add(comment);
    string expectedEmit = R""""(package:
  id: zN&UM2AHrXX07rAiNxTmmMwLYI1O
  ownedComments:
    - comment:
        id: FqaulNq6bCe_8J5M0Ff2oCCaQD05)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(pckg));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}