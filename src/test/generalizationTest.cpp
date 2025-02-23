#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

using namespace UML;

UML_SINGLETON_INTEGRATION_TEST(GeneralizationSpecific, DataType, Generalization, getSpecificSingleton)
UML_SINGLETON_INTEGRATION_TEST(GeneralizationGeneral, Class, Generalization, getGeneralSingleton)
// 
// typedef UmlPtr<Generalization> GeneralizationPtr;

class GeneralizationTest : public ::testing::Test {
   
};

TEST_F(GeneralizationTest, basicSetSpecificAndGeneralTest) {
    UmlManager m;
    auto r = m.create<Generalization>();
    auto s = m.create<Class>();
    auto g = m.create<Class>();
    r->setSpecific(*s);
    r->setGeneral(*g);
    ASSERT_EQ(r->getSpecific(), s);
    ASSERT_EQ(r->getGeneral(), g);
    ASSERT_EQ(r->getTargets().size(), 1);
    ASSERT_EQ(r->getTargets().front(), g);
    ASSERT_EQ(r->getSources().size(), 1);
    ASSERT_EQ(r->getSources().front(), s);
    ASSERT_EQ(r->getRelatedElements().size(), 2);
    ASSERT_EQ(r->getRelatedElements().contains(s->getID()), 1);
    ASSERT_EQ(r->getRelatedElements().contains(g->getID()), 1);
    ASSERT_TRUE(r->getOwner());
    ASSERT_EQ(r->getOwner(), s);
    ASSERT_EQ(s->getGeneralizations().size(), 1);
    ASSERT_EQ(s->getGeneralizations().front(), r);
    ASSERT_EQ(s->getOwnedElements().size(), 1);
    ASSERT_EQ(s->getOwnedElements().front(), r);
    ASSERT_EQ(g->getGeneralizations().size(), 0);
}

TEST_F(GeneralizationTest, addGeneralFunctorTest) {
    UmlManager m;
    auto& s = *m.create<Class>();
    auto& g = *m.create<Class>();
    s.getGenerals().add(g);
    ASSERT_EQ(s.getGenerals().size(), 1);
    ASSERT_EQ(s.getGenerals().front(), &g);
    ASSERT_EQ(s.getGeneralizations().size(), 1);
    auto r = s.getGeneralizations().front();
    ASSERT_TRUE(r->getSpecific());
    ASSERT_EQ(*r->getSpecific(), s);
    ASSERT_TRUE(r->getGeneral());
    ASSERT_EQ(*r->getGeneral(), g);
    ASSERT_EQ(r->getTargets().size(), 1);
    ASSERT_EQ(r->getTargets().front(), &g);
    ASSERT_EQ(r->getSources().size(), 1);
    ASSERT_EQ(r->getSources().front(), &s);
    ASSERT_EQ(r->getRelatedElements().size(), 2);
    ASSERT_EQ(r->getRelatedElements().contains(s.getID()), 1);
    ASSERT_EQ(r->getRelatedElements().contains(g.getID()), 1);
    ASSERT_EQ(*r->getOwner(), s);
    ASSERT_EQ(s.getGeneralizations().size(), 1);
    ASSERT_EQ(s.getGeneralizations().front(), r);
    ASSERT_EQ(s.getOwnedElements().size(), 1);
    ASSERT_EQ(s.getOwnedElements().front(), r);
}

TEST_F(GeneralizationTest, AddGeneralizationFunctorTest) {
    UmlManager m;
    auto s = m.create<Class>();
    auto g = m.create<Class>();
    auto r = m.create<Generalization>();
    s->getGeneralizations().add(*r);
    r->setGeneral(*g);
    ASSERT_TRUE(r->getGeneral());
    ASSERT_EQ(r->getSpecific(), s);
}

TEST_F(GeneralizationTest, AddGeneralizationFunctorGeneralSetTest) {
    UmlManager m;
    auto s = m.create<Class>();
    auto g = m.create<Class>();
    auto r = m.create<Generalization>();
    r->setGeneral(*g);
    s->getGeneralizations().add(*r);
    ASSERT_TRUE(r->getSpecific());
    ASSERT_EQ(r->getSpecific(), s);
}

TEST_F(GeneralizationTest, resetGeneralTest) {
    UmlManager m;
    auto& r = *m.create<Generalization>();
    auto& g = *m.create<Class>();
    auto& s = *m.create<Class>();
    auto& g2 = *m.create<Class>();
    r.setGeneral(g);
    r.setSpecific(s);
    r.setGeneral(g2);
    ASSERT_EQ(g.getGeneralizations().size(), 0);
    ASSERT_EQ(s.getGeneralizations().size(), 1);
    ASSERT_EQ(*s.getGeneralizations().front(), r);
    ASSERT_EQ(g2.getGeneralizations().size(), 0);
    ASSERT_EQ(r.getRelatedElements().size(), 2);
    ASSERT_TRUE(r.getRelatedElements().contains(s.getID()));
    ASSERT_TRUE(r.getRelatedElements().contains(g2.getID()));
    ASSERT_EQ(r.getTargets().size(), 1);
    ASSERT_EQ(*r.getTargets().front(), g2);
    ASSERT_EQ(r.getSources().size(), 1);
    ASSERT_EQ(*r.getSources().front(), s);
}

