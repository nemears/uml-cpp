#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/yumlParsersTest.h"

using namespace UML;

class TemplateableElementTest : public ::testing::Test {
    public:
    std::string ymlPath = YML_FILES_PATH;
};

TEST_F(TemplateableElementTest, setOwnedTemplateSignature) {
    UmlManager m;
    Class& c = m.create<Class>();
    TemplateSignature& t = m.create<TemplateSignature>();
    ASSERT_NO_THROW(c.setOwnedTemplateSignature(&t));
    ASSERT_EQ(t.getTemplate(), &c);
    ASSERT_TRUE(c.getOwnedElements().size() == 1);
    ASSERT_EQ(*c.getOwnedElements().begin(), t);
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
    ASSERT_EQ(*c.getOwnedElements().begin(), t2);
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
    ASSERT_EQ(*c.getOwnedElements().begin(), t);
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
    ASSERT_EQ((*c2.getOwnedElements().begin()).getID(), t.getID());
    ASSERT_EQ(t.getOwner()->getID(), c2.getID());
}

TEST_F(TemplateableElementTest, addTemplateParameterTest) {
    UmlManager m;
    Class& c = m.create<Class>();
    TemplateSignature& s = m.create<TemplateSignature>();
    TemplateParameter& p = m.create<TemplateParameter>();
    c.setOwnedTemplateSignature(&s);
    ASSERT_NO_THROW(s.getOwnedParameters().add(p));
    ASSERT_EQ(s.getOwnedParameters().size(), 1);
    ASSERT_EQ(s.getOwnedParameters().front().getID(), p.getID());
    ASSERT_EQ(s.getParameters().size(), 1);
    ASSERT_EQ(s.getParameters().front().getID(), p.getID());
    ASSERT_EQ(p.getSignature()->getID(), s.getID());
    ASSERT_EQ(s.getOwnedElements().size(), 1);
    ASSERT_EQ((*s.getOwnedElements().begin()).getID(), p.getID());
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
    s1.getOwnedParameters().add(p);
    ASSERT_NO_THROW(p.setSignature(&s2));
    ASSERT_TRUE(p.getSignature() == &s2);
    ASSERT_EQ(s2.getOwnedParameters().size(), 1);
    ASSERT_EQ(s2.getOwnedParameters().front().getID(), p.getID());
    ASSERT_EQ(s2.getParameters().size(), 1);
    ASSERT_EQ(s2.getParameters().front().getID(), p.getID());
    ASSERT_EQ(s1.getOwnedParameters().size(), 0);
    ASSERT_EQ(s1.getParameters().size(), 0);
    ASSERT_EQ(p.getOwner()->getID(), s2.getID());
    ASSERT_EQ(s2.getOwnedElements().size(), 1);
    ASSERT_EQ((*s2.getOwnedElements().begin()).getID(), p.getID());
    ASSERT_EQ(s1.getOwnedElements().size(), 0);
}

TEST_F(TemplateableElementTest, copyTemplateableTest) {
    UmlManager m;
    Class&c = m.create<Class>();
    TemplateSignature& s = m.create<TemplateSignature>();
    TemplateParameter& p = m.create<TemplateParameter>();
    c.setOwnedTemplateSignature(&s);
    s.getOwnedParameters().add(p);
    Class c2 = c;
    ASSERT_EQ(c2.getID(), c.getID());
    ASSERT_EQ(c2.getOwnedTemplateSignature()->getID(), c.getOwnedTemplateSignature()->getID());

    Operation&o = m.create<Operation>();
    TemplateSignature& s1 = m.create<TemplateSignature>();
    TemplateParameter& p1 = m.create<TemplateParameter>();
    o.setOwnedTemplateSignature(&s1);
    s1.getOwnedParameters().add(p1);
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
    s.getOwnedParameters().add(p);
    ASSERT_NO_THROW(p.setOwnedParameteredElement(&t));
    ASSERT_EQ(p.getOwnedParameteredElement()->getID(), t.getID());
    ASSERT_TRUE(p.getParameteredElement() != 0);
    ASSERT_EQ(p.getParameteredElement()->getID(), t.getID());
    ASSERT_EQ(p.getOwnedElements().size(), 1);
    ASSERT_EQ((*p.getOwnedElements().begin()).getID(), t.getID());
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
    s.getOwnedParameters().add(p);
    p.setOwnedParameteredElement(&t1);
    p.setOwnedParameteredElement(&t2);
    ASSERT_EQ(p.getOwnedParameteredElement()->getID(), t2.getID());
    ASSERT_TRUE(p.getParameteredElement() != 0);
    ASSERT_EQ(p.getParameteredElement()->getID(), t2.getID());
    ASSERT_EQ(p.getOwnedElements().size(), 1);
    ASSERT_EQ((*p.getOwnedElements().begin()).getID(), t2.getID());
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
    s.getOwnedParameters().add(p);
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
    s.getOwnedParameters().add(p);
    p.setDefault(&t1);
    p.setDefault(&t2);
    ASSERT_TRUE(p.getDefault() != 0);
    ASSERT_EQ(p.getDefault()->getID(), t2.getID());
}

