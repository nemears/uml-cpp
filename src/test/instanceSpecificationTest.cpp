#include "uml/types/instanceSpecification.h"
#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

using namespace UML;

UML_SET_INTEGRATION_TEST(InstanceSpecificationSlots, Slot, InstanceSpecification, getSlots)
UML_SINGLETON_INTEGRATION_TEST(InstanceSpecificationSpecification, LiteralUnlimitedNatural, InstanceSpecification, getSpecificationSingleton)
UML_SET_INTEGRATION_TEST(InstanceSpecificationClassifiers, Class, InstanceSpecification, getClassifiers)
UML_SINGLETON_INTEGRATION_TEST(InstanceValueInstance, InstanceSpecification, InstanceValue, getInstanceSingleton)

class InstanceSpecificationTest : public ::testing::Test {
    public:
        std::string ymlPath;
    void SetUp() override {
        ymlPath = "../examples/";
    };
};

TEST_F(InstanceSpecificationTest, setClassifierAsClass) {
    UmlManager m;
    auto& c = *m.create<Class>();
    auto& i = *m.create<InstanceSpecification>();
    ASSERT_NO_THROW(i.getClassifiers().add(c));
    EXPECT_TRUE(i.getClassifiers().front()->getID() == c.getID());
}

TEST_F(InstanceSpecificationTest, setStringValueSlots) {
    UmlManager m;
    auto& c = *m.create<Class>();
    auto& stringP = *m.create<Property>();
    auto& stringPrim = *m.create<PrimitiveType>();
    stringP.setType(stringPrim);
    c.getOwnedAttributes().add(stringP);
    auto& ls = *m.create<LiteralString>();
    ls.setValue("test");
    auto& stringSlot = *m.create<Slot>();
    stringSlot.setDefiningFeature(&stringP);
    stringSlot.getValues().add(ls);
    auto& i = *m.create<InstanceSpecification>();
    i.getSlots().add(stringSlot);
    ASSERT_TRUE(i.getSlots().front()->getID() == stringSlot.getID());
    ASSERT_TRUE(i.getSlots().front()->getDefiningFeature()->getID() == stringP.getID());
    ASSERT_TRUE(i.getSlots().front()->getValues().front()->getID() == ls.getID());
}

TEST_F(InstanceSpecificationTest, setSlotAsInstanceValue) {
    UmlManager m;
    auto& c = *m.create<Class>();
    c.setName("typeA");
    auto& b = *m.create<Class>();
    b.setName("typeB");
    auto& bProp = *m.create<Property>();
    bProp.setName("b");
    bProp.setType(b);
    c.getOwnedAttributes().add(bProp);
    auto& bInst = *m.create<InstanceSpecification>();
    bInst.getClassifiers().add(b);
    auto& aInst = *m.create<InstanceSpecification>();
    aInst.getClassifiers().add(c);
    auto& bVal = *m.create<InstanceValue>();
    bVal.setInstance(&bInst);
    auto& aSlot = *m.create<Slot>();
    aSlot.setDefiningFeature(&bProp);
    aSlot.getValues().add(bVal);
    aInst.getSlots().add(aSlot);
    ASSERT_TRUE(aInst.getSlots().front()->getDefiningFeature()->getID() == bProp.getID());
}

// // TODO add throw for pushing slots that don't correspond structural feature


TEST_F(InstanceSpecificationTest, addSlotFunctorTest) {
    UmlManager m;
    auto& i = *m.create<InstanceSpecification>();
    auto& s = *m.create<Slot>();
    i.getSlots().add(s);
    ASSERT_EQ(i.getSlots().get(s.getID()), &s);
    ASSERT_EQ(i.getSlots().size(), 1);
    ASSERT_EQ(i.getOwnedElements().get(s.getID()), &s);
    ASSERT_EQ(i.getOwnedElements().size(), 1);
    ASSERT_TRUE(s.getOwningInstance());
    ASSERT_EQ(*s.getOwningInstance(), i);
}

