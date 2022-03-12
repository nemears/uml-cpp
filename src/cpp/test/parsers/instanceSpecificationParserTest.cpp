#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/uml-cpp-paths.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

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
    ASSERT_NO_THROW(el = m.parse(ymlPath + "instanceSpecificationTests/forwardClassifier.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(&pckg->getPackagedElements().front());
    ASSERT_TRUE(pckg->getPackagedElements().back().getElementType() == ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification* i = dynamic_cast<InstanceSpecification*>(&pckg->getPackagedElements().back());
    ASSERT_EQ(i->getClassifiers().size(), 1);
    ASSERT_EQ(i->getClassifiers().front(), *c);
}

TEST_F(InstanceSpecificationParserTest, backwardsClassifierTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "instanceSpecificationTests/backwardsClassifier.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::INSTANCE_SPECIFICATION);
    ASSERT_TRUE(pckg->getPackagedElements().back().getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(&pckg->getPackagedElements().back());
    InstanceSpecification* i = dynamic_cast<InstanceSpecification*>(&pckg->getPackagedElements().front());
    ASSERT_EQ(i->getClassifiers().size(), 1);
    ASSERT_EQ(i->getClassifiers().front(), *c);
}

TEST_F(InstanceSpecificationParserTest, basicSlotTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "instanceSpecificationTests/basicSlot.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().back().getElementType() == ElementType::INSTANCE_SPECIFICATION);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(&pckg->getPackagedElements().front());
    InstanceSpecification& i = *dynamic_cast<InstanceSpecification*>(&pckg->getPackagedElements().back());
    ASSERT_EQ(i.getClassifiers().size(), 1);
    ASSERT_EQ(i.getClassifiers().front(), *c);
    ASSERT_TRUE(i.getSlots().size() == 1);
    ASSERT_TRUE(c->getOwnedAttributes().size() == 1);
    Slot* s = &i.getSlots().front();
    Property* p = &c->getOwnedAttributes().front();
    ASSERT_TRUE(s->getDefiningFeature() == p);
    ASSERT_TRUE(*s->getOwningInstance() == pckg->getPackagedElements().back());
}

TEST_F(InstanceSpecificationParserTest, backwardsSlotTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "instanceSpecificationTests/backwardsSlot.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::INSTANCE_SPECIFICATION);
    ASSERT_TRUE(pckg->getPackagedElements().back().getElementType() == ElementType::CLASS);
    Class* c = dynamic_cast<Class*>(&pckg->getPackagedElements().back());
    InstanceSpecification& i = *dynamic_cast<InstanceSpecification*>(&pckg->getPackagedElements().front());
    ASSERT_EQ(i.getClassifiers().size(), 1);
    ASSERT_EQ(i.getClassifiers().front(), *c);
    ASSERT_TRUE(i.getSlots().size() == 1);
    ASSERT_TRUE(c->getOwnedAttributes().size() == 1);
    Slot* s = &i.getSlots().front();
    Property* p = &c->getOwnedAttributes().front();
    ASSERT_TRUE(s->getDefiningFeature() == p);
    ASSERT_TRUE(*s->getOwningInstance() == pckg->getPackagedElements().front());
}

TEST_F(InstanceSpecificationParserTest, instanceValueSlot) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "instanceSpecificationTests/instanceSlot.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 4);
    ASSERT_TRUE(pckg->getPackagedElements().get("clazz1").getElementType() == ElementType::CLASS);
    Class* c1 = dynamic_cast<Class*>(&pckg->getPackagedElements().get("clazz1"));
    ASSERT_TRUE(c1->getName().compare("clazz1") == 0);
    ASSERT_TRUE(pckg->getPackagedElements().get("clazz2").getElementType() == ElementType::CLASS);
    Class* c2 = dynamic_cast<Class*>(&pckg->getPackagedElements().get("clazz2"));
    ASSERT_TRUE(c2->getName().compare("clazz2") == 0);
    ASSERT_TRUE(c2->getOwnedAttributes().size() == 1);
    Property* p = &c2->getOwnedAttributes().front();
    ASSERT_TRUE(p->getType() == c1);
    ASSERT_TRUE(pckg->getPackagedElements().get(ID::fromString("x2zUOks6utCje0z&ID4EOM91HJuj")).getElementType() == ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification* i1 = dynamic_cast<InstanceSpecification*>(&pckg->getPackagedElements().get(ID::fromString("x2zUOks6utCje0z&ID4EOM91HJuj")));
    ASSERT_EQ(i1->getClassifiers().size(), 1);
    ASSERT_EQ(i1->getClassifiers().front(), *c1);
    ASSERT_TRUE(pckg->getPackagedElements().get(ID::fromString("6CB8GbwIp4Ii0rKfx4YSVRoiZvmc")).getElementType() == ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification* i2 = dynamic_cast<InstanceSpecification*>(&pckg->getPackagedElements().get(ID::fromString("6CB8GbwIp4Ii0rKfx4YSVRoiZvmc")));
    ASSERT_EQ(i2->getClassifiers().size(), 1);
    ASSERT_EQ(i2->getClassifiers().front(), *c2);
    ASSERT_TRUE(i2->getSlots().size() == 1);
    Slot* s = &i2->getSlots().front();
    ASSERT_TRUE(s->getDefiningFeature() == p);
    ASSERT_TRUE(s->getValues().size() == 1);
    ASSERT_TRUE(s->getValues().front().getElementType() == ElementType::INSTANCE_VALUE);
    InstanceValue* v = dynamic_cast<InstanceValue*>(&s->getValues().front());
    ASSERT_TRUE(v->getInstance() == i1);
}

TEST_F(InstanceSpecificationParserTest, simpleInstanceEmitTest) {
    UmlManager m;
    InstanceSpecification& inst = *m.create<InstanceSpecification>();
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
    UmlManager m;
    InstanceSpecification& inst = *m.create<InstanceSpecification>();
    inst.setID("yaogA9yjaFoD_RdGQzRrwe1826Aj");
    inst.setName("slot");
    inst.setVisibility(VisibilityKind::PROTECTED);
    Slot& s = *m.create<Slot>();
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
    ASSERT_NO_THROW(el = m.parse(ymlPath + "instanceSpecificationTests/specification.yml").ptr());
    ASSERT_EQ(el->getElementType(), ElementType::INSTANCE_SPECIFICATION);
    InstanceSpecification& inst = el->as<InstanceSpecification>();
    ASSERT_TRUE(inst.getSpecification());
    ASSERT_EQ(inst.getSpecification()->getElementType(), ElementType::LITERAL_STRING);
    ASSERT_EQ(inst.getSpecification()->as<LiteralString>().getValue(), "booga");
}

TEST_F(InstanceSpecificationParserTest, emitSpecificationTest) {
    UmlManager m;
    InstanceSpecification& inst = *m.create<InstanceSpecification>();
    LiteralString& str = *m.create<LiteralString>();
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

void ASSERT_RESTORE_SLOT_CORRECTLY(InstanceSpecification& inst, Slot& slot, size_t i) {
    ASSERT_EQ(inst.getSlots().size(), 1);
    ASSERT_EQ(inst.getSlots().get(i), slot);
    ASSERT_EQ(inst.getOwnedElements().size(), 1);
    ASSERT_TRUE(inst.getOwnedElements().contains(slot));
}

TEST_F(InstanceSpecificationParserTest, mountAndEditInstanceTest) {
    UmlManager m;
    Package& root = *m.create<Package>();
    Class& classifier = *m.create<Class>();
    Property& attribute = *m.create<Property>();
    InstanceSpecification& inst = *m.create<InstanceSpecification>();
    Slot& slot = *m.create<Slot>();
    PrimitiveType& type = *m.create<PrimitiveType>();
    InstanceSpecification& typeInst = *m.create<InstanceSpecification>();
    LiteralString& typeSpecification = *m.create<LiteralString>();
    InstanceValue& slotVal = *m.create<InstanceValue>();
    attribute.setType(type);
    classifier.getOwnedAttributes().add(attribute);
    slot.setDefiningFeature(attribute);
    typeInst.getClassifiers().add(type);
    typeSpecification.setValue("test");
    typeInst.setSpecification(typeSpecification);
    slotVal.setInstance(typeInst);
    slot.getValues().add(slotVal);
    inst.getClassifiers().add(classifier);
    inst.getSlots().add(slot);
    root.getPackagedElements().add(classifier, inst, type, typeInst);
    m.setRoot(&root);
    m.mount(ymlPath + "instanceSpecificationTests");

    ID instID = inst.getID();
    m.release(inst);
    InstanceSpecification& inst2 = *slot.getOwningInstance();
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(inst2, root));
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORE_SLOT_CORRECTLY(inst2, slot, 0));
    ASSERT_FALSE(inst2.getClassifiers().empty());
    ASSERT_EQ(inst2.getClassifiers().front(), classifier);

    m.release(classifier);
    ASSERT_FALSE(inst2.getClassifiers().empty());
    Class& classifier2 = inst2.getClassifiers().front().as<Class>();
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(classifier2, root));

    ID classifierID = classifier2.getID();
    m.release(inst2, classifier2);
    InstanceSpecification& inst3 = m.aquire(instID)->as<InstanceSpecification>();
    ASSERT_FALSE(inst3.getClassifiers().empty());
    ASSERT_FALSE(m.loaded(classifierID));
    Class& classifier3 = m.aquire(classifierID)->as<Class>();
    ASSERT_EQ(inst3.getClassifiers().front(), classifier3);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(inst3, root));
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORE_SLOT_CORRECTLY(inst3, slot, 0));
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(classifier3, root));

    ID slotID = slot.getID();
    m.release(slot);
    Slot& slot2 = m.aquire(slotID)->as<Slot>();
    ASSERT_TRUE(slot2.getOwningInstance());
    ASSERT_EQ(*slot2.getOwningInstance(), inst3);
    ASSERT_TRUE(slot2.getOwner());
    ASSERT_EQ(*slot2.getOwner(), inst3);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORE_SLOT_CORRECTLY(inst3, slot2, 0));
    ASSERT_TRUE(slot2.getDefiningFeature());
    ASSERT_EQ(*slot2.getDefiningFeature(), attribute);

    m.release(slot2, inst3);
    Slot& slot3 = m.aquire(slotID)->as<Slot>();
    ASSERT_TRUE(slot3.getOwningInstance());
    InstanceSpecification& inst4 = m.aquire(instID)->as<InstanceSpecification>();
    ASSERT_EQ(*slot3.getOwningInstance(), inst4);
    ASSERT_TRUE(slot3.getOwner());
    ASSERT_EQ(*slot3.getOwner(), inst4);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORE_SLOT_CORRECTLY(inst4, slot3, 0));
    ASSERT_TRUE(slot3.getDefiningFeature());
    ASSERT_EQ(*slot3.getDefiningFeature(), attribute);
    ASSERT_EQ(slot3.getValues().size(), 1);
    ASSERT_EQ(slot3.getValues().front(), slotVal);
    ASSERT_EQ(slot3.getOwnedElements().size(), 1);
    ASSERT_EQ(*slot3.getOwnedElements().begin(), slotVal);

    m.release(slot3, inst4);
    InstanceSpecification& inst5 = m.aquire(instID)->as<InstanceSpecification>();
    Slot& slot4 = m.aquire(slotID)->as<Slot>();
    ASSERT_EQ(*slot4.getOwningInstance(), inst5);
    ASSERT_TRUE(slot4.getOwner());
    ASSERT_EQ(*slot4.getOwner(), inst5);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORE_SLOT_CORRECTLY(inst5, slot4, 0));
    ASSERT_TRUE(slot4.getDefiningFeature());
    ASSERT_EQ(*slot4.getDefiningFeature(), attribute);
    ASSERT_EQ(slot4.getOwnedElements().size(), 1);
    ASSERT_EQ(*slot4.getOwnedElements().begin(), slotVal);

    ID propID = attribute.getID();
    m.release(slot4, attribute);
    Slot& slot5 = m.aquire(slotID)->as<Slot>();
    ASSERT_FALSE(m.loaded(propID));
    Property& attribute2 = m.aquire(propID)->as<Property>();
    ASSERT_TRUE(slot5.getDefiningFeature());
    ASSERT_EQ(*slot5.getDefiningFeature(), attribute2);
    ASSERT_EQ(slot5.getOwnedElements().size(), 1);
    ASSERT_EQ(*slot5.getOwnedElements().begin(), slotVal);

    ID valID = slotVal.getID();
    m.release(slotVal, slot5);
    Slot& slot6 = m.aquire(slotID)->as<Slot>();
    ASSERT_FALSE(m.loaded(valID));
    InstanceValue& slotVal2 = m.aquire(valID)->as<InstanceValue>();
    ASSERT_EQ(slot6.getOwnedElements().size(), 1);
    ASSERT_EQ(*slot6.getOwnedElements().begin(), slotVal2);
    ASSERT_TRUE(slotVal2.getOwner());
    ASSERT_EQ(*slotVal2.getOwner(), slot6);
    ASSERT_TRUE(slotVal2.getInstance());
    ASSERT_EQ(*slotVal2.getInstance(), typeInst);

    ID typeInstID = typeInst.getID();
    m.release(slotVal2, typeInst);
    InstanceValue& slotVal3 = m.aquire(valID)->as<InstanceValue>();
    ASSERT_FALSE(m.loaded(typeInstID));
    InstanceSpecification& typeInst2 = m.aquire(typeInstID)->as<InstanceSpecification>();
    ASSERT_TRUE(slotVal3.getInstance());
    ASSERT_EQ(*slotVal3.getInstance(), typeInst2);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(typeInst2, root));
    ASSERT_FALSE(typeInst2.getClassifiers().empty());
    ASSERT_EQ(typeInst2.getClassifiers().front(), type);
    ASSERT_TRUE(typeInst2.getSpecification());
    ASSERT_EQ(*typeInst2.getSpecification(), typeSpecification);
    ASSERT_EQ(typeInst2.getOwnedElements().size(), 1);
    ASSERT_EQ(*typeInst2.getOwnedElements().begin(), typeSpecification);

    ID specID = typeSpecification.getID();
    m.release(typeInst2, typeSpecification);
    InstanceSpecification& typeInst3 = m.aquire(typeInstID)->as<InstanceSpecification>();
    ASSERT_FALSE(m.loaded(specID));
    LiteralString& typeSpecification2 = m.aquire(specID)->as<LiteralString>();
    ASSERT_TRUE(typeSpecification2.getOwner());
    ASSERT_EQ(*typeSpecification2.getOwner(), typeInst3);
    ASSERT_TRUE(typeInst3.getSpecification());
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(typeInst3, root));
    ASSERT_FALSE(typeInst3.getClassifiers().empty());
    ASSERT_EQ(typeInst3.getClassifiers().front(), type);
    ASSERT_EQ(*typeInst3.getSpecification(), typeSpecification2);
    ASSERT_EQ(typeInst3.getOwnedElements().size(), 1);
    ASSERT_EQ(*typeInst3.getOwnedElements().begin(), typeSpecification2);
}