#include "gtest/gtest.h"
#include "uml/class.h"
#include "uml/operation.h"
#include "uml/primitiveType.h"

using namespace UML;

class TemplateableElementTest : public ::testing::Test {
   
};

TEST_F(TemplateableElementTest, setOwnedTemplateSignature) {
    UmlManager m;
    Class& c = m.create<Class>();
    TemplateSignature& t = m.create<TemplateSignature>();
    ASSERT_NO_THROW(c.setOwnedTemplateSignature(&t));
    ASSERT_EQ(t.getTemplate(), &c);
    ASSERT_TRUE(c.getOwnedElements().size() == 1);
    ASSERT_TRUE(c.getOwnedElements().front() == &t);
    ASSERT_TRUE(t.getOwner() == &c);
}

TEST_F(TemplateableElementTest, overwriteOwnedTemplateSignatureTest) {
    UmlManager m;
    Class& c = m.create<Class>();
    TemplateSignature& t = m.create<TemplateSignature>();
    TemplateSignature& t2 = m.create<TemplateSignature>();
    c.setOwnedTemplateSignature(&t);
    ASSERT_NO_THROW(c.setOwnedTemplateSignature(&t2));
    ASSERT_EQ(t2.getTemplate(), &c);
    ASSERT_TRUE(t.getTemplate() == 0);
    ASSERT_TRUE(c.getOwnedElements().size() == 1);
    ASSERT_TRUE(c.getOwnedElements().front() == &t2);
    ASSERT_TRUE(t2.getOwner() == &c);
    ASSERT_TRUE(t.getOwner() == 0);
}

TEST_F(TemplateableElementTest, setTemplateTest) {
    UmlManager m;
    Class& c = m.create<Class>();
    TemplateSignature& t = m.create<TemplateSignature>();
    ASSERT_NO_THROW(t.setTemplate(&c));
    ASSERT_EQ(t.getTemplate(), &c);
    ASSERT_TRUE(c.getOwnedElements().size() == 1);
    ASSERT_TRUE(c.getOwnedElements().front() == &t);
    ASSERT_TRUE(t.getOwner() == &c);
}

TEST_F(TemplateableElementTest, overrideTemplateTest) {
    UmlManager m;
    Class& c = m.create<Class>();
    Class& c2 = m.create<Class>();
    TemplateSignature& t = m.create<TemplateSignature>();
    t.setTemplate(&c);
    ASSERT_NO_THROW(t.setTemplate(&c2));
    ASSERT_EQ(t.getTemplate()->getID(), c2.getID());
    ASSERT_EQ(c2.getOwnedTemplateSignature()->getID(), t.getID());
    ASSERT_TRUE(c.getOwnedTemplateSignature() == 0);
    ASSERT_TRUE(c.getOwnedElements().size() == 0);
    ASSERT_TRUE(c2.getOwnedElements().size() == 1);
    ASSERT_EQ(c2.getOwnedElements().front()->getID(), t.getID());
    ASSERT_EQ(t.getOwner()->getID(), c2.getID());
}

TEST_F(TemplateableElementTest, addTemplateParameterTest) {
    UmlManager m;
    Class& c = m.create<Class>();
    TemplateSignature& s = m.create<TemplateSignature>();
    TemplateParameter& p = m.create<TemplateParameter>();
    c.setOwnedTemplateSignature(&s);
    ASSERT_NO_THROW(s.getOwnedParameter().add(p));
    ASSERT_EQ(s.getOwnedParameter().size(), 1);
    ASSERT_EQ(s.getOwnedParameter().front()->getID(), p.getID());
    ASSERT_EQ(p.getSignature()->getID(), s.getID());
    ASSERT_EQ(s.getOwnedElements().size(), 1);
    ASSERT_EQ(s.getOwnedElements().front()->getID(), p.getID());
    ASSERT_EQ(p.getOwner()->getID(), s.getID());
}

TEST_F(TemplateableElementTest, overrideSignatureTest) {
    UmlManager m;
    Class& c1 = m.create<Class>();
    Class& c2 = m.create<Class>();
    TemplateSignature& s1 = m.create<TemplateSignature>();
    TemplateSignature& s2 = m.create<TemplateSignature>();
    TemplateParameter& p = m.create<TemplateParameter>();
    c1.setOwnedTemplateSignature(&s1);
    c2.setOwnedTemplateSignature(&s2);
    s1.getOwnedParameter().add(p);
    ASSERT_NO_THROW(p.setSignature(&s2));
    ASSERT_TRUE(p.getSignature() == &s2);
    ASSERT_EQ(s2.getOwnedParameter().size(), 1);
    ASSERT_EQ(s2.getOwnedParameter().front()->getID(), p.getID());
    ASSERT_EQ(s1.getOwnedParameter().size(), 0);
    ASSERT_EQ(p.getOwner()->getID(), s2.getID());
    ASSERT_EQ(s2.getOwnedElements().size(), 1);
    ASSERT_EQ(s2.getOwnedElements().front()->getID(), p.getID());
    ASSERT_EQ(s1.getOwnedElements().size(), 0);
}

