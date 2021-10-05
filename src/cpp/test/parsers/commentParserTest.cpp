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
    Comment& comment = pckg.getOwnedComments().front();
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

TEST_F(CommentParserTest, mountAndEditCommentTest) {
    UmlManager m;
    Package& root = m.create<Package>();
    Comment& comment = m.create<Comment>();
    root.getOwnedComments().add(comment);
    comment.setBody("foo bar");
    m.setRoot(&root);
    m.mount(ymlPath + "commentTests");

    m.release(comment);
    Comment& comment2 = root.getOwnedComments().front();
    ASSERT_TRUE(comment2.hasOwningElement());
    ASSERT_EQ(comment2.getOwningElementRef(), root);
    ASSERT_TRUE(comment2.hasOwner());
    ASSERT_EQ(comment2.getOwnerRef(), root);
    ASSERT_TRUE(root.getOwnedComments().count(comment2.getID()));
    ASSERT_TRUE(root.getOwnedComments().count(comment2.getID()));

    ID commentID = comment2.getID();
    m.release(comment2, root);
    Comment& comment3 = m.aquire(commentID)->as<Comment>();
    Package& root2 = comment3.getOwningElementRef().as<Package>();
    ASSERT_TRUE(comment3.hasOwner());
    ASSERT_EQ(comment3.getOwnerRef(), root2);
    ASSERT_TRUE(root2.getOwnedComments().count(comment3.getID()));
    ASSERT_TRUE(root2.getOwnedComments().count(comment3.getID()));
}