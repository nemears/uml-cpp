#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/instanceSpecification.h"
#include "uml/package.h"
#include "uml/class.h"

using namespace std;
using namespace UML;

class InstanceSpecificationParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(InstanceSpecificationParserTest, forwardClassifierTest) {
    Element* el;
    ASSERT_NO_THROW(el = Parsers::parse(YAML::LoadFile(ymlPath + "instanceSpecificationTests/forwardClassifier.yml")));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().front()->getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(pckg->getPackagedElements().front());
    ASSERT_TRUE(pckg->getPackagedElements().back()->getElementType() == ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification i = *dynamic_cast<InstanceSpecification*>(pckg->getPackagedElements().back());
    ASSERT_TRUE(i.getClassifier() == c);
}

TEST_F(InstanceSpecificationParserTest, backwardsClassifierTest) {
    Element* el;
    ASSERT_NO_THROW(el = Parsers::parse(YAML::LoadFile(ymlPath + "instanceSpecificationTests/backwardsClassifier.yml")));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().front()->getElementType() == ElementType::INSTANCE_SPECIFICATION);
    ASSERT_TRUE(pckg->getPackagedElements().back()->getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(pckg->getPackagedElements().back());
    InstanceSpecification i = *dynamic_cast<InstanceSpecification*>(pckg->getPackagedElements().front());
    ASSERT_TRUE(i.getClassifier() == c);
}

// TODO slot tests

TEST_F(InstanceSpecificationParserTest, basicSlotTest) {
    Element* el;
    ASSERT_NO_THROW(el = Parsers::parse(YAML::LoadFile(ymlPath + "instanceSpecificationTests/basicSlot.yml")));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().back()->getElementType() == ElementType::INSTANCE_SPECIFICATION);
    ASSERT_TRUE(pckg->getPackagedElements().front()->getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(pckg->getPackagedElements().front());
    InstanceSpecification i = *dynamic_cast<InstanceSpecification*>(pckg->getPackagedElements().back());
    ASSERT_TRUE(i.getClassifier() == c);
    ASSERT_TRUE(i.getSlots().size() == 1);
    ASSERT_TRUE(c->getOwnedAttributes().size() == 1);
    Slot* s = i.getSlots().front();
    Property* p = c->getOwnedAttributes().front();
    ASSERT_TRUE(s->getDefiningFeature() == p);
    ASSERT_TRUE(s->getOwningInstance() == pckg->getPackagedElements().back());
}

TEST_F(InstanceSpecificationParserTest, backwardsSlotTest) {
    Element* el;
    ASSERT_NO_THROW(el = Parsers::parse(YAML::LoadFile(ymlPath + "instanceSpecificationTests/backwardsSlot.yml")));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().front()->getElementType() == ElementType::INSTANCE_SPECIFICATION);
    ASSERT_TRUE(pckg->getPackagedElements().back()->getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(pckg->getPackagedElements().back());
    InstanceSpecification i = *dynamic_cast<InstanceSpecification*>(pckg->getPackagedElements().front());
    ASSERT_TRUE(i.getClassifier() == c);
    ASSERT_TRUE(i.getSlots().size() == 1);
    ASSERT_TRUE(c->getOwnedAttributes().size() == 1);
    Slot* s = i.getSlots().front();
    Property* p = c->getOwnedAttributes().front();
    ASSERT_TRUE(s->getDefiningFeature() == p);
    ASSERT_TRUE(s->getOwningInstance() == pckg->getPackagedElements().front());
}