TEST_F(InstanceSpecificationTest, SetOwningInstanceFunctionalityTest) {
    UmlManager m;
    auto& i = *m.create<InstanceSpecification>();
    auto& s = *m.create<Slot>();
    s.setOwningInstance(i);
    ASSERT_EQ(i.getSlots().get(s.getID()), &s);
    ASSERT_EQ(i.getSlots().size(), 1);
    ASSERT_EQ(i.getOwnedElements().get(s.getID()), &s);
    ASSERT_EQ(i.getOwnedElements().size(), 1);
    ASSERT_TRUE(s.getOwningInstance());
    ASSERT_EQ(*s.getOwningInstance(), i);
}

TEST_F(InstanceSpecificationTest, removeSlotFunctorTest) {
    UmlManager m;
    auto& i = *m.create<InstanceSpecification>();
    auto& s = *m.create<Slot>();
    s.setOwningInstance(&i);
    ASSERT_NO_THROW(i.getSlots().remove(s));
    ASSERT_TRUE(i.getSlots().size() == 0);
    ASSERT_TRUE(i.getOwnedElements().size() == 0);
    ASSERT_TRUE(!s.getOwningInstance());
    ASSERT_TRUE(!s.getOwner());
}

TEST_F(InstanceSpecificationTest, setAndRemoveSpecificationTest) {
    UmlManager m;
    auto& inst = *m.create<InstanceSpecification>();
    auto& str = *m.create<LiteralString>();
    inst.setSpecification(&str);
    ASSERT_TRUE(inst.getSpecification());
    ASSERT_EQ(inst.getSpecification()->getID(), str.getID());
    ASSERT_EQ(inst.getOwnedElements().size(), 1);
    ASSERT_EQ(inst.getOwnedElements().get(str.getID())->getID(), str.getID()); //lol
    ASSERT_TRUE(str.getOwner());
    ASSERT_EQ(str.getOwner()->getID(), inst.getID());
    inst.setSpecification(0);
    ASSERT_FALSE(inst.getSpecification());
    ASSERT_EQ(inst.getOwnedElements().size(), 0);
    ASSERT_FALSE(str.getOwner());    
}

// TEST_F(InstanceSpecificationTest, reindexClassifierTest) {
//     UmlManager m;
//     Class& c = *m.create<Class>();
//     InstanceSpecification& i = *m.create<InstanceSpecification>();
//     Package& root = *m.create<Package>();
//     m.setRoot(&root);
//     root.getPackagedElements().add(c, i);
//     i.getClassifiers().add(c);
//     m.mount(ymlPath + "instanceSpecificationTests");
//     ID id = ID::fromString("9nU_h2_riMLlgcg2FzNiGyvtbew3");
//     c.setID(id);
//     ASSERT_FALSE(i.getClassifiers().empty());
//     ASSERT_EQ(i.getClassifiers().size(), 1);
//     ASSERT_EQ(c.getID(), i.getClassifiers().front().getID());
//     m.release(c);
//     ASSERT_TRUE(i.hasClassifier());
//     ASSERT_TRUE(i.getClassifier());
//     ASSERT_EQ(i.getClassifierRef().getID(), id);
// }

TEST_F(InstanceSpecificationTest, forwardClassifierTest) {
    UmlManager::Pointer<Element> el = 0;
    UmlManager m;
    ASSERT_NO_THROW(m.open(ymlPath + "instanceSpecificationTests/forwardClassifier.yml"));
    el = m.getRoot();
    ASSERT_EQ(el->getElementType(), UmlManager::ElementType<Package>::result);
    UmlManager::Pointer<Package> pckg = el;
    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().get(EGM::ID::fromString("W2gUELk0mKh-Kqc5fjFCOuppln2b"))->getElementType() == UmlManager::ElementType<Class>::result);
    auto c = pckg->getPackagedElements().get(EGM::ID::fromString("W2gUELk0mKh-Kqc5fjFCOuppln2b"));
    ASSERT_TRUE(pckg->getPackagedElements().get(EGM::ID::fromString("8SXVdBHWl9N6o6Uut3WImOPk6Ngw"))->getElementType() == UmlManager::ElementType<InstanceSpecification>::result);
    UmlManager::Pointer<InstanceSpecification> i = pckg->getPackagedElements().get(EGM::ID::fromString("8SXVdBHWl9N6o6Uut3WImOPk6Ngw"));
    ASSERT_EQ(i->getClassifiers().size(), 1);
    ASSERT_EQ(i->getClassifiers().front(), c);
}

