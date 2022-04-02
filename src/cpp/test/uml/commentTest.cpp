#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

using namespace UML;

UML_SET_INTEGRATION_TEST(ElementOwnedComment, Comment, DataStoreNode, &Element::getOwnedComments);
UML_SET_INTEGRATION_TEST(CommentAnnotatedElement, Abstraction, Comment, &Comment::getAnnotatedElements);

class CommentTest : public ::testing::Test {};

TEST_F(CommentTest, annotatedElementTest) {
    UmlManager m;
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

