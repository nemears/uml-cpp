#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"
#include "test/uml-cpp-paths.h"

using namespace UML;

UML_SET_INTEGRATION_TEST(ElementOwnedComment, Comment, Class, &Element::getOwnedComments);
UML_SET_INTEGRATION_TEST(CommentAnnotatedElement, Abstraction, Comment, &Comment::getAnnotatedElements);

class CommentTest : public ::testing::Test {
    public:
        std::string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(CommentTest, annotatedElementTest) {
    BasicManager m;
    Package& pckg = *m.create<Package>();
    Package& annotated = *m.create<Package>();
    Comment& comment = *m.create<Comment>();
    comment.setBody("I am a comment!");
    comment.getAnnotatedElements().add(annotated);
    pckg.getPackagedElements().add(annotated);
    pckg.getOwnedComments().add(comment);
    ASSERT_EQ(pckg.getOwnedComments().size(), 1);
    ASSERT_EQ(pckg.getOwnedComments().front(), comment);
    ASSERT_EQ(pckg.getOwnedElements().size(), 2);
    ASSERT_TRUE(comment.getOwner());
    ASSERT_EQ(*comment.getOwner(), pckg);
    ASSERT_EQ(comment.getBody(), "I am a comment!");
}

TEST_F(CommentTest, testBasicComment) {
    BasicManager m;
    ASSERT_NO_THROW(m.open(ymlPath + "commentTests/comment.yml").ptr());
    ASSERT_EQ(m.getRoot()->getElementType(), ElementType::PACKAGE);
    Package& pckg = m.getRoot()->as<Package>();
    ASSERT_EQ(pckg.getOwnedComments().size(), 1);
    Comment& comment = pckg.getOwnedComments().front();
    ASSERT_EQ(comment.getBody(), "i am a comment!");
}

TEST_F(CommentTest, commentEmitTest) {
    BasicManager m;
    Package& pckg = *m.create<Package>();
    Comment& comment = *m.create<Comment>();
    pckg.setID("zN&UM2AHrXX07rAiNxTmmMwLYI1O");
    comment.setID("FqaulNq6bCe_8J5M0Ff2oCCaQD05");
    pckg.getOwnedComments().add(comment);
    std::string expectedEmit = R""""(package:
  id: "zN&UM2AHrXX07rAiNxTmmMwLYI1O"
  ownedComments:
    - comment:
        id: FqaulNq6bCe_8J5M0Ff2oCCaQD05)"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(pckg));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(CommentTest, mountAndEditCommentTest) {
    BasicManager m;
    Package& root = *m.create<Package>();
    Comment& comment = *m.create<Comment>();
    root.getOwnedComments().add(comment);
    comment.setBody("foo bar");
    m.setRoot(&root);
    m.mount(ymlPath + "commentTests");

    m.release(comment);
    Comment& comment2 = root.getOwnedComments().front();
    ASSERT_TRUE(comment2.getOwner());
    ASSERT_EQ(*comment2.getOwner(), root);
    ASSERT_TRUE(root.getOwnedComments().contains(comment2.getID()));
    ASSERT_TRUE(root.getOwnedElements().contains(comment2.getID()));

    ID commentID = comment2.getID();
    m.release(comment2, root);
    Comment& comment3 = m.get(commentID)->as<Comment>();
    Package& root2 = comment3.getOwner()->as<Package>();
    ASSERT_TRUE(comment3.getOwner());
    ASSERT_EQ(*comment3.getOwner(), root2);
    ASSERT_TRUE(root2.getOwnedComments().contains(comment3.getID()));
    ASSERT_TRUE(root2.getOwnedElements().contains(comment3.getID()));
}