#include "gtest/gtest.h"
#include "uml/class.h"
#include "uml/comment.h"

using namespace UML;

class CommentTest : public ::testing::Test {
   
};

TEST_F(CommentTest, addOwnedCommentTest) {
    Class e;
    Comment c;
    c.setBody("I am a pretty cool comment B)");
    e.getOwnedComments().add(c);
    ASSERT_EQ(c.getBody(), "I am a pretty cool comment B)");
    ASSERT_TRUE(e.getOwnedComments().size() == 1);
    ASSERT_TRUE(e.getOwnedComments().front() == &c);
    ASSERT_TRUE(e.getOwnedElements().size() == 1);
    ASSERT_TRUE(e.getOwnedElements().front() == &c);
}