#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/association.h"

using namespace std;
using namespace UML;

class AssociationParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(AssociationParserTest, parseNavigableOwnedEndAndMemberEndTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "associationTests/navigableEnd.yml"));
    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package& pckg = m.get<Package>(el->getID());
    ASSERT_EQ(pckg.getPackageMerge().size(), 1);
    PackageMerge& merge = *pckg.getPackageMerge().front();
    // TODO?

    ASSERT_EQ(pckg.getPackagedElements().size(), 3);
    ASSERT_EQ(pckg.getPackagedElements().front()->getElementType(), ElementType::CLASS);
    Class& type = m.get<Class>(pckg.getPackagedElements().front()->getID());
    ASSERT_EQ(pckg.getPackagedElements().get(1)->getElementType(), ElementType::CLASS);
    Class& c = *dynamic_cast<Class*>(pckg.getPackagedElements().get(1));
    ASSERT_EQ(c.getOwnedAttributes().size(), 1);
    Property& p = *c.getOwnedAttributes().front();
    ASSERT_TRUE(p.getType() != 0);
    ASSERT_EQ(p.getType()->getID(), type.getID());
    ASSERT_EQ(pckg.getPackagedElements().back()->getElementType(), ElementType::ASSOCIATION);
    Association& ass = dynamic_cast<Association&>(*pckg.getPackagedElements().back());
    ASSERT_EQ(ass.getMemberEnds().size(), 2);
    ASSERT_EQ(ass.getMemberEnds().back()->getID(), p.getID());
    ASSERT_EQ(ass.getNavigableOwnedEnds().size(), 1);
    Property& ne = *ass.getNavigableOwnedEnds().front();
    ASSERT_TRUE(ne.getType() != 0);
    ASSERT_EQ(ne.getType()->getID(), ID::fromString("real_aZG&w6yl61bXVWutgeyScN9"));
}

TEST_F(AssociationParserTest, parseOwnedEndAndMemberEndTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "associationTests/ownedEnd.yml"));
    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package& pckg = m.get<Package>(el->getID());
    ASSERT_EQ(pckg.getPackageMerge().size(), 1);
    PackageMerge& merge = *pckg.getPackageMerge().front();
    // TODO?

    ASSERT_EQ(pckg.getPackagedElements().size(), 3);
    ASSERT_EQ(pckg.getPackagedElements().front()->getElementType(), ElementType::CLASS);
    Class& type = m.get<Class>(pckg.getPackagedElements().front()->getID());
    ASSERT_EQ(pckg.getPackagedElements().back()->getElementType(), ElementType::CLASS);
    Class& c = *dynamic_cast<Class*>(pckg.getPackagedElements().back());
    ASSERT_EQ(c.getOwnedAttributes().size(), 1);
    Property& p = *c.getOwnedAttributes().front();
    ASSERT_TRUE(p.getType() != 0);
    ASSERT_EQ(p.getType()->getID(), type.getID());
    ASSERT_EQ(pckg.getPackagedElements().get(1)->getElementType(), ElementType::ASSOCIATION);
    Association& ass = dynamic_cast<Association&>(*pckg.getPackagedElements().get(1));
    ASSERT_EQ(ass.getMemberEnds().size(), 2);
    ASSERT_EQ(ass.getMemberEnds().back()->getID(), p.getID());
    ASSERT_EQ(ass.getOwnedEnds().size(), 1);
    Property& ne = *ass.getOwnedEnds().front();
    ASSERT_TRUE(ne.getType() != 0);
    ASSERT_EQ(ne.getType()->getID(), ID::fromString("real_aZG&w6yl61bXVWutgeyScN9"));
}