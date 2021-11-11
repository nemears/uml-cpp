#include "gtest/gtest.h"
#include "uml/class.h"
#include "uml/comment.h"

using namespace UML;

class CommentTest : public ::testing::Test {
   
};

TEST_F(CommentTest, addOwnedCommentTest) {
    UmlManager m;
    Class& e = m.create<Class>();
    Comment& c = m.create<Comment>();
    c.setBody("I am a pretty cool comment B)");
    e.getOwnedComments().add(c);
    ASSERT_EQ(c.getBody(), "I am a pretty cool comment B)");
    ASSERT_TRUE(e.getOwnedComments().size() == 1);
    ASSERT_TRUE(&e.getOwnedComments().front() == &c);
    ASSERT_TRUE(e.getOwnedElements().size() == 1);
    ASSERT_TRUE(*e.getOwnedElements().begin() == c);
    ASSERT_TRUE(c.getOwningElement() == &e);
}

TEST_F(CommentTest, removeOwnedCommentTest) {
    UmlManager m;
    Class& e = m.create<Class>();
    Comment& c = m.create<Comment>();
    c.setBody("p cool comment");
    e.getOwnedComments().add(c);
    ASSERT_NO_THROW(e.getOwnedComments().remove(c));
    ASSERT_TRUE(e.getOwnedComments().size() == 0);
    ASSERT_TRUE(e.getOwnedElements().size() == 0);
    ASSERT_TRUE(c.getOwningElement() == 0);
}

TEST_F(CommentTest, overideOwningElement) {
    UmlManager m;
    Class& e = m.create<Class>();
    Comment& c = m.create<Comment>();
    c.setOwningElement(&e);
    ASSERT_TRUE(e.getOwnedComments().size() == 1);
    ASSERT_TRUE(&e.getOwnedComments().front() == &c);
    ASSERT_TRUE(e.getOwnedElements().size() == 1);
    ASSERT_TRUE(*e.getOwnedElements().begin() == c);
}