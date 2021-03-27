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
}