TEST_F(InstanceSpecificationTest, backwardsClassifierTest) {
    UmlManager m;
    ASSERT_NO_THROW(m.open(ymlPath + "instanceSpecificationTests/backwardsClassifier.yml"));
    ASSERT_TRUE(m.getRoot()->getElementType() == UmlManager::ElementType<Package>::result);
    auto* pckg = &m.getRoot()->as<Package>();
    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().get(EGM::ID::fromString("HQci2EpoewLIakXPsMAjHKpLkByC"))->getElementType() == UmlManager::ElementType<InstanceSpecification>::result);
    ASSERT_TRUE(pckg->getPackagedElements().get(EGM::ID::fromString("wkz5zWQPLT7RaxpqT3RDFMeF0bA7"))->getElementType() == UmlManager::ElementType<Class>::result);
    auto c = pckg->getPackagedElements().get(EGM::ID::fromString("wkz5zWQPLT7RaxpqT3RDFMeF0bA7"));
    UmlManager::Pointer<InstanceSpecification> i = pckg->getPackagedElements().get(EGM::ID::fromString("HQci2EpoewLIakXPsMAjHKpLkByC"));
    ASSERT_EQ(i->getClassifiers().size(), 1);
    ASSERT_EQ(i->getClassifiers().front(), c);
}

TEST_F(InstanceSpecificationTest, basicSlotTest) {
    UmlManager m;
    ASSERT_NO_THROW(m.open(ymlPath + "instanceSpecificationTests/basicSlot.yml"));
    ASSERT_TRUE(m.getRoot()->getElementType() == UmlManager::ElementType<Package>::result);
    auto pckg = &m.getRoot()->as<Package>();
    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().get(EGM::ID::fromString("sQCzPFXcFEeTenYV0l8pGDwWTpo1"))->getElementType() == UmlManager::ElementType<InstanceSpecification>::result);
    ASSERT_TRUE(pckg->getPackagedElements().get(EGM::ID::fromString("efp5gW8d2yV5b9bKtY-OmBHJpUYw"))->getElementType() == UmlManager::ElementType<Class>::result);
    UmlManager::Pointer<Class> c = pckg->getPackagedElements().get(EGM::ID::fromString("efp5gW8d2yV5b9bKtY-OmBHJpUYw"));
    auto& i = pckg->getPackagedElements().get(EGM::ID::fromString("sQCzPFXcFEeTenYV0l8pGDwWTpo1"))->as<InstanceSpecification>();
    ASSERT_EQ(i.getClassifiers().size(), 1);
    ASSERT_EQ(i.getClassifiers().front(), c);
    ASSERT_TRUE(i.getSlots().size() == 1);
    ASSERT_TRUE(c->getOwnedAttributes().size() == 1);
    auto s = i.getSlots().front();
    auto p = c->getOwnedAttributes().front();
    ASSERT_TRUE(s->getDefiningFeature() == p);
    ASSERT_TRUE(*s->getOwningInstance() == i);
}