TEST_F(TemplateableElementTest, setOwnedDefaultTest) {
    UmlManager m;
    Class& c = m.create<Class>();
    TemplateSignature& s = m.create<TemplateSignature>();
    TemplateParameter& p = m.create<TemplateParameter>();
    PrimitiveType& t = m.create<PrimitiveType>();
    c.setOwnedTemplateSignature(&s);
    s.getOwnedParameters().add(p);
    p.setOwnedDefault(&t);
    ASSERT_TRUE(p.getOwnedDefault() != 0);
    ASSERT_EQ(p.getOwnedDefault()->getID(), t.getID());
    ASSERT_TRUE(p.getDefault() != 0);
    ASSERT_EQ(p.getDefault()->getID(), t.getID());
    ASSERT_EQ(p.getOwnedElements().size(), 1);
    ASSERT_EQ((*p.getOwnedElements().begin()).getID(), t.getID());
    ASSERT_TRUE(t.getOwner() != 0);
    ASSERT_EQ(t.getOwner()->getID(), p.getID());
}

TEST_F(TemplateableElementTest, overrideOwnedDefaultTest) {
    UmlManager m;
    Class& c = m.create<Class>();
    TemplateSignature& s = m.create<TemplateSignature>();
    TemplateParameter& p = m.create<TemplateParameter>();
    PrimitiveType& t1 = m.create<PrimitiveType>();
    PrimitiveType& t2 = m.create<PrimitiveType>();
    c.setOwnedTemplateSignature(&s);
    s.getOwnedParameters().add(p);
    p.setOwnedDefault(&t1);
    p.setOwnedDefault(&t2);
    ASSERT_TRUE(p.getOwnedDefault() != 0);
    ASSERT_EQ(p.getOwnedDefault()->getID(), t2.getID());
    ASSERT_TRUE(p.getDefault() != 0);
    ASSERT_EQ(p.getDefault()->getID(), t2.getID());
    ASSERT_EQ(p.getOwnedElements().size(), 1);
    ASSERT_EQ((*p.getOwnedElements().begin()).getID(), t2.getID());
    ASSERT_TRUE(t2.getOwner() != 0);
    ASSERT_EQ(t2.getOwner()->getID(), p.getID());
    ASSERT_TRUE(t1.getOwner() == 0);
}

TEST_F(TemplateableElementTest, basicTemplateBindTest) {
    UmlManager m;
    Class& c = m.create<Class>();
    TemplateSignature& s = m.create<TemplateSignature>();
    TemplateBinding& b = m.create<TemplateBinding>();
    b.setBoundElement(&c);
    b.setSignature(&s);
    ASSERT_TRUE(b.getBoundElement() != 0);
    ASSERT_EQ(b.getBoundElement()->getID(), c.getID());
    ASSERT_EQ(c.getTemplateBindings().size(), 1);
    ASSERT_EQ(c.getTemplateBindings().front().getID(), b.getID());
    ASSERT_EQ(b.getSources().size(), 1);
    ASSERT_EQ(b.getSources().front().getID(), c.getID());
    ASSERT_EQ(c.getOwnedElements().size(), 1);
    ASSERT_EQ((*c.getOwnedElements().begin()).getID(), b.getID());
    ASSERT_TRUE(b.getSignature() != 0);
    ASSERT_EQ(b.getSignature()->getID(), s.getID());
    ASSERT_EQ(b.getTargets().size(), 1);
    ASSERT_EQ(b.getTargets().front().getID(), s.getID());
    ASSERT_EQ(b.getRelatedElements().size(), 2);
    ASSERT_TRUE(b.getRelatedElements().contains(c.getID()));
    ASSERT_TRUE(b.getRelatedElements().contains(s.getID()));
}

