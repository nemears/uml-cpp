#include "gtest/gtest.h"
#include "uml/generalization.h"

using namespace UML;

class GeneralizationTest : public ::testing::Test {
   
};

TEST_F(GeneralizationTest, basicSetSpecificAndGeneralTest) {
    Generalization r;
    Classifier s;
    Classifier g;
    r.setSpecific(&s);
    r.setGeneral(&g);
    ASSERT_TRUE(r.getSpecific() == &s);
    ASSERT_TRUE(r.getGeneral() == &g);
    ASSERT_TRUE(r.getTargets().size() == 1);
    ASSERT_TRUE(r.getTargets().front() == &g);
    ASSERT_TRUE(r.getSources().size() == 1);
    ASSERT_TRUE(r.getSources().front() == &s);
    ASSERT_TRUE(r.getRelatedElements().size() == 2);
    ASSERT_TRUE(r.getRelatedElements().count(s.getID()) == 1);
    ASSERT_TRUE(r.getRelatedElements().count(g.getID()) == 1);
    ASSERT_TRUE(s.getRelationships().size() == 1);
    ASSERT_TRUE(s.getRelationships().front() == &r);
    ASSERT_TRUE(g.getRelationships().size() == 1);
    ASSERT_TRUE(g.getRelationships().front() == &r);
    ASSERT_TRUE(s.getGeneralizations().size() == 1);
    ASSERT_TRUE(s.getGeneralizations().front() == &r);
    ASSERT_TRUE(g.getGeneralizations().size() == 0);
    ASSERT_TRUE(s.getGenerals().size() == 1);
    ASSERT_TRUE(s.getGenerals().front() == &g);
}

TEST_F(GeneralizationTest, addGeneralFunctorTest) {
    Classifier s;
    Classifier g;
    s.getGenerals().add(g);
    ASSERT_TRUE(s.getGenerals().size() == 1);
    ASSERT_TRUE(s.getGenerals().front() == &g);
    ASSERT_TRUE(s.getGeneralizations().size() == 1);
    Generalization* r = s.getGeneralizations().front();
    ASSERT_TRUE(r->getSpecific() == &s);
    ASSERT_TRUE(r->getGeneral() == &g);
    ASSERT_TRUE(r->getTargets().size() == 1);
    ASSERT_TRUE(r->getTargets().front() == &g);
    ASSERT_TRUE(r->getSources().size() == 1);
    ASSERT_TRUE(r->getSources().front() == &s);
    ASSERT_TRUE(r->getRelatedElements().size() == 2);
    ASSERT_TRUE(r->getRelatedElements().count(s.getID()) == 1);
    ASSERT_TRUE(r->getRelatedElements().count(g.getID()) == 1);
    ASSERT_TRUE(s.getRelationships().size() == 1);
    ASSERT_TRUE(s.getRelationships().front() == r);
    ASSERT_TRUE(g.getRelationships().size() == 1);
    ASSERT_TRUE(g.getRelationships().front() == r);
    ASSERT_TRUE(s.getGeneralizations().size() == 1);
    ASSERT_TRUE(s.getGeneralizations().front() == r);
}

TEST_F(GeneralizationTest, AddGeneralizationFunctorTest) {
    Classifier s;
    Classifier g;
    Generalization r;
    s.getGeneralizations().add(r);
    r.setGeneral(&g);
    ASSERT_TRUE(r.getSpecific() == &s);
}

TEST_F(GeneralizationTest, AddGeneralizationFunctorGeneralSetTest) {
    Classifier s;
    Classifier g;
    Generalization r;
    r.setGeneral(&g);
    s.getGeneralizations().add(r);
    ASSERT_TRUE(r.getSpecific() == &s);
}

TEST_F(GeneralizationTest, CheckGeneralizationFunctorTest) {
    Generalization r;
    Classifier g;
    Classifier s;
    r.setGeneral(&g);
    r.setSpecific(&s);
    ASSERT_THROW(g.getGeneralizations().add(r), InvalidGeneralizationException);
}

TEST_F(GeneralizationTest, resetGeneralTest) {
    Generalization r;
    Classifier g;
    Classifier s;
    Classifier g2;
    r.setGeneral(&g);
    r.setSpecific(&s);
    r.setGeneral(&g2);
    ASSERT_TRUE(g.getRelationships().size() == 0);
    ASSERT_TRUE(g.getGeneralizations().size() == 0);
    ASSERT_TRUE(s.getGenerals().size() == 1);
    ASSERT_TRUE(s.getGenerals().front() == &g2);
    ASSERT_TRUE(s.getGeneralizations().size() == 1);
    ASSERT_TRUE(s.getGeneralizations().front() == &r);
    ASSERT_TRUE(g2.getGeneralizations().size() == 0);
    ASSERT_TRUE(g2.getRelationships().size() == 1);
    ASSERT_TRUE(g2.getRelationships().front() == &r);
    ASSERT_TRUE(r.getRelatedElements().size() == 2);
    ASSERT_TRUE(r.getRelatedElements().front() == &s);
    ASSERT_TRUE(r.getRelatedElements().back() == &g2);
    ASSERT_TRUE(r.getTargets().size() == 1);
    ASSERT_TRUE(r.getTargets().front() == &g2);
    ASSERT_TRUE(r.getSources().size() == 1);
    ASSERT_TRUE(r.getSources().front() == &s);
}