TEST_F(InstanceSpecificationTest, instanceValueSlot) {
    UmlManager::Pointer<Element> el = 0;
    UmlManager m;
    ASSERT_NO_THROW(m.open(ymlPath + "instanceSpecificationTests/instanceSlot.yml"));
    el = m.getRoot();
    ASSERT_TRUE(el->getElementType() == UmlManager::ElementType<Package>::result);
    UmlManager::Pointer<Package> pckg = el;
    ASSERT_TRUE(pckg->getPackagedElements().size() == 4);
    ASSERT_TRUE(pckg->getPackagedElements().get("clazz1")->getElementType() == UmlManager::ElementType<Class>::result);
    auto c1 = pckg->getPackagedElements().get("clazz1");
    ASSERT_TRUE(c1->getName().compare("clazz1") == 0);
    ASSERT_TRUE(pckg->getPackagedElements().get("clazz2")->getElementType() == UmlManager::ElementType<Class>::result);
    UmlManager::Pointer<Class> c2 = pckg->getPackagedElements().get("clazz2");
    ASSERT_TRUE(c2->getName().compare("clazz2") == 0);
    ASSERT_TRUE(c2->getOwnedAttributes().size() == 1);
    auto p = c2->getOwnedAttributes().front();
    ASSERT_TRUE(p->getType() == c1);
    ASSERT_TRUE(pckg->getPackagedElements().get(EGM::ID::fromString("x2zUOks6utCje0z-ID4EOM91HJuj"))->getElementType() == UmlManager::ElementType<InstanceSpecification>::result);
    UmlManager::Pointer<InstanceSpecification> i1 = pckg->getPackagedElements().get(EGM::ID::fromString("x2zUOks6utCje0z-ID4EOM91HJuj"));
    ASSERT_EQ(i1->getClassifiers().size(), 1);
    ASSERT_EQ(i1->getClassifiers().front(), c1);
    ASSERT_TRUE(pckg->getPackagedElements().get(EGM::ID::fromString("6CB8GbwIp4Ii0rKfx4YSVRoiZvmc"))->getElementType() == UmlManager::ElementType<InstanceSpecification>::result);
    UmlManager::Pointer<InstanceSpecification> i2 = pckg->getPackagedElements().get(EGM::ID::fromString("6CB8GbwIp4Ii0rKfx4YSVRoiZvmc"));
    ASSERT_EQ(i2->getClassifiers().size(), 1);
    ASSERT_EQ(i2->getClassifiers().front(), c2);
    ASSERT_TRUE(i2->getSlots().size() == 1);
    auto s = i2->getSlots().front();
    ASSERT_TRUE(s->getDefiningFeature() == p);
    ASSERT_TRUE(s->getValues().size() == 1);
    ASSERT_TRUE(s->getValues().front()->getElementType() == UmlManager::ElementType<InstanceValue>::result);
    UmlManager::Pointer<InstanceValue> v = s->getValues().front();
    ASSERT_TRUE(v->getInstance() == i1);
}

TEST_F(InstanceSpecificationTest, simpleInstanceEmitTest) {
    UmlManager m;
    auto& inst = *m.create<InstanceSpecification>();
    inst.setID(EGM::ID::fromString("3XvQFHIrqSmU7WAXA7fVzkw1v2U3"));
    inst.setName("simple");
    inst.setVisibility(VisibilityKind::PROTECTED);
    std::string expectedEmit = R""""(InstanceSpecification:
  id: 3XvQFHIrqSmU7WAXA7fVzkw1v2U3
  name: simple
  visibility: protected)"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = m.dump(inst));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(InstanceSpecificationTest, simpleSlotTest) {
    UmlManager m;
    auto& inst = *m.create<InstanceSpecification>();
    inst.setID(EGM::ID::fromString("yaogA9yjaFoD_RdGQzRrwe1826Aj"));
    inst.setName("slot");
    inst.setVisibility(VisibilityKind::PROTECTED);
    auto& s = *m.create<Slot>();
    s.setID(EGM::ID::fromString("w6arMVW4Plw0aLOBWLE9_8Xo_UL-"));
    inst.getSlots().add(s);
    std::string expectedEmit = R""""(InstanceSpecification:
  id: yaogA9yjaFoD_RdGQzRrwe1826Aj
  name: slot
  visibility: protected
  slots:
    - Slot:
        id: w6arMVW4Plw0aLOBWLE9_8Xo_UL-)"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = m.dump(inst));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(InstanceSpecificationTest, parseSpecificationTest) {
    UmlManager::Pointer<Element> el = 0;
    UmlManager m;
    ASSERT_NO_THROW(m.open(ymlPath + "instanceSpecificationTests/specification.yml"));
    el = m.getRoot();
    ASSERT_EQ(el->getElementType(), UmlManager::ElementType<InstanceSpecification>::result);
    auto& inst = el->as<InstanceSpecification>();
    ASSERT_TRUE(inst.getSpecification());
    ASSERT_EQ(inst.getSpecification()->getElementType(), UmlManager::ElementType<LiteralString>::result);
    ASSERT_EQ(inst.getSpecification()->as<LiteralString>().getValue(), "booga");
}

