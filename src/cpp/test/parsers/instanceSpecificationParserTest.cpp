#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/instanceSpecification.h"
#include "uml/package.h"
#include "uml/class.h"
#include "uml/slot.h"
#include "uml/property.h"
#include "uml/instanceValue.h"
#include "uml/literalString.h"

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
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "instanceSpecificationTests/forwardClassifier.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(&pckg->getPackagedElements().front());
    ASSERT_TRUE(pckg->getPackagedElements().back().getElementType() == ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification* i = dynamic_cast<InstanceSpecification*>(&pckg->getPackagedElements().back());
    ASSERT_TRUE(i->getClassifier() == c);
}

TEST_F(InstanceSpecificationParserTest, backwardsClassifierTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "instanceSpecificationTests/backwardsClassifier.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::INSTANCE_SPECIFICATION);
    ASSERT_TRUE(pckg->getPackagedElements().back().getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(&pckg->getPackagedElements().back());
    InstanceSpecification* i = dynamic_cast<InstanceSpecification*>(&pckg->getPackagedElements().front());
    ASSERT_TRUE(i->getClassifier() == c);
}

TEST_F(InstanceSpecificationParserTest, basicSlotTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "instanceSpecificationTests/basicSlot.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().back().getElementType() == ElementType::INSTANCE_SPECIFICATION);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(&pckg->getPackagedElements().front());
    InstanceSpecification i = *dynamic_cast<InstanceSpecification*>(&pckg->getPackagedElements().back());
    ASSERT_TRUE(i.getClassifier() == c);
    ASSERT_TRUE(i.getSlots().size() == 1);
    ASSERT_TRUE(c->getOwnedAttributes().size() == 1);
    Slot* s = &i.getSlots().front();
    Property* p = &c->getOwnedAttributes().front();
    ASSERT_TRUE(s->getDefiningFeature() == p);
    ASSERT_TRUE(s->getOwningInstance() == &pckg->getPackagedElements().back());
}

TEST_F(InstanceSpecificationParserTest, backwardsSlotTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "instanceSpecificationTests/backwardsSlot.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::INSTANCE_SPECIFICATION);
    ASSERT_TRUE(pckg->getPackagedElements().back().getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(&pckg->getPackagedElements().back());
    InstanceSpecification i = *dynamic_cast<InstanceSpecification*>(&pckg->getPackagedElements().front());
    ASSERT_TRUE(i.getClassifier() == c);
    ASSERT_TRUE(i.getSlots().size() == 1);
    ASSERT_TRUE(c->getOwnedAttributes().size() == 1);
    Slot* s = &i.getSlots().front();
    Property* p = &c->getOwnedAttributes().front();
    ASSERT_TRUE(s->getDefiningFeature() == p);
    ASSERT_TRUE(s->getOwningInstance() == &pckg->getPackagedElements().front());
}

TEST_F(InstanceSpecificationParserTest, instanceValueSlot) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "instanceSpecificationTests/instanceSlot.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 4);
    ASSERT_TRUE(pckg->getPackagedElements().get(0).getElementType() == ElementType::CLASS);
    Class* c1 = dynamic_cast<Class*>(&pckg->getPackagedElements().get(0));
    ASSERT_TRUE(c1->getName().compare("clazz1") == 0);
    ASSERT_TRUE(pckg->getPackagedElements().get(1).getElementType() == ElementType::CLASS);
    Class* c2 = dynamic_cast<Class*>(&pckg->getPackagedElements().get(1));
    ASSERT_TRUE(c2->getName().compare("clazz2") == 0);
    ASSERT_TRUE(c2->getOwnedAttributes().size() == 1);
    Property* p = &c2->getOwnedAttributes().front();
    ASSERT_TRUE(p->getType() == c1);
    ASSERT_TRUE(pckg->getPackagedElements().get(2).getElementType() == ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification* i1 = dynamic_cast<InstanceSpecification*>(&pckg->getPackagedElements().get(2));
    ASSERT_TRUE(i1->getClassifier() == c1);
    ASSERT_TRUE(pckg->getPackagedElements().get(3).getElementType() == ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification* i2 = dynamic_cast<InstanceSpecification*>(&pckg->getPackagedElements().get(3));
    ASSERT_TRUE(i2->getClassifier() == c2);
    ASSERT_TRUE(i2->getSlots().size() == 1);
    Slot* s = &i2->getSlots().front();
    ASSERT_TRUE(s->getDefiningFeature() == p);
    ASSERT_TRUE(s->getValues().size() == 1);
    ASSERT_TRUE(s->getValues().front().getElementType() == ElementType::INSTANCE_VALUE);
    InstanceValue* v = dynamic_cast<InstanceValue*>(&s->getValues().front());
    ASSERT_TRUE(v->getInstance() == i1);
}

TEST_F(InstanceSpecificationParserTest, simpleInstanceEmitTest) {
    InstanceSpecification inst;
    inst.setID("3XvQFHIrqSmU7WAXA7fVzkw1v2U3");
    inst.setName("simple");
    inst.setVisibility(VisibilityKind::PROTECTED);
    string expectedEmit = R""""(instanceSpecification:
  id: 3XvQFHIrqSmU7WAXA7fVzkw1v2U3
  name: simple
  visibility: PROTECTED)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(inst));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(InstanceSpecificationParserTest, simpleSlotTest) {
    InstanceSpecification inst;
    inst.setID("yaogA9yjaFoD_RdGQzRrwe1826Aj");
    inst.setName("slot");
    inst.setVisibility(VisibilityKind::PROTECTED);
    Slot s;
    s.setID("w6arMVW4Plw0aLOBWLE9_8Xo_UL&");
    inst.getSlots().add(s);
    string expectedEmit = R""""(instanceSpecification:
  id: yaogA9yjaFoD_RdGQzRrwe1826Aj
  name: slot
  visibility: PROTECTED
  slots:
    - slot:
        id: w6arMVW4Plw0aLOBWLE9_8Xo_UL&)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(inst));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(InstanceSpecificationParserTest, parseSpecificationTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "instanceSpecificationTests/specification.yml"));
    ASSERT_EQ(el->getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& inst = el->as<InstanceSpecification>();
    ASSERT_TRUE(inst.getSpecification() != 0);
    ASSERT_EQ(inst.getSpecification()->getElementType(), ElementType::LITERAL_STRING);
    ASSERT_EQ(inst.getSpecification()->as<LiteralString>().getValue(), "booga");
}

TEST_F(InstanceSpecificationParserTest, emitSpecificationTest) {
    UmlManager m;
    InstanceSpecification& inst = m.create<InstanceSpecification>();
    LiteralString& str = m.create<LiteralString>();
    inst.setID("fsU5Fw&5REaNv4NCvC0d4qZnXg4C");
    str.setID("nVzJ8mHx1yrRlct0ot34p7uBaVvC");
    str.setValue("ooga");
    inst.setSpecification(&str);
    string expectedEmit = R""""(instanceSpecification:
  id: fsU5Fw&5REaNv4NCvC0d4qZnXg4C
  specification:
    literalString:
      id: nVzJ8mHx1yrRlct0ot34p7uBaVvC
      value: ooga)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(inst));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}