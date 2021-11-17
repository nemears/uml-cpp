#include "gtest/gtest.h"
#include "uml/generalization.h"
#include "uml/classifier.h"
#include "uml/class.h"

using namespace UML;

class GeneralizationTest : public ::testing::Test {
   
};

TEST_F(GeneralizationTest, basicSetSpecificAndGeneralTest) {
    UmlManager m;
    Generalization r = m.create<Generalization>();
    Class s = m.create<Class>();
    Class g = m.create<Class>();
    r.setSpecific(s);
    r.setGeneral(g);
    ASSERT_EQ(r.getSpecificRef(), s);
    ASSERT_EQ(r.getGeneralRef(), g);
    ASSERT_EQ(r.getTargets().size(), 1);
    ASSERT_EQ(r.getTargets().front(), g);
    ASSERT_EQ(r.getSources().size(), 1);
    ASSERT_EQ(r.getSources().front(), s);
    ASSERT_EQ(r.getRelatedElements().size(), 2);
    ASSERT_EQ(r.getRelatedElements().count(s.getID()), 1);
    ASSERT_EQ(r.getRelatedElements().count(g.getID()), 1);
    ASSERT_TRUE(r.getOwner());
    ASSERT_TRUE(*r.getOwner() == s);
    // ASSERT_EQ(s.getRelationships().size(), 1);
    // ASSERT_EQ(s.getRelationships().front(), r);
    // ASSERT_EQ(g.getRelationships().size(), 1);
    // ASSERT_EQ(g.getRelationships().front(), r);
    ASSERT_EQ(s.getGeneralizations().size(), 1);
    ASSERT_EQ(s.getGeneralizations().front(), r);
    ASSERT_EQ(s.getOwnedElements().size(), 1);
    ASSERT_EQ(*s.getOwnedElements().begin(), r);
    ASSERT_EQ(g.getGeneralizations().size(), 0);
    // ASSERT_EQ(s.getGenerals().size(), 1);
    // ASSERT_EQ(s.getGenerals().front(), g);
}

TEST_F(GeneralizationTest, addGeneralFunctorTest) {
    // UmlManager m;
    // Classifier& s = m.create<Classifier>();
    // Classifier& g = m.create<Classifier>();
    // s.getGenerals().add(g);
    // ASSERT_EQ(s.getGenerals().size(), 1);
    // ASSERT_EQ(s.getGenerals().front(), g);
    // ASSERT_EQ(s.getGeneralizations().size(), 1);
    // Generalization* r = &s.getGeneralizations().front();
    // ASSERT_TRUE(r->hasSpecific());
    // ASSERT_EQ(r->getSpecificRef(), s);
    // ASSERT_TRUE(r->hasGeneral());
    // ASSERT_EQ(r->getGeneralRef(), g);
    // ASSERT_EQ(r->getTargets().size(), 1);
    // ASSERT_EQ(r->getTargets().front(), g);
    // ASSERT_EQ(r->getSources().size(), 1);
    // ASSERT_EQ(r->getSources().front(), s);
    // ASSERT_EQ(r->getRelatedElements().size(), 2);
    // ASSERT_EQ(r->getRelatedElements().count(s.getID()), 1);
    // ASSERT_EQ(r->getRelatedElements().count(g.getID()), 1);
    // ASSERT_EQ(*r->getOwner(), s);
    // ASSERT_EQ(s.getRelationships().size(), 1);
    // ASSERT_EQ(s.getRelationships().front(), *r);
    // ASSERT_EQ(g.getRelationships().size(), 1);
    // ASSERT_EQ(g.getRelationships().front(), *r);
    // ASSERT_EQ(s.getGeneralizations().size(), 1);
    // ASSERT_EQ(s.getGeneralizations().front(), *r);
    // ASSERT_EQ(s.getOwnedElements().size(), 1);
    // ASSERT_EQ(*s.getOwnedElements().begin(), *r);
}

TEST_F(GeneralizationTest, AddGeneralizationFunctorTest) {
    UmlManager m;
    Class s = m.create<Class>();
    Class g = m.create<Class>();
    Generalization r = m.create<Generalization>();
    s.getGeneralizations().add(r);
    r.setGeneral(&g);
    ASSERT_TRUE(r.hasGeneral());
    ASSERT_EQ(r.getSpecificRef(), s);
}