TEST_F(GeneralizationTest, resetSpecificTest) {
    Generalization r;
    Classifier g;
    Classifier s;
    Classifier s2;
    r.setGeneral(&g);
    r.setSpecific(&s);
    r.setSpecific(&s2);
    ASSERT_TRUE(s.getGeneralizations().size() == 0);
    ASSERT_TRUE(s.getRelationships().size() == 0);
    ASSERT_TRUE(g.getRelationships().size() == 1);
    ASSERT_TRUE(g.getRelationships().front() == &r);
    ASSERT_TRUE(g.getGeneralizations().size() == 0);
    ASSERT_TRUE(s2.getGeneralizations().size() == 1);
    ASSERT_TRUE(s2.getGeneralizations().front() == &r);
    ASSERT_TRUE(s2.getRelationships().size() == 1);
    ASSERT_TRUE(s2.getRelationships().front() == &r);
    ASSERT_TRUE(r.getRelatedElements().size() == 2);
    ASSERT_TRUE(r.getRelatedElements().front() == &g);
    ASSERT_TRUE(r.getRelatedElements().back() == &s2);
    ASSERT_TRUE(r.getTargets().size() == 1);
    ASSERT_TRUE(r.getTargets().front() == &g);
    ASSERT_TRUE(r.getSources().size() == 1);
    ASSERT_TRUE(r.getSources().front() == &s2);
    ASSERT_TRUE(r.getGeneral() == &g);
    ASSERT_TRUE(r.getSpecific() == &s2);
}

TEST_F(GeneralizationTest, backwardsResetGeneralTest) {
    Generalization r;
    Classifier g;
    Classifier s;
    Classifier g2;
    r.setSpecific(&s);
    r.setGeneral(&g);
    r.setGeneral(&g2);
    ASSERT_TRUE(g.getRelationships().size() == 0);
    ASSERT_TRUE(g.getGeneralizations().size() == 0);
    ASSERT_TRUE(s.getGenerals().size() == 1);
    ASSERT_TRUE(s.getGenerals().front() == &g2);
    ASSERT_TRUE(s.getGeneralizations().size() == 1);
    ASSERT_TRUE(s.getGeneralizations().front() == &r);
    ASSERT_TRUE(g2.getGeneralizations().size() == 0);
    ASSERT_TRUE(g2.getRelationships().size() == 1);
    ASSERT_TRUE(g2.getRelationships().front() == &r);
    ASSERT_TRUE(r.getRelatedElements().size() == 2);
    ASSERT_TRUE(r.getRelatedElements().front() == &s);
    ASSERT_TRUE(r.getRelatedElements().back() == &g2);
    ASSERT_TRUE(r.getTargets().size() == 1);
    ASSERT_TRUE(r.getTargets().front() == &g2);
    ASSERT_TRUE(r.getSources().size() == 1);
    ASSERT_TRUE(r.getSources().front() == &s);
}

TEST_F(GeneralizationTest, backwardsResetSpecificTest) {
    Generalization r;
    Classifier g;
    Classifier s;
    Classifier s2;
    r.setSpecific(&s);
    r.setGeneral(&g);
    r.setSpecific(&s2);
    ASSERT_TRUE(s.getGeneralizations().size() == 0);
    ASSERT_TRUE(s.getRelationships().size() == 0);
    ASSERT_TRUE(g.getRelationships().size() == 1);
    ASSERT_TRUE(g.getRelationships().front() == &r);
    ASSERT_TRUE(g.getGeneralizations().size() == 0);
    ASSERT_TRUE(s2.getGeneralizations().size() == 1);
    ASSERT_TRUE(s2.getGeneralizations().front() == &r);
    ASSERT_TRUE(s2.getRelationships().size() == 1);
    ASSERT_TRUE(s2.getRelationships().front() == &r);
    ASSERT_TRUE(r.getRelatedElements().size() == 2);
    ASSERT_TRUE(r.getRelatedElements().front() == &g);
    ASSERT_TRUE(r.getRelatedElements().back() == &s2);
    ASSERT_TRUE(r.getTargets().size() == 1);
    ASSERT_TRUE(r.getTargets().front() == &g);
    ASSERT_TRUE(r.getSources().size() == 1);
    ASSERT_TRUE(r.getSources().front() == &s2);
    ASSERT_TRUE(r.getGeneral() == &g);
    ASSERT_TRUE(r.getSpecific() == &s2);
}

TEST_F(GeneralizationTest, ResetSpecificByGeneralTest) {
    Classifier g;
    Classifier s;
    Classifier s2;
    s.getGenerals().add(g);
    Generalization* r = s.getGeneralizations().front();
    r->setSpecific(&s2);
    ASSERT_TRUE(s.getGeneralizations().size() == 0);
    ASSERT_TRUE(s.getRelationships().size() == 0);
    ASSERT_TRUE(g.getRelationships().size() == 1);
    ASSERT_TRUE(g.getRelationships().front() == r);
    ASSERT_TRUE(g.getGeneralizations().size() == 0);
    ASSERT_TRUE(s2.getGeneralizations().size() == 1);
    ASSERT_TRUE(s2.getGeneralizations().front() == r);
    ASSERT_TRUE(s2.getRelationships().size() == 1);
    ASSERT_TRUE(s2.getRelationships().front() == r);
    ASSERT_TRUE(r->getRelatedElements().size() == 2);
    ASSERT_TRUE(r->getRelatedElements().front() == &g);
    ASSERT_TRUE(r->getRelatedElements().back() == &s2);
    ASSERT_TRUE(r->getTargets().size() == 1);
    ASSERT_TRUE(r->getTargets().front() == &g);
    ASSERT_TRUE(r->getSources().size() == 1);
    ASSERT_TRUE(r->getSources().front() == &s2);
    ASSERT_TRUE(r->getGeneral() == &g);
    ASSERT_TRUE(r->getSpecific() == &s2);
}