TEST_F(TemplateableElementTest, overrideBoundElementTest) {
    UmlManager m;
    Class& c1 = m.create<Class>();
    Class& c2 = m.create<Class>();
    TemplateSignature& s = m.create<TemplateSignature>();
    TemplateBinding& b = m.create<TemplateBinding>();
    b.setBoundElement(&c1);
    b.setSignature(&s);
    b.setBoundElement(&c2);
    ASSERT_TRUE(b.getBoundElement() != 0);
    ASSERT_EQ(b.getBoundElement()->getID(), c2.getID());
    ASSERT_EQ(b.getSources().size(), 1);
    ASSERT_EQ(b.getSources().front().getID(), c2.getID());
    ASSERT_EQ(b.getRelatedElements().size(), 2);
    ASSERT_TRUE(b.getRelatedElements().contains(c2.getID()));
    ASSERT_TRUE(b.getOwner() != 0);
    ASSERT_EQ(b.getOwner()->getID(), c2.getID());
    ASSERT_EQ(c2.getTemplateBindings().size(), 1);
    ASSERT_EQ(c2.getTemplateBindings().front().getID(), b.getID());
    ASSERT_EQ(c2.getOwnedElements().size(), 1);
    ASSERT_EQ((*c2.getOwnedElements().begin()).getID(), b.getID());
    ASSERT_EQ(c1.getTemplateBindings().size(), 0);
    ASSERT_EQ(c1.getOwnedElements().size(), 0);
}

TEST_F(TemplateableElementTest, overrideSignatureForBindingTest) {
    UmlManager m;
    Class& c = m.create<Class>();
    TemplateSignature& s1 = m.create<TemplateSignature>();
    TemplateSignature& s2 = m.create<TemplateSignature>();
    TemplateBinding& b = m.create<TemplateBinding>();
    b.setBoundElement(&c);
    b.setSignature(&s1);
    b.setSignature(&s2);
    ASSERT_TRUE(b.getSignature() != 0);
    ASSERT_EQ(b.getSignature()->getID(), s2.getID());
    ASSERT_EQ(b.getTargets().size(), 1);
    ASSERT_EQ(b.getTargets().front().getID(), s2.getID());
}

TEST_F(TemplateableElementTest, setFormalParameterTest) {
    UmlManager m;
    Class& c = m.create<Class>();
    TemplateSignature& s = m.create<TemplateSignature>();
    TemplateBinding& b = m.create<TemplateBinding>();
    Class& bc = m.create<Class>();
    PrimitiveType& t = m.create<PrimitiveType>();
    TemplateParameter& p = m.create<TemplateParameter>();
    TemplateParameter& pp = m.create<TemplateParameter>();
    TemplateParameterSubstitution& ps = m.create<TemplateParameterSubstitution>();
    c.setOwnedTemplateSignature(&s);
    s.getOwnedParameters().add(p);
    b.setBoundElement(&bc);
    b.setSignature(&s);
    b.getParameterSubstitution().add(ps);
    pp.setOwnedParameteredElement(&t);
    ps.setFormal(&pp);
    ASSERT_TRUE(ps.getFormal() != 0);
    ASSERT_EQ(ps.getFormal()->getID(), pp.getID());
    ASSERT_TRUE(ps.getTemplateBinding() != 0);
    ASSERT_EQ(ps.getTemplateBinding()->getID(), b.getID());
    ASSERT_EQ(b.getParameterSubstitution().size(), 1);
    ASSERT_EQ(b.getParameterSubstitution().front().getID(), ps.getID());
}

TEST_F(TemplateableElementTest, overrideFormalTest) {
    UmlManager m;
    Class& c = m.create<Class>();
    TemplateSignature& s = m.create<TemplateSignature>();
    TemplateBinding& b = m.create<TemplateBinding>();
    Class& bc = m.create<Class>();
    PrimitiveType& t = m.create<PrimitiveType>();
    TemplateParameter& p = m.create<TemplateParameter>();
    TemplateParameter& pp = m.create<TemplateParameter>();
    TemplateParameterSubstitution& ps = m.create<TemplateParameterSubstitution>();
    TemplateParameter& pp2 = m.create<TemplateParameter>();
    c.setOwnedTemplateSignature(&s);
    s.getOwnedParameters().add(p);
    b.setBoundElement(&bc);
    b.setSignature(&s);
    b.getParameterSubstitution().add(ps);
    pp.setOwnedParameteredElement(&t);
    ps.setFormal(&pp);
    ps.setFormal(&pp2);
    ASSERT_TRUE(ps.getFormal() != 0);
    ASSERT_EQ(ps.getFormal()->getID(), pp2.getID());
    ASSERT_TRUE(ps.getTemplateBinding() != 0);
    ASSERT_EQ(ps.getTemplateBinding()->getID(), b.getID());
    ASSERT_EQ(b.getParameterSubstitution().size(), 1);
    ASSERT_EQ(b.getParameterSubstitution().front().getID(), ps.getID());
}