TEST_F(TemplateableElementTest, copyTemplateableTest) {
    UmlManager m;
    Class&c = m.create<Class>();
    TemplateSignature& s = m.create<TemplateSignature>();
    TemplateParameter& p = m.create<TemplateParameter>();
    c.setOwnedTemplateSignature(&s);
    s.getOwnedParameter().add(p);
    Class c2 = c;
    ASSERT_EQ(c2.getID(), c.getID());
    ASSERT_EQ(c2.getOwnedTemplateSignature()->getID(), c.getOwnedTemplateSignature()->getID());

    Operation&o = m.create<Operation>();
    TemplateSignature& s1 = m.create<TemplateSignature>();
    TemplateParameter& p1 = m.create<TemplateParameter>();
    o.setOwnedTemplateSignature(&s1);
    s1.getOwnedParameter().add(p1);
    Operation o2 = o;
    ASSERT_EQ(o2.getID(), o.getID());
    ASSERT_EQ(o2.getOwnedTemplateSignature()->getID(), o.getOwnedTemplateSignature()->getID());
}

TEST_F(TemplateableElementTest, setOwnedParameteredElementTest) {
    UmlManager m;
    Class& c = m.create<Class>();
    TemplateSignature& s = m.create<TemplateSignature>();
    TemplateParameter& p = m.create<TemplateParameter>();
    PrimitiveType& t = m.create<PrimitiveType>();
    c.setOwnedTemplateSignature(&s);
    s.getOwnedParameter().add(p);
    ASSERT_NO_THROW(p.setOwnedParameteredElement(&t));
    ASSERT_EQ(p.getOwnedParameteredElement()->getID(), t.getID());
    ASSERT_TRUE(p.getParameteredElement() != 0);
    ASSERT_EQ(p.getParameteredElement()->getID(), t.getID());
    ASSERT_EQ(p.getOwnedElements().size(), 1);
    ASSERT_EQ(p.getOwnedElements().front()->getID(), t.getID());
    ASSERT_EQ(t.getOwner()->getID(), p.getID());
}

TEST_F(TemplateableElementTest, overrideOwnedParameteredElementTest) {
    UmlManager m;
    Class& c = m.create<Class>();
    TemplateSignature& s = m.create<TemplateSignature>();
    TemplateParameter& p = m.create<TemplateParameter>();
    PrimitiveType& t1 = m.create<PrimitiveType>();
    PrimitiveType& t2 = m.create<PrimitiveType>();
    c.setOwnedTemplateSignature(&s);
    s.getOwnedParameter().add(p);
    p.setOwnedParameteredElement(&t1);
    p.setOwnedParameteredElement(&t2);
    ASSERT_EQ(p.getOwnedParameteredElement()->getID(), t2.getID());
    ASSERT_TRUE(p.getParameteredElement() != 0);
    ASSERT_EQ(p.getParameteredElement()->getID(), t2.getID());
    ASSERT_EQ(p.getOwnedElements().size(), 1);
    ASSERT_EQ(p.getOwnedElements().front()->getID(), t2.getID());
    ASSERT_EQ(t2.getOwner()->getID(), p.getID());
    ASSERT_TRUE(t1.getOwner() == 0);
}

TEST_F(TemplateableElementTest, setDefaultTest) {
    UmlManager m;
    Class& c = m.create<Class>();
    TemplateSignature& s = m.create<TemplateSignature>();
    TemplateParameter& p = m.create<TemplateParameter>();
    PrimitiveType& t = m.create<PrimitiveType>();
    c.setOwnedTemplateSignature(&s);
    s.getOwnedParameter().add(p);
    p.setDefault(&t);
    ASSERT_TRUE(p.getDefault() != 0);
    ASSERT_EQ(p.getDefault()->getID(), t.getID());
}

TEST_F(TemplateableElementTest, overrideDefaultTest) {
    UmlManager m;
    Class& c = m.create<Class>();
    TemplateSignature& s = m.create<TemplateSignature>();
    TemplateParameter& p = m.create<TemplateParameter>();
    PrimitiveType& t1 = m.create<PrimitiveType>();
    PrimitiveType& t2 = m.create<PrimitiveType>();
    c.setOwnedTemplateSignature(&s);
    s.getOwnedParameter().add(p);
    p.setDefault(&t1);
    p.setDefault(&t2);
    ASSERT_TRUE(p.getDefault() != 0);
    ASSERT_EQ(p.getDefault()->getID(), t2.getID());
}