TEST_F(InstanceSpecificationTest, emitSpecificationTest) {
    UmlManager m;
    auto& inst = *m.create<InstanceSpecification>();
    auto& str = *m.create<LiteralString>();
    inst.setID(EGM::ID::fromString("fsU5Fw-5REaNv4NCvC0d4qZnXg4C"));
    str.setID(EGM::ID::fromString("nVzJ8mHx1yrRlct0ot34p7uBaVvC"));
    str.setValue("gatito");
    inst.setSpecification(&str);
    std::string expectedEmit = R""""(InstanceSpecification:
  id: fsU5Fw-5REaNv4NCvC0d4qZnXg4C
  specification:
    LiteralString:
      id: nVzJ8mHx1yrRlct0ot34p7uBaVvC
      value: gatito)"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = m.dump(inst));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

void ASSERT_RESTORE_SLOT_CORRECTLY(UmlManager::Implementation<InstanceSpecification>& inst, UmlManager::Implementation<Slot>& slot, size_t i) {
    ASSERT_EQ(inst.getSlots().size(), 1);
    //ASSERT_EQ(i == 0 ? inst.getSlots().front() : inst.getSlots().back(), slot);
    ASSERT_EQ(inst.getOwnedElements().size(), 1);
    ASSERT_TRUE(inst.getOwnedElements().contains(slot));
}

TEST_F(InstanceSpecificationTest, mountAndEditInstanceTest) {
    UmlManager m;
    auto& root = *m.create<Package>();
    auto& classifier = *m.create<Class>();
    auto& attribute = *m.create<Property>();
    auto& inst = *m.create<InstanceSpecification>();
    auto& slot = *m.create<Slot>();
    auto& type = *m.create<PrimitiveType>();
    auto& typeInst = *m.create<InstanceSpecification>();
    auto& typeSpecification = *m.create<LiteralString>();
    auto& slotVal = *m.create<InstanceValue>();
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

    EGM::ID instID = inst.getID();
    m.release(inst);
    auto& inst2 = *slot.getOwningInstance();
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(inst2, root));
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORE_SLOT_CORRECTLY(inst2, slot, 0));
    ASSERT_FALSE(inst2.getClassifiers().empty());
    ASSERT_EQ(inst2.getClassifiers().front(), &classifier);

    m.release(classifier);
    ASSERT_FALSE(inst2.getClassifiers().empty());
    auto& classifier2 = inst2.getClassifiers().front()->as<Class>();
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(classifier2, root));

    EGM::ID classifierID = classifier2.getID();
    m.release(inst2, classifier2);
    auto& inst3 = m.get(instID)->as<InstanceSpecification>();
    ASSERT_FALSE(inst3.getClassifiers().empty());
    // ASSERT_FALSE(m.loaded(classifierID));
    auto& classifier3 = m.get(classifierID)->as<Class>();
    ASSERT_EQ(inst3.getClassifiers().front(), &classifier3);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(inst3, root));
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORE_SLOT_CORRECTLY(inst3, slot, 0));
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(classifier3, root));

    EGM::ID slotID = slot.getID();
    m.release(slot);
    auto& slot2 = m.get(slotID)->as<Slot>();
    ASSERT_TRUE(slot2.getOwningInstance());
    ASSERT_EQ(*slot2.getOwningInstance(), inst3);
    ASSERT_TRUE(slot2.getOwner());
    ASSERT_EQ(*slot2.getOwner(), inst3);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORE_SLOT_CORRECTLY(inst3, slot2, 0));
    ASSERT_TRUE(slot2.getDefiningFeature());
    ASSERT_EQ(*slot2.getDefiningFeature(), attribute);

    m.release(slot2, inst3);
    auto& slot3 = m.get(slotID)->as<Slot>();
    ASSERT_TRUE(slot3.getOwningInstance());
    auto& inst4 = m.get(instID)->as<InstanceSpecification>();
    ASSERT_EQ(*slot3.getOwningInstance(), inst4);
    ASSERT_TRUE(slot3.getOwner());
    ASSERT_EQ(*slot3.getOwner(), inst4);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORE_SLOT_CORRECTLY(inst4, slot3, 0));
    ASSERT_TRUE(slot3.getDefiningFeature());
    ASSERT_EQ(*slot3.getDefiningFeature(), attribute);
    ASSERT_EQ(slot3.getValues().size(), 1);
    ASSERT_EQ(slot3.getValues().front(), &slotVal);
    ASSERT_EQ(slot3.getOwnedElements().size(), 1);
    ASSERT_EQ(*slot3.getOwnedElements().begin(), slotVal);

    m.release(slot3, inst4);
    auto& inst5 = m.get(instID)->as<InstanceSpecification>();
    auto& slot4 = m.get(slotID)->as<Slot>();
    ASSERT_EQ(*slot4.getOwningInstance(), inst5);
    ASSERT_TRUE(slot4.getOwner());
    ASSERT_EQ(*slot4.getOwner(), inst5);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORE_SLOT_CORRECTLY(inst5, slot4, 0));
    ASSERT_TRUE(slot4.getDefiningFeature());
    ASSERT_EQ(*slot4.getDefiningFeature(), attribute);
    ASSERT_EQ(slot4.getOwnedElements().size(), 1);
    ASSERT_EQ(*slot4.getOwnedElements().begin(), slotVal);

    EGM::ID propID = attribute.getID();
    m.release(slot4, attribute);
    auto& slot5 = m.get(slotID)->as<Slot>();
    // ASSERT_FALSE(m.loaded(propID));
    auto& attribute2 = m.get(propID)->as<Property>();
    ASSERT_TRUE(slot5.getDefiningFeature());
    ASSERT_EQ(*slot5.getDefiningFeature(), attribute2);
    ASSERT_EQ(slot5.getOwnedElements().size(), 1);
    ASSERT_EQ(*slot5.getOwnedElements().begin(), slotVal);

    EGM::ID valID = slotVal.getID();
    m.release(slotVal, slot5);
    auto& slot6 = m.get(slotID)->as<Slot>();
    // ASSERT_FALSE(m.loaded(valID));
    auto& slotVal2 = m.get(valID)->as<InstanceValue>();
    ASSERT_EQ(slot6.getOwnedElements().size(), 1);
    ASSERT_EQ(*slot6.getOwnedElements().begin(), slotVal2);
    ASSERT_TRUE(slotVal2.getOwner());
    ASSERT_EQ(*slotVal2.getOwner(), slot6);
    ASSERT_TRUE(slotVal2.getInstance());
    ASSERT_EQ(*slotVal2.getInstance(), typeInst);

    EGM::ID typeInstID = typeInst.getID();
    m.release(slotVal2, typeInst);
    auto& slotVal3 = m.get(valID)->as<InstanceValue>();
    // ASSERT_FALSE(m.loaded(typeInstID));
    auto& typeInst2 = m.get(typeInstID)->as<InstanceSpecification>();
    ASSERT_TRUE(slotVal3.getInstance());
    ASSERT_EQ(*slotVal3.getInstance(), typeInst2);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(typeInst2, root));
    ASSERT_FALSE(typeInst2.getClassifiers().empty());
    ASSERT_EQ(typeInst2.getClassifiers().front(), &type);
    ASSERT_TRUE(typeInst2.getSpecification());
    ASSERT_EQ(*typeInst2.getSpecification(), typeSpecification);
    ASSERT_EQ(typeInst2.getOwnedElements().size(), 1);
    ASSERT_EQ(*typeInst2.getOwnedElements().begin(), typeSpecification);

    EGM::ID specID = typeSpecification.getID();
    m.release(typeInst2, typeSpecification);
    auto& typeInst3 = m.get(typeInstID)->as<InstanceSpecification>();
    // ASSERT_FALSE(m.loaded(specID));
    auto& typeSpecification2 = m.get(specID)->as<LiteralString>();
    ASSERT_TRUE(typeSpecification2.getOwner());
    ASSERT_EQ(*typeSpecification2.getOwner(), typeInst3);
    ASSERT_TRUE(typeInst3.getSpecification());
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_OWNING_PACKAGE(typeInst3, root));
    ASSERT_FALSE(typeInst3.getClassifiers().empty());
    ASSERT_EQ(typeInst3.getClassifiers().front(), &type);
    ASSERT_EQ(*typeInst3.getSpecification(), typeSpecification2);
    ASSERT_EQ(typeInst3.getOwnedElements().size(), 1);
    ASSERT_EQ(*typeInst3.getOwnedElements().begin(), typeSpecification2);
}