TEST_F(GeneralizationTest, resetSpecificTest) {
    UmlManager m;
    auto& r = *m.create<Generalization>();
    auto& g = *m.create<Class>();
    auto& s = *m.create<Class>();
    auto& s2 = *m.create<Class>();
    r.setGeneral(g);
    r.setSpecific(s);
    r.setSpecific(s2);
    ASSERT_EQ(s.getGeneralizations().size(), 0);
    ASSERT_EQ(g.getGeneralizations().size(), 0);
    ASSERT_EQ(s2.getGeneralizations().size(), 1);
    ASSERT_EQ(*s2.getGeneralizations().front(), r);
    ASSERT_EQ(r.getRelatedElements().size(), 2);
    ASSERT_TRUE(r.getRelatedElements().contains(g.getID()));
    ASSERT_TRUE(r.getRelatedElements().contains(s2.getID()));
    ASSERT_EQ(r.getTargets().size(), 1);
    ASSERT_EQ(r.getTargets().front(), &g);
    ASSERT_EQ(r.getSources().size(), 1);
    ASSERT_EQ(r.getSources().front(), &s2);
    ASSERT_TRUE(r.getGeneral());
    ASSERT_EQ(*r.getGeneral(), g);
    ASSERT_TRUE(r.getSpecific());
    ASSERT_EQ(*r.getSpecific(), s2);
}

TEST_F(GeneralizationTest, backwardsResetGeneralTest) {
    UmlManager m;
    auto& r = *m.create<Generalization>();
    auto& g = *m.create<Class>();
    auto& s = *m.create<Class>();
    auto& g2 = *m.create<Class>();
    r.setSpecific(s);
    r.setGeneral(g);
    r.setGeneral(g2);
    ASSERT_EQ(g.getGeneralizations().size(), 0);
    ASSERT_EQ(s.getGeneralizations().size(), 1);
    ASSERT_EQ(s.getGeneralizations().front(), &r);
    ASSERT_EQ(g2.getGeneralizations().size(), 0);
    ASSERT_EQ(r.getRelatedElements().size(), 2);
    ASSERT_TRUE(r.getRelatedElements().contains(s.getID()));
    ASSERT_TRUE(r.getRelatedElements().contains(g2.getID()));
    ASSERT_EQ(r.getTargets().size(), 1);
    ASSERT_EQ(r.getTargets().front(), &g2);
    ASSERT_EQ(r.getSources().size(), 1);
    ASSERT_EQ(r.getSources().front(), &s);
}

TEST_F(GeneralizationTest, backwardsResetSpecificTest) {
    UmlManager m;
    auto& r = *m.create<Generalization>();
    auto& g = *m.create<Class>();
    auto& s = *m.create<Class>();
    auto& s2 = *m.create<Class>();
    r.setSpecific(s);
    r.setGeneral(g);
    r.setSpecific(s2);
    ASSERT_EQ(s.getGeneralizations().size(), 0);
    ASSERT_EQ(g.getGeneralizations().size(), 0);
    ASSERT_EQ(s2.getGeneralizations().size(), 1);
    ASSERT_EQ(s2.getGeneralizations().front(), &r);
    ASSERT_EQ(r.getRelatedElements().size(), 2);
    ASSERT_TRUE(r.getRelatedElements().contains(g.getID()));
    ASSERT_TRUE(r.getRelatedElements().contains(s2.getID()));
    ASSERT_EQ(r.getTargets().size(), 1);
    ASSERT_EQ(r.getTargets().front(), &g);
    ASSERT_EQ(r.getSources().size(), 1);
    ASSERT_EQ(r.getSources().front(), &s2);
    ASSERT_TRUE(r.getGeneral());
    ASSERT_EQ(*r.getGeneral(), g);
    ASSERT_TRUE(r.getSpecific());
    ASSERT_EQ(*r.getSpecific(), s2);
}

TEST_F(GeneralizationTest, ResetSpecificByGeneralTest) {
    UmlManager m;
    auto& g = *m.create<Class>();
    auto& s = *m.create<Class>();
    auto& s2 = *m.create<Class>();
    s.getGenerals().add(g);
    auto r = s.getGeneralizations().front();
    r->setSpecific(&s2);
    ASSERT_TRUE(s.getGeneralizations().size() == 0);
    ASSERT_TRUE(g.getGeneralizations().size() == 0);
    ASSERT_TRUE(s2.getGeneralizations().size() == 1);
    ASSERT_TRUE(s2.getGeneralizations().front() == r);
    ASSERT_TRUE(r->getRelatedElements().size() == 2);
    ASSERT_TRUE(r->getRelatedElements().contains(g.getID()));
    ASSERT_TRUE(r->getRelatedElements().contains(s2.getID()));
    ASSERT_TRUE(r->getTargets().size() == 1);
    ASSERT_TRUE(r->getTargets().front() == &g);
    ASSERT_TRUE(r->getSources().size() == 1);
    ASSERT_TRUE(r->getSources().front() == &s2);
    ASSERT_TRUE(r->getGeneral() == &g);
    ASSERT_TRUE(r->getSpecific() == &s2);
}