TEST_F(GeneralizationTest, AddGeneralizationFunctorGeneralSetTest) {
    UmlManager m;
    Class s = m.create<Class>();
    Class g = m.create<Class>();
    Generalization r = m.create<Generalization>();
    r.setGeneral(&g);
    s.getGeneralizations().add(r);
    ASSERT_TRUE(r.hasSpecific());
    ASSERT_EQ(r.getSpecificRef(), s);
}

TEST_F(GeneralizationTest, CheckGeneralizationFunctorTest) {
    // UmlManager m;
    // Generalization r = m.create<Generalization>();
    // Class g = m.create<Class>();
    // Class s = m.create<Class>();
    // r.setGeneral(g);
    // r.setSpecific(s);
    // ASSERT_THROW(g.getGeneralizations().add(r), InvalidGeneralizationException);
}

TEST_F(GeneralizationTest, resetGeneralTest) {
    UmlManager m;
    Generalization& r = m.create<Generalization>();
    Class& g = m.create<Class>();
    Class& s = m.create<Class>();
    Class& g2 = m.create<Class>();
    r.setGeneral(&g);
    r.setSpecific(&s);
    r.setGeneral(&g2);
    // ASSERT_TRUE(g.getDirectedRelationships().size() == 0);
    // ASSERT_TRUE(g.getRelationships().size() == 0);
    ASSERT_TRUE(g.getGeneralizations().size() == 0);
    // ASSERT_TRUE(s.getGenerals().size() == 1);
    // ASSERT_TRUE(&s.getGenerals().front() == &g2);
    ASSERT_TRUE(s.getGeneralizations().size() == 1);
    ASSERT_TRUE(&s.getGeneralizations().front() == &r);
    ASSERT_TRUE(g2.getGeneralizations().size() == 0);
    // ASSERT_TRUE(g2.getRelationships().size() == 1);
    // ASSERT_TRUE(&g2.getRelationships().front() == &r);
    // ASSERT_TRUE(g2.getDirectedRelationships().size() == 1);
    // ASSERT_TRUE(&g2.getDirectedRelationships().front() == &r);
    ASSERT_TRUE(r.getRelatedElements().size() == 2);
    ASSERT_TRUE(r.getRelatedElements().contains(s.getID()));
    ASSERT_TRUE(r.getRelatedElements().contains(g2.getID()));
    ASSERT_TRUE(r.getTargets().size() == 1);
    ASSERT_TRUE(&r.getTargets().front() == &g2);
    ASSERT_TRUE(r.getSources().size() == 1);
    ASSERT_TRUE(&r.getSources().front() == &s);
}

TEST_F(GeneralizationTest, resetSpecificTest) {
    UmlManager m;
    Generalization r = m.create<Generalization>();
    Class g = m.create<Class>();
    Class s = m.create<Class>();
    Class s2 = m.create<Class>();
    r.setGeneral(&g);
    r.setSpecific(&s);
    r.setSpecific(&s2);
    ASSERT_EQ(s.getGeneralizations().size(), 0);
    // ASSERT_EQ(s.getRelationships().size(), 0);
    // ASSERT_EQ(g.getRelationships().size(), 1);
    // ASSERT_EQ(g.getRelationships().front(), r);
    ASSERT_EQ(g.getGeneralizations().size(), 0);
    ASSERT_EQ(s2.getGeneralizations().size(), 1);
    ASSERT_EQ(s2.getGeneralizations().front(), r);
    // ASSERT_EQ(s2.getRelationships().size(), 1);
    // ASSERT_EQ(s2.getRelationships().front(), r);
    ASSERT_EQ(r.getRelatedElements().size(), 2);
    ASSERT_TRUE(r.getRelatedElements().contains(g.getID()));
    ASSERT_TRUE(r.getRelatedElements().contains(s2.getID()));
    ASSERT_EQ(r.getTargets().size(), 1);
    ASSERT_EQ(r.getTargets().front(), g);
    ASSERT_EQ(r.getSources().size(), 1);
    ASSERT_EQ(r.getSources().front(), s2);
    ASSERT_TRUE(r.hasGeneral());
    ASSERT_EQ(r.getGeneralRef(), g);
    ASSERT_TRUE(r.hasSpecific());
    ASSERT_EQ(r.getSpecificRef(), s2);
}