TEST_F(TemplateableElementTest, setActualTest) {
    UmlManager m;
    TemplateParameterSubstitution& s = m.create<TemplateParameterSubstitution>();
    PrimitiveType& t = m.create<PrimitiveType>();
    s.setActual(&t);
    ASSERT_TRUE(s.getActual() != 0);
    ASSERT_EQ(s.getActual()->getID(), t.getID());
}

TEST_F(TemplateableElementTest, nullActualTest) {
    UmlManager m;
    TemplateParameterSubstitution& s = m.create<TemplateParameterSubstitution>();
    PrimitiveType& t = m.create<PrimitiveType>();
    ASSERT_TRUE(s.getActual() == 0);
    s.setActual(&t);
    ASSERT_TRUE(s.getActual() != 0);
    ASSERT_EQ(s.getActual()->getID(), t.getID());
    s.setActual(0);
    ASSERT_TRUE(s.getActual() == 0);
    PrimitiveType& t2 = m.create<PrimitiveType>();
    s.setActual(&t);
    s.setActual(&t2);
    ASSERT_TRUE(s.getActual() != 0);
    ASSERT_EQ(s.getActual()->getID(), t2.getID());
}

TEST_F(TemplateableElementTest, setOwnedActualTestAndOverride) {
    UmlManager m;
    TemplateParameterSubstitution& s = m.create<TemplateParameterSubstitution>();
    PrimitiveType& a1 = m.create<PrimitiveType>();
    PrimitiveType& a2 = m.create<PrimitiveType>();
    s.setOwnedActual(&a1);
    ASSERT_TRUE(s.getActual() != 0);
    ASSERT_TRUE(s.getOwnedActual() != 0);
    ASSERT_EQ(s.getActual()->getID(), a1.getID());
    ASSERT_EQ(s.getOwnedActual()->getID(), a1.getID());
    ASSERT_EQ(s.getOwnedElements().size(), 1);
    ASSERT_EQ((*s.getOwnedElements().begin()).getID(), a1.getID());
    ASSERT_TRUE(a1.getOwner() != 0);
    ASSERT_EQ(a1.getOwner()->getID(), s.getID());
    s.setOwnedActual(&a2);
    ASSERT_TRUE(s.getActual() != 0);
    ASSERT_TRUE(s.getOwnedActual() != 0);
    ASSERT_EQ(s.getActual()->getID(), a2.getID());
    ASSERT_EQ(s.getOwnedActual()->getID(), a2.getID());
    ASSERT_EQ(s.getOwnedElements().size(), 1);
    ASSERT_EQ((*s.getOwnedElements().begin()).getID(), a2.getID());
    ASSERT_TRUE(a2.getOwner() != 0);
    ASSERT_EQ(a2.getOwner()->getID(), s.getID());
    ASSERT_TRUE(a1.getOwner() == 0);
}

TEST_F(TemplateableElementTest, reindexTemplateableElementsTest) {
    UmlManager m;
    Class templateClass = m.create<Class>();
    TemplateSignature classSignature = m.create<TemplateSignature>();
    TemplateBinding classBinding = m.create<TemplateBinding>();
    Package root = m.create<Package>();
    templateClass.setOwnedTemplateSignature(classSignature);
    templateClass.getTemplateBindings().add(classBinding);
    classBinding.setSignature(classSignature);
    root.getPackagedElements().add(templateClass);
    m.setRoot(&root);
    m.mount(ymlPath + "templateableElementTests");
    ID classID = ID::fromString("x6iYAE&S1H7jjIFXc0Um0zdAfOVl");
    templateClass.setID(classID);
    m.release(templateClass);
    ASSERT_NO_THROW(ASSERT_EQ(classSignature.getTemplateRef().getID(), classID));
    m.release(classID);
    ASSERT_NO_THROW(ASSERT_EQ(classBinding.getBoundElementRef().getID(), classID));
}