TEST_F(GeneralizationTest, backwardsResetGeneralTest) {
    UmlManager m;
    Generalization r = m.create<Generalization>();
    Class g = m.create<Class>();
    Class s = m.create<Class>();
    Class g2 = m.create<Class>();
    r.setSpecific(&s);
    r.setGeneral(&g);
    r.setGeneral(&g2);
    // ASSERT_EQ(g.getRelationships().size(), 0);
    ASSERT_EQ(g.getGeneralizations().size(), 0);
    // ASSERT_EQ(s.getGenerals().size(), 1);
    // ASSERT_EQ(s.getGenerals().front(), g2);
    ASSERT_EQ(s.getGeneralizations().size(), 1);
    ASSERT_EQ(s.getGeneralizations().front(), r);
    ASSERT_EQ(g2.getGeneralizations().size(), 0);
    // ASSERT_EQ(g2.getRelationships().size(), 1);
    // ASSERT_EQ(g2.getRelationships().front(), r);
    ASSERT_EQ(r.getRelatedElements().size(), 2);
    ASSERT_TRUE(r.getRelatedElements().contains(s.getID()));
    ASSERT_TRUE(r.getRelatedElements().contains(g2.getID()));
    ASSERT_EQ(r.getTargets().size(), 1);
    ASSERT_EQ(r.getTargets().front(), g2);
    ASSERT_EQ(r.getSources().size(), 1);
    ASSERT_EQ(r.getSources().front(), s);
}

TEST_F(GeneralizationTest, backwardsResetSpecificTest) {
    UmlManager m;
    Generalization r = m.create<Generalization>();
    Class g = m.create<Class>();
    Class s = m.create<Class>();
    Class s2 = m.create<Class>();
    r.setSpecific(&s);
    r.setGeneral(&g);
    r.setSpecific(&s2);
    ASSERT_EQ(s.getGeneralizations().size(), 0);
    // ASSERT_EQ(s.getRelationships().size(), 0);
    // ASSERT_EQ(g.getRelationships().size(), 1);
    // ASSERT_EQ(g.getRelationships().front(), r);
    ASSERT_EQ(g.getGeneralizations().size(), 0);
    ASSERT_EQ(s2.getGeneralizations().size(), 1);
    ASSERT_EQ(s2.getGeneralizations().front(), r);
    // ASSERT_EQ(s2.getRelationships().size(), 1);
    // ASSERT_EQ(s2.getRelationships().front(), r);
    ASSERT_EQ(r.getRelatedElements().size(), 2);
    ASSERT_TRUE(r.getRelatedElements().contains(g.getID()));
    ASSERT_TRUE(r.getRelatedElements().contains(s2.getID()));
    ASSERT_EQ(r.getTargets().size(), 1);
    ASSERT_EQ(r.getTargets().front(), g);
    ASSERT_EQ(r.getSources().size(), 1);
    ASSERT_EQ(r.getSources().front(), s2);
    ASSERT_TRUE(r.hasGeneral());
    ASSERT_EQ(r.getGeneralRef(), g);
    ASSERT_TRUE(r.hasSpecific());
    ASSERT_EQ(r.getSpecificRef(), s2);
}

TEST_F(GeneralizationTest, ResetSpecificByGeneralTest) {
    // UmlManager m;
    // Classifier& g = m.create<Classifier>();
    // Classifier& s = m.create<Classifier>();
    // Classifier& s2 = m.create<Classifier>();
    // s.getGenerals().add(g);
    // Generalization* r = &s.getGeneralizations().front();
    // r->setSpecific(&s2);
    // ASSERT_TRUE(s.getGeneralizations().size() == 0);
    // ASSERT_TRUE(s.getRelationships().size() == 0);
    // ASSERT_TRUE(g.getRelationships().size() == 1);
    // ASSERT_TRUE(&g.getRelationships().front() == r);
    // ASSERT_TRUE(g.getGeneralizations().size() == 0);
    // ASSERT_TRUE(s2.getGeneralizations().size() == 1);
    // ASSERT_TRUE(&s2.getGeneralizations().front() == r);
    // ASSERT_TRUE(s2.getRelationships().size() == 1);
    // ASSERT_TRUE(&s2.getRelationships().front() == r);
    // ASSERT_TRUE(r->getRelatedElements().size() == 2);
    // ASSERT_TRUE(&r->getRelatedElements().front() == &g);
    // ASSERT_TRUE(&r->getRelatedElements().back() == &s2);
    // ASSERT_TRUE(r->getTargets().size() == 1);
    // ASSERT_TRUE(&r->getTargets().front() == &g);
    // ASSERT_TRUE(r->getSources().size() == 1);
    // ASSERT_TRUE(&r->getSources().front() == &s2);
    // ASSERT_TRUE(r->getGeneral() == &g);
    // ASSERT_TRUE(r->getSpecific() == &s2);
}