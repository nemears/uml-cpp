#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"
#include "test/uml-cpp-paths.h"

using namespace UML;

UML_SET_INTEGRATION_TEST(StructuredClassifierOwnedAttributes, Property, Class, &StructuredClassifier::getOwnedAttributes);
UML_SET_INTEGRATION_TEST(ClassOwnedAttributes, Property, Class, &Class::getOwnedAttributes);
UML_SET_INTEGRATION_TEST(StructuredClassifierOwnedConnectors, Connector, Class, &StructuredClassifier::getOwnedConnectors);
UML_SET_INTEGRATION_TEST(ClassNestingClassifiers, DataType, Class, &Class::getNestedClassifiers);
UML_SET_INTEGRATION_TEST(ClassOwnedOperations, Operation, Class, &Class::getOwnedOperations);
UML_SET_INTEGRATION_TEST(ClassOwnedReceptions, Reception, Class, &Class::getOwnedReceptions);
UML_SINGLETON_INTEGRATION_TEST(PropertyClass, Class, Property, &Property::getClass, &Property::setClass);
UML_SINGLETON_INTEGRATION_TEST(OperationClass, Class, Operation, &Operation::getClass, &Operation::setClass);

class ClassTest : public ::testing::Test {
    public:
        std::string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(ClassTest, BasicOperationTest) {
    UmlManager m;
    Class& c = *m.create<Class>();
    Operation& o = *m.create<Operation>();
    OpaqueBehavior& oB = *m.create<OpaqueBehavior>();
    LiteralString& val = *m.create<LiteralString>();
    Parameter& p = *m.create<Parameter>();
    ASSERT_NO_THROW(oB.getOwnedParameters().add(p));
    ASSERT_NO_THROW(val.setValue("return true"));
    ASSERT_NO_THROW(oB.getBodies().add(val));
    ASSERT_NO_THROW(o.getMethods().add(oB));
    ASSERT_NO_THROW(c.getOwnedOperations().add(o));
}

TEST_F(ClassTest, addOperationFunctorTest) {
    UmlManager m;
    Class& c = *m.create<Class>();
    Operation& o = *m.create<Operation>();
    c.getOwnedOperations().add(o);
    ASSERT_TRUE(c.getOwnedOperations().size() == 1);
    ASSERT_TRUE(&c.getOwnedOperations().front() == &o);
    ASSERT_TRUE(o.getClass() == &c);
    ASSERT_TRUE(c.getFeatures().size() == 1);
    ASSERT_TRUE(&c.getFeatures().front() == &o);
    ASSERT_TRUE(o.getFeaturingClassifier() == &c);
    ASSERT_TRUE(c.getMembers().size() == 1);
    ASSERT_TRUE(&c.getMembers().front() == &o);
    ASSERT_TRUE(o.getNamespace() == &c);
    ASSERT_TRUE(c.getOwnedElements().size() == 1);
    ASSERT_TRUE(&c.getOwnedElements().get(o.getID()) == &o);
    ASSERT_TRUE(o.getOwner() == &c);
}

TEST_F(ClassTest, setClassTest) {
    UmlManager m;
    Class& c = *m.create<Class>();
    Operation& o = *m.create<Operation>();
    o.setClass(c);
    ASSERT_TRUE(c.getOwnedOperations().size() == 1);
    ASSERT_TRUE(&c.getOwnedOperations().front() == &o);
    ASSERT_TRUE(o.getClass() == &c);
    ASSERT_TRUE(c.getFeatures().size() == 1);
    ASSERT_TRUE(&c.getFeatures().front() == &o);
    ASSERT_TRUE(o.getFeaturingClassifier() == &c);
    ASSERT_TRUE(c.getMembers().size() == 1);
    ASSERT_TRUE(&c.getMembers().front() == &o);
    ASSERT_TRUE(o.getNamespace() == &c);
    ASSERT_TRUE(c.getOwnedElements().size() == 1);
    ASSERT_TRUE(&c.getOwnedElements().get(o.getID()) == &o);
    ASSERT_TRUE(o.getOwner() == &c);
}

TEST_F(ClassTest, overwriteClassTest) {
  UmlManager m;
  Class& p1 = *m.create<Class>();
  Class& p2 = *m.create<Class>();
  Operation& c = *m.create<Operation>();
  p1.getOwnedOperations().add(c);
  c.setClass(&p2);
  ASSERT_TRUE(p2.getOwnedOperations().size() == 1);
  ASSERT_TRUE(&p2.getOwnedOperations().front() == &c);
  ASSERT_TRUE(c.getClass() == &p2);
  ASSERT_TRUE(p2.getFeatures().size() == 1);
  ASSERT_TRUE(&p2.getFeatures().front() == &c);
  ASSERT_TRUE(c.getFeaturingClassifier() == &p2);
  ASSERT_TRUE(p2.getMembers().size() == 1);
  ASSERT_TRUE(&p2.getMembers().front() == &c);
  ASSERT_TRUE(c.getNamespace() == &p2);
  ASSERT_TRUE(p2.getOwnedElements().size() == 1);
  ASSERT_TRUE(&p2.getOwnedElements().get(c.getID()) == &c);
  ASSERT_TRUE(c.getOwner() == &p2);
  ASSERT_TRUE(p1.getOwnedOperations().size() == 0);
  ASSERT_TRUE(p1.getFeatures().size() == 0);
  ASSERT_TRUE(p1.getMembers().size() == 0);
  ASSERT_TRUE(p1.getOwnedElements().size() == 0);
}

TEST_F(ClassTest, overwriteClassByOperationsAddTest) {
  UmlManager m;
  Class& p1 = *m.create<Class>();
  Class& p2 = *m.create<Class>();
  Operation& c = *m.create<Operation>();
  p1.getOwnedOperations().add(c);
  p2.getOwnedOperations().add(c);
  ASSERT_TRUE(p2.getOwnedOperations().size() == 1);
  ASSERT_TRUE(&p2.getOwnedOperations().front() == &c);
  ASSERT_TRUE(c.getClass() == &p2);
  ASSERT_TRUE(p2.getFeatures().size() == 1);
  ASSERT_TRUE(&p2.getFeatures().front() == &c);
  ASSERT_TRUE(c.getFeaturingClassifier() == &p2);
  ASSERT_TRUE(p2.getMembers().size() == 1);
  ASSERT_TRUE(&p2.getMembers().front() == &c);
  ASSERT_TRUE(c.getNamespace() == &p2);
  ASSERT_TRUE(p2.getOwnedElements().size() == 1);
  ASSERT_TRUE(&p2.getOwnedElements().get(c.getID()) == &c);
  ASSERT_TRUE(c.getOwner() == &p2);
  ASSERT_TRUE(p1.getOwnedOperations().size() == 0);
  ASSERT_TRUE(p1.getFeatures().size() == 0);
  ASSERT_TRUE(p1.getMembers().size() == 0);
  ASSERT_TRUE(p1.getOwnedElements().size() == 0);
}

TEST_F(ClassTest, removeOperationFunctorTest) {
  UmlManager m;
    Class& c = *m.create<Class>();
    Operation& o = *m.create<Operation>();
  c.getOwnedOperations().add(o);
  ASSERT_NO_THROW(c.getOwnedOperations().remove(o));
  ASSERT_TRUE(c.getOwnedOperations().size() == 0);
  ASSERT_TRUE(c.getFeatures().size() == 0);
  ASSERT_TRUE(c.getMembers().size() == 0);
  ASSERT_TRUE(c.getOwnedElements().size() == 0);
  ASSERT_TRUE(!o.getClass());
  ASSERT_TRUE(!o.getFeaturingClassifier());
  ASSERT_TRUE(!o.getNamespace());
  ASSERT_TRUE(!o.getOwner());
}

TEST_F(ClassTest, addOwnedAttributeTest) {
  UmlManager m;
  Class& c = *m.create<Class>();
  Property& p = *m.create<Property>();
  ASSERT_NO_THROW(c.getOwnedAttributes().add(p));
  ASSERT_TRUE(c.getOwnedAttributes().size() == 1);
  ASSERT_TRUE(&c.getOwnedAttributes().front() == &p);
  ASSERT_TRUE(c.getRoles().size() == 1);
  ASSERT_TRUE(&c.getRoles().front() == &p);
  ASSERT_TRUE(c.getOwnedMembers().size() == 1);
  ASSERT_TRUE(&c.getOwnedMembers().front() == &p);
  ASSERT_TRUE(c.getFeatures().size() == 1);
  ASSERT_TRUE(&c.getFeatures().front() == &p);
  ASSERT_TRUE(c.getAttributes().size() == 1);
  ASSERT_TRUE(&c.getAttributes().front() == &p);
  ASSERT_TRUE(c.getMembers().size() == 1);
  ASSERT_TRUE(&c.getMembers().front() == &p);
  ASSERT_TRUE(c.getOwnedElements().size() == 1);
  ASSERT_TRUE(&c.getOwnedElements().get(p.getID()) == &p);

  ASSERT_TRUE(p.getClass());
  ASSERT_EQ(*p.getClass(), c);
  ASSERT_TRUE(p.getFeaturingClassifier() == &c);
  ASSERT_TRUE(p.getNamespace() == &c);
  ASSERT_TRUE(p.getOwner() == &c);
}

TEST_F(ClassTest, addOwnedAttributeAsStructuredClassifierTest) {
  UmlManager m;
  Class& c = *m.create<Class>();
  Property& p = *m.create<Property>();
  c.as<StructuredClassifier>().getOwnedAttributes().add(p);
  ASSERT_TRUE(c.getOwnedAttributes().size() == 1);
  ASSERT_TRUE(&c.getOwnedAttributes().front() == &p);
  ASSERT_TRUE(c.getRoles().size() == 1);
  ASSERT_TRUE(&c.getRoles().front() == &p);
  ASSERT_TRUE(c.getOwnedMembers().size() == 1);
  ASSERT_TRUE(&c.getOwnedMembers().front() == &p);
  ASSERT_TRUE(c.getFeatures().size() == 1);
  ASSERT_TRUE(&c.getFeatures().front() == &p);
  ASSERT_TRUE(c.getAttributes().size() == 1);
  ASSERT_TRUE(&c.getAttributes().front() == &p);
  ASSERT_TRUE(c.getMembers().size() == 1);
  ASSERT_TRUE(&c.getMembers().front() == &p);
  ASSERT_TRUE(c.getOwnedElements().size() == 1);
  ASSERT_TRUE(&c.getOwnedElements().get(p.getID()) == &p);

  ASSERT_TRUE(p.getClass());
  ASSERT_EQ(*p.getClass(), c);
  ASSERT_TRUE(p.getFeaturingClassifier() == &c);
  ASSERT_TRUE(p.getNamespace() == &c);
  ASSERT_TRUE(p.getOwner() == &c);
}

TEST_F(ClassTest, setStructuredClassifierTest) {
  UmlManager m;
  Class& c = *m.create<Class>();
  Property& p = *m.create<Property>();
  ASSERT_NO_THROW(p.setClass(&c));
  ASSERT_TRUE(c.getOwnedAttributes().size() == 1);
  ASSERT_TRUE(&c.getOwnedAttributes().front() == &p);
  // ASSERT_TRUE(c.getRole().size() == 1);
  // ASSERT_TRUE(&c.getRole().front() == &p);
  ASSERT_TRUE(c.getOwnedMembers().size() == 1);
  ASSERT_TRUE(&c.getOwnedMembers().front() == &p);
  ASSERT_TRUE(c.getFeatures().size() == 1);
  ASSERT_TRUE(&c.getFeatures().front() == &p);
  ASSERT_TRUE(c.getAttributes().size() == 1);
  ASSERT_TRUE(&c.getAttributes().front() == &p);
  ASSERT_TRUE(c.getMembers().size() == 1);
  ASSERT_TRUE(&c.getMembers().front() == &p);
  ASSERT_TRUE(c.getOwnedElements().size() == 1);
  ASSERT_TRUE(&c.getOwnedElements().get(p.getID()) == &p);

  ASSERT_TRUE(*p.getFeaturingClassifier() == c);
  ASSERT_TRUE(*p.getNamespace() == c);
  ASSERT_TRUE(*p.getOwner() == c);
}

TEST_F(ClassTest, removeOwnedAttributeFunctorTest) {
  UmlManager m;
  Class& c = *m.create<Class>();
  Property& p = *m.create<Property>();
  c.getOwnedAttributes().add(p);
  ASSERT_NO_THROW(c.getOwnedAttributes().remove(p));
  ASSERT_TRUE(c.getOwnedAttributes().size() == 0);
  ASSERT_TRUE(c.getAttributes().size() == 0);
  // ASSERT_TRUE(c.getRole().size() == 0);
  ASSERT_TRUE(c.getFeatures().size() == 0);
  ASSERT_TRUE(c.getOwnedMembers().size() == 0);
  ASSERT_TRUE(c.getMembers().size() == 0);
  ASSERT_TRUE(c.getOwnedElements().size() == 0);
  ASSERT_FALSE(p.getFeaturingClassifier());
  ASSERT_FALSE(p.getNamespace());
  ASSERT_FALSE(p.getOwner());
}

TEST_F(ClassTest, setFeaturingClassifierNullTest) {
  UmlManager m;
  Class& c = *m.create<Class>();
  Property& p = *m.create<Property>();
  c.getOwnedAttributes().add(p);
  ASSERT_NO_THROW(p.setClass(0));
  ASSERT_TRUE(c.getOwnedAttributes().size() == 0);
  ASSERT_TRUE(c.getAttributes().size() == 0);
  // ASSERT_TRUE(c.getRole().size() == 0);
  ASSERT_TRUE(c.getFeatures().size() == 0);
  ASSERT_TRUE(c.getOwnedMembers().size() == 0);
  ASSERT_TRUE(c.getMembers().size() == 0);
  ASSERT_TRUE(c.getOwnedElements().size() == 0);

  ASSERT_FALSE(p.getFeaturingClassifier());
  ASSERT_FALSE(p.getNamespace());
  ASSERT_FALSE(p.getOwner());
}

TEST_F(ClassTest, addCompositePropertyTest) {
  UmlManager m;
  Class& c = *m.create<Class>();
  Property& p = *m.create<Property>();
  p.setAggregation(AggregationKind::COMPOSITE);
  c.getOwnedAttributes().add(p);
  ASSERT_TRUE(c.getParts().size() == 1);
  ASSERT_TRUE(&c.getParts().front() == &p);
  ASSERT_TRUE(c.getOwnedAttributes().size() == 1);
  ASSERT_TRUE(&c.getOwnedAttributes().front() == &p);
  ASSERT_TRUE(c.getAttributes().size() == 1);
  ASSERT_TRUE(&c.getAttributes().front() == &p);
  ASSERT_TRUE(c.getRoles().size() == 1);
  ASSERT_TRUE(&c.getRoles().front() == &p);
  ASSERT_TRUE(c.getFeatures().size() == 1);
  ASSERT_TRUE(&c.getFeatures().front() == &p);
  ASSERT_TRUE(c.getOwnedMembers().size() == 1);
  ASSERT_TRUE(&c.getOwnedMembers().front() == &p);
  ASSERT_TRUE(c.getMembers().size() == 1);
  ASSERT_TRUE(&c.getMembers().front() == &p);
  ASSERT_TRUE(c.getOwnedElements().size() == 1);
  ASSERT_TRUE(&c.getOwnedElements().get(p.getID()) == &p);

  ASSERT_TRUE(p.isComposite());
  ASSERT_TRUE(p.getFeaturingClassifier() == &c);
  ASSERT_TRUE(p.getNamespace() == &c);
  ASSERT_TRUE(p.getOwner() == &c);
}

TEST_F(ClassTest, backwardsAddCompositePropertyTest) {
  UmlManager m;
  Class& c = *m.create<Class>();
  Property& p = *m.create<Property>();
  ASSERT_NO_THROW(c.getOwnedAttributes().add(p));
  ASSERT_NO_THROW(p.setAggregation(AggregationKind::COMPOSITE));
  ASSERT_TRUE(c.getParts().size() == 1);
  ASSERT_TRUE(&c.getParts().front() == &p);
  ASSERT_TRUE(c.getOwnedAttributes().size() == 1);
  ASSERT_TRUE(&c.getOwnedAttributes().front() == &p);
  ASSERT_TRUE(c.getAttributes().size() == 1);
  ASSERT_TRUE(&c.getAttributes().front() == &p);
  ASSERT_TRUE(c.getRoles().size() == 1);
  ASSERT_TRUE(&c.getRoles().front() == &p);
  ASSERT_TRUE(c.getFeatures().size() == 1);
  ASSERT_TRUE(&c.getFeatures().front() == &p);
  ASSERT_TRUE(c.getOwnedMembers().size() == 1);
  ASSERT_TRUE(&c.getOwnedMembers().front() == &p);
  ASSERT_TRUE(c.getMembers().size() == 1);
  ASSERT_TRUE(&c.getMembers().front() == &p);
  ASSERT_TRUE(c.getOwnedElements().size() == 1);
  ASSERT_TRUE(&c.getOwnedElements().get(p.getID()) == &p);

  ASSERT_TRUE(p.isComposite());
  ASSERT_TRUE(p.getFeaturingClassifier() == &c);
  ASSERT_TRUE(p.getNamespace() == &c);
  ASSERT_TRUE(p.getOwner() == &c);
}

TEST_F(ClassTest, removePropertyFromParts) {
  UmlManager m;
  Class& c = *m.create<Class>();
  Property& p = *m.create<Property>();
  p.setAggregation(AggregationKind::COMPOSITE);
  c.getOwnedAttributes().add(p);
  ASSERT_NO_THROW(p.setAggregation(AggregationKind::NONE));

  ASSERT_TRUE(c.getParts().size() == 0);
  ASSERT_TRUE(c.getOwnedAttributes().size() == 1);
  ASSERT_TRUE(&c.getOwnedAttributes().front() == &p);
  ASSERT_TRUE(c.getAttributes().size() == 1);
  ASSERT_TRUE(&c.getAttributes().front() == &p);
  ASSERT_TRUE(c.getRoles().size() == 1);
  ASSERT_TRUE(&c.getRoles().front() == &p);
  ASSERT_TRUE(c.getFeatures().size() == 1);
  ASSERT_TRUE(&c.getFeatures().front() == &p);
  ASSERT_TRUE(c.getOwnedMembers().size() == 1);
  ASSERT_TRUE(&c.getOwnedMembers().front() == &p);
  ASSERT_TRUE(c.getMembers().size() == 1);
  ASSERT_TRUE(&c.getMembers().front() == &p);
  ASSERT_TRUE(c.getOwnedElements().size() == 1);
  ASSERT_TRUE(&c.getOwnedElements().get(p.getID()) == &p);

  ASSERT_TRUE(!p.isComposite());
  ASSERT_TRUE(p.getFeaturingClassifier() == &c);
  ASSERT_TRUE(p.getNamespace() == &c);
  ASSERT_TRUE(p.getOwner() == &c);
}

TEST_F(ClassTest, addAndRemoveNestedClassifierTest) {
  UmlManager m;
  Class& c = *m.create<Class>();
  DataType& d = *m.create<DataType>();
  c.getNestedClassifiers().add(d);
  ASSERT_EQ(c.getNestedClassifiers().size(), 1);
  ASSERT_EQ(c.getNestedClassifiers().front().getID(), d.getID());
  ASSERT_EQ(c.getOwnedMembers().size(), 1);
  ASSERT_EQ(c.getOwnedMembers().front().getID(), d.getID());
  // ASSERT_EQ(c.getRedefinedElements().size(), 1);
  // ASSERT_EQ(c.getRedefinedElements().front().getID(), d.getID());
  c.getNestedClassifiers().remove(d);
  ASSERT_EQ(c.getNestedClassifiers().size(), 0);
  ASSERT_EQ(c.getOwnedMembers().size(), 0);
  ASSERT_EQ(c.getNestedClassifiers().size(), 0);
}

TEST_F(ClassTest, parseID_andName) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "classTests/class_w_id_and_name.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::CLASS);
    Class* clazz = dynamic_cast<Class*>(el);
    ASSERT_TRUE(clazz->getID() == ID::fromString("g8WBwHt6sgOqvS9ZlgKv9XTmHZ&C"));
    ASSERT_TRUE(clazz->getName().compare("test") == 0);
}

TEST_F(ClassTest, parseBasicProperty) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "classTests/classWithAttributes.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::CLASS);
    Class* clazz = dynamic_cast<Class*>(el);
    ASSERT_TRUE(clazz->getAttributes().size() == 2);
    Property* prop1 = dynamic_cast<Property*>(&clazz->getAttributes().front());
    Property* prop2 = dynamic_cast<Property*>(&clazz->getAttributes().back());
    ASSERT_TRUE(prop1->getClass() == clazz);
    // ASSERT_TRUE(prop1->getNamespace() == clazz);
    // ASSERT_TRUE(prop1->getOwner() == clazz);
    ASSERT_TRUE(&clazz->getMembers().front() == prop1);
    // ASSERT_TRUE(clazz->getOwnedElements().front() == prop1);
    ASSERT_TRUE(prop2->getClass() == clazz);
    //ASSERT_TRUE(prop2->getNamespace() == clazz);
    // ASSERT_TRUE(prop2->getOwner() == clazz);
    ASSERT_TRUE(&clazz->getMembers().back() == prop2);
    // ASSERT_TRUE(clazz->getOwnedElements().back() == prop2);
}

TEST_F(ClassTest, parseOperation) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "classTests/operation.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::CLASS);
    Class* clazz = dynamic_cast<Class*>(el);
    ASSERT_EQ(clazz->getOwnedOperations().size(), 1);
    Operation* op = &clazz->getOwnedOperations().front();
    ASSERT_TRUE(op->getName().compare("isValid") == 0);
    ASSERT_EQ(op->getMethods().size(), 1);
    OpaqueBehavior* bhv = dynamic_cast<OpaqueBehavior*>(&op->getMethods().front());
    ASSERT_TRUE(bhv->getName().compare("isValid") == 0);
    ASSERT_EQ(bhv->getBodies().size(), 1);
    ASSERT_TRUE(bhv->getBodies().front().getValue().compare("return true") == 0);
    ASSERT_EQ(bhv->getOwnedParameters().size(), 1);
}

TEST_F(ClassTest, properErrors) {
    Element* el;
    UmlManager m;
    ASSERT_THROW(el = m.parse(ymlPath + "classTests/improperOperationIdentifier.yml").ptr(), Parsers::UmlParserException);
    ASSERT_THROW(el = m.parse(ymlPath + "classTests/operationsNotSequence.yml").ptr(), Parsers::UmlParserException);
}

TEST_F(ClassTest, basicGeneralizationTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "classTests/basicGeneralization.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::CLASS);
    Class* general = dynamic_cast<Class*>(&pckg->getPackagedElements().front());
    ASSERT_TRUE(general->getName().compare("general") == 0);
    ASSERT_TRUE(pckg->getPackagedElements().back().getElementType() == ElementType::CLASS);
    Class* specific = dynamic_cast<Class*>(&pckg->getPackagedElements().back());
    ASSERT_TRUE(specific->getName().compare("specific") == 0);
    ASSERT_TRUE(specific->getGeneralizations().size() == 1);
    ASSERT_TRUE(specific->getGenerals().size() == 1);
    ASSERT_TRUE(&specific->getGenerals().front() == general);
    Generalization* g = &specific->getGeneralizations().front();
    ASSERT_TRUE(g->getGeneral() == general);
    ASSERT_TRUE(g->getSpecific() == specific);
}

TEST_F(ClassTest, inheritedMembersTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "classTests/inheritedMembers.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 4);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::CLASS);
    Class* general = dynamic_cast<Class*>(&pckg->getPackagedElements().get("general"));
    ASSERT_TRUE(general->getName().compare("general") == 0);
    ASSERT_TRUE(pckg->getPackagedElements().get(1).getElementType() == ElementType::CLASS);
    Class* specific = dynamic_cast<Class*>(&pckg->getPackagedElements().get("specific"));
    ASSERT_TRUE(specific->getName().compare("specific") == 0);
    ASSERT_TRUE(specific->getGeneralizations().size() == 1);
    ASSERT_TRUE(specific->getGenerals().size() == 1);
    ASSERT_TRUE(&specific->getGenerals().front() == general);
    Generalization* g = &specific->getGeneralizations().front();
    ASSERT_TRUE(g->getGeneral() == general);
    ASSERT_TRUE(g->getSpecific() == specific);
    ASSERT_TRUE(general->getOwnedAttributes().size() == 1);
    Property* gProp = &general->getOwnedAttributes().front();
    ASSERT_TRUE(specific->getInheritedMembers().size() == 1);
    ASSERT_TRUE(&specific->getInheritedMembers().front() == gProp);
    
    ASSERT_TRUE(pckg->getPackagedElements().get(2).getElementType() == ElementType::CLASS);
    Class* privateGeneral = dynamic_cast<Class*>(&pckg->getPackagedElements().get("private"));
    ASSERT_TRUE(pckg->getPackagedElements().get(3).getElementType() == ElementType::CLASS);
    Class* privateSpecific = dynamic_cast<Class*>(&pckg->getPackagedElements().get(ID::fromString("hWVMp5upOkVsWnkrfl0I6O5bQsbO")));
    ASSERT_TRUE(privateGeneral->getOwnedAttributes().size() == 1);
    ASSERT_TRUE(privateGeneral->getOwnedAttributes().front().getVisibility() == VisibilityKind::PRIVATE);
    ASSERT_TRUE(privateSpecific->getInheritedMembers().size() == 0);
}

TEST_F(ClassTest, emitClassWAttribute) {
    UmlManager m;
    Class& c = *m.create<Class>();
    Property& p = *m.create<Property>();
    c.setID("hWVMp5upOkVsWnkrfl0I6O5bQsbO");
    c.setName("Class");
    p.setID("61255etITfg0LgPLZaU1PEByTjo3");
    p.setName("prop");
    p.setVisibility(VisibilityKind::PRIVATE);
    c.getOwnedAttributes().add(p);
    std::string expectedEmit = R""""(class:
  id: hWVMp5upOkVsWnkrfl0I6O5bQsbO
  name: Class
  ownedAttributes:
    - property:
        id: 61255etITfg0LgPLZaU1PEByTjo3
        name: prop
        visibility: PRIVATE)"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(c));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(ClassTest, emitClassWAttributeNOperation) {
    UmlManager m;
    Class& c = *m.create<Class>();
    Property& p = *m.create<Property>();
    Operation& o = *m.create<Operation>();
    c.setID("b0XPjtodVDLoVu2YCMwBWYqglsoX");
    c.setName("Class");
    p.setID("kfuX2BvkrRFhMX4UAfchHJgL8sER");
    p.setName("prop");
    p.setVisibility(VisibilityKind::PRIVATE);
    o.setID("ESKTcd5FmF2q4O&WI_Oiu5FrXHeN");
    o.setName("op");
    o.setVisibility(VisibilityKind::PROTECTED);
    c.getOwnedAttributes().add(p);
    c.getOwnedOperations().add(o);
    std::string expectedEmit = R""""(class:
  id: b0XPjtodVDLoVu2YCMwBWYqglsoX
  name: Class
  ownedAttributes:
    - property:
        id: kfuX2BvkrRFhMX4UAfchHJgL8sER
        name: prop
        visibility: PRIVATE
  ownedOperations:
    - operation:
        id: ESKTcd5FmF2q4O&WI_Oiu5FrXHeN
        name: op
        visibility: PROTECTED)"""";
    std::string generatedEmit;
    generatedEmit = Parsers::emit(c);
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(ClassTest, emitFilledInOperation) {
    UmlManager m;
    Class& c = *m.create<Class>();
    Operation& o = *m.create<Operation>();
    OpaqueBehavior& b = *m.create<OpaqueBehavior>();
    Parameter& p = *m.create<Parameter>();
    Parameter& p2 = *m.create<Parameter>();
    c.setID("6cCDjqUmkrXZ46z7CcNaTDso4SfQ");
    c.setName("Class");
    o.setID("Y3WV0c_Wa_zfOTb6zo9BAiIqRhn7");
    o.setName("op");
    o.setVisibility(VisibilityKind::PROTECTED);
    b.setID("&_DLItAl_5ASwPNvNVqXaIwEiLOx");
    b.setName("opaque");
    p.setID("s2q_fjRnyV_Gst&gAQ4JTr3crFNU");
    p.setName("pee");
    p2.setID("C7lT8BaQxmMi7cnZLIjjWCVD3k_9");
    p2.setName("opaquePee");
    o.getOwnedParameters().add(p);
    b.getOwnedParameters().add(p2);
    c.getOwnedBehaviors().add(b);
    o.getMethods().add(b);
    c.getOwnedOperations().add(o);
    std::string expectedEmit = R""""(class:
  id: 6cCDjqUmkrXZ46z7CcNaTDso4SfQ
  name: Class
  ownedBehaviors:
    - opaqueBehavior:
        id: "&_DLItAl_5ASwPNvNVqXaIwEiLOx"
        name: opaque
        ownedParameters:
          - parameter:
              id: C7lT8BaQxmMi7cnZLIjjWCVD3k_9
              name: opaquePee
        specification: Y3WV0c_Wa_zfOTb6zo9BAiIqRhn7
  ownedOperations:
    - operation:
        id: Y3WV0c_Wa_zfOTb6zo9BAiIqRhn7
        name: op
        visibility: PROTECTED
        methods:
          - "&_DLItAl_5ASwPNvNVqXaIwEiLOx"
        ownedParameters:
          - parameter:
              id: s2q_fjRnyV_Gst&gAQ4JTr3crFNU
              name: pee)"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(c));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(ClassTest, nestedClassifierParsingTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "classTests/nestedClassifiers.yml").ptr());
    ASSERT_EQ(el->getElementType(), ElementType::CLASS);
    Class& clazz = el->as<Class>();
    ASSERT_EQ(clazz.getNestedClassifiers().size(), 7);
    ASSERT_EQ(clazz.getNestedClassifiers().get(0).getElementType(), ElementType::ASSOCIATION);
    ASSERT_EQ(clazz.getNestedClassifiers().get(0).getName(), "assoc");
    ASSERT_EQ(clazz.getNestedClassifiers().get(1).getElementType(), ElementType::ARTIFACT);
    ASSERT_EQ(clazz.getNestedClassifiers().get(1).getName(), "art");
    ASSERT_EQ(clazz.getNestedClassifiers().get(2).getElementType(), ElementType::CLASS);
    ASSERT_EQ(clazz.getNestedClassifiers().get(2).getName(), "class");
    ASSERT_EQ(clazz.getNestedClassifiers().get(3).getElementType(), ElementType::DATA_TYPE);
    ASSERT_EQ(clazz.getNestedClassifiers().get(3).getName(), "bigD");
    ASSERT_EQ(clazz.getNestedClassifiers().get(4).getElementType(), ElementType::ENUMERATION);
    ASSERT_EQ(clazz.getNestedClassifiers().get(4).getName(), "e");
    ASSERT_EQ(clazz.getNestedClassifiers().get(5).getElementType(), ElementType::OPAQUE_BEHAVIOR);
    ASSERT_EQ(clazz.getNestedClassifiers().get(5).getName(), "bb");
    ASSERT_EQ(clazz.getNestedClassifiers().get(6).getElementType(), ElementType::PRIMITIVE_TYPE);
    ASSERT_EQ(clazz.getNestedClassifiers().get(6).getName(), "pp");
}

TEST_F(ClassTest, nestedClassifierEmitTest) {
    UmlManager m;
    Class& clazz = *m.create<Class>();
    Artifact& artifact = *m.create<Artifact>();
    Association& association = *m.create<Association>();
    Class& nestedClazz = *m.create<Class>();
    DataType& dataType = *m.create<DataType>();
    Enumeration& enumeration = *m.create<Enumeration>();
    OpaqueBehavior& opaqueBehavior = *m.create<OpaqueBehavior>();
    PrimitiveType& primitiveType = *m.create<PrimitiveType>();
    clazz.setID("5mOWzor&UjeUs13VT9&HYj5DKh&Y");
    artifact.setID("F_exblp0xsz5k1lmTLDtjBrFWqS6");
    association.setID("oOgal3or1U2zY9ktKohwQS6ChLw7");
    nestedClazz.setID("4Q6XfIWChz&mfIB_6Vc71mnxHDpU");
    dataType.setID("Nw3c30z1PCo3GNs6QFh&wt9fVVzf");
    enumeration.setID("lItnoDw_Ka4bfYaRnzrdFZnwqY3X");
    opaqueBehavior.setID("j82g9_8Al4Vcp1PQ0wsS4ia9_MR4");
    primitiveType.setID("FTjeJqMozlqjetKextwOJiSIeZA7");
    clazz.getNestedClassifiers().add(artifact);
    clazz.getNestedClassifiers().add(association);
    clazz.getNestedClassifiers().add(nestedClazz);
    clazz.getNestedClassifiers().add(dataType);
    clazz.getNestedClassifiers().add(enumeration);
    clazz.getNestedClassifiers().add(opaqueBehavior);
    clazz.getNestedClassifiers().add(primitiveType);
    std::string expectedEmit = R""""(class:
  id: 5mOWzor&UjeUs13VT9&HYj5DKh&Y
  nestedClassifiers:
    - artifact:
        id: F_exblp0xsz5k1lmTLDtjBrFWqS6
    - association:
        id: oOgal3or1U2zY9ktKohwQS6ChLw7
    - class:
        id: 4Q6XfIWChz&mfIB_6Vc71mnxHDpU
    - dataType:
        id: Nw3c30z1PCo3GNs6QFh&wt9fVVzf
    - enumeration:
        id: lItnoDw_Ka4bfYaRnzrdFZnwqY3X
    - opaqueBehavior:
        id: j82g9_8Al4Vcp1PQ0wsS4ia9_MR4
    - primitiveType:
        id: FTjeJqMozlqjetKextwOJiSIeZA7)"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(clazz));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(ClassTest, mountFullClassTest) {
    UmlManager m;
    Package& pckg = *m.create<Package>();
    Class& base = *m.create<Class>();
    Class& spec = *m.create<Class>();
    Class& nestSpec = *m.create<Class>();
    Property& prop = *m.create<Property>();
    Operation& op = *m.create<Operation>();
    Generalization& gen = *m.create<Generalization>();
    spec.setName("specific");
    prop.setName("property");
    base.getOwnedAttributes().add(prop);
    op.setName("operation");
    base.getOwnedOperations().add(op);
    gen.setGeneral(&base);
    gen.setSpecific(&spec);
    nestSpec.setName("nested");
    spec.getNestedClassifiers().add(nestSpec);
    pckg.getPackagedElements().add(base);
    pckg.getPackagedElements().add(spec);
    m.setRoot(&pckg);
    std::string mountPath = ymlPath + "classTests";
    ASSERT_NO_THROW(m.mount(mountPath));
    ASSERT_NO_THROW(m.release(base.getID()));
    ASSERT_TRUE(prop.getOwner());
    Class& base2 = prop.getOwner()->as<Class>();
    ASSERT_TRUE(prop.getClass());
    ASSERT_EQ(prop.getClass(), &base2);
    ASSERT_TRUE(prop.getFeaturingClassifier());
    ASSERT_EQ(*prop.getFeaturingClassifier(), base2);
    ASSERT_TRUE(prop.getNamespace());
    ASSERT_EQ(*prop.getNamespace(), base2);

    ASSERT_EQ(base2.getOwnedAttributes().size(), 1);
    ASSERT_EQ(base2.getOwnedAttributes().front(), prop);
    ASSERT_EQ(base2.getAttributes().size(), 1);
    ASSERT_EQ(&base2.getAttributes().get("property"), &prop);
    ASSERT_EQ(base2.getFeatures().size(), 2);
    ASSERT_EQ(&base2.getFeatures().get("property"), &prop);
    ASSERT_EQ(base2.getOwnedMembers().size(), 2);
    ASSERT_EQ(&base2.getOwnedMembers().get("property"), &prop);
    ASSERT_EQ(base2.getMembers().size(), 2);
    ASSERT_EQ(&base2.getMembers().get("property"), &prop);
    ASSERT_EQ(base2.getOwnedElements().size(), 2);
    ASSERT_EQ(base2.getOwnedElements().get("property"), prop);

    ASSERT_TRUE(op.getClass());
    ASSERT_EQ(*op.getClass(), base2);
    ASSERT_TRUE(op.getFeaturingClassifier());
    ASSERT_EQ(*op.getFeaturingClassifier(), base2);
    ASSERT_TRUE(op.getNamespace());
    ASSERT_EQ(*op.getNamespace(), base2);
    ASSERT_TRUE(op.getOwner());
    ASSERT_EQ(*op.getOwner(), base2);

    ASSERT_EQ(base2.getOwnedOperations().size(), 1);
    ASSERT_EQ(&base2.getOwnedOperations().front(), &op);
    ASSERT_EQ(&base2.getFeatures().get("operation"), &op);
    ASSERT_EQ(&base2.getOwnedMembers().get("operation"), &op);
    ASSERT_EQ(&base2.getMembers().get("operation"), &op);
    ASSERT_EQ(base2.getOwnedElements().get("operation"), op);

    ASSERT_TRUE(gen.getGeneral());
    ASSERT_EQ(gen.getGeneral(), &base2);
    ASSERT_EQ(spec.getGenerals().size(), 1);
    ASSERT_EQ(&spec.getGenerals().front(), &base2);

    /** TODO: anything else to test with base class? **/
    /** release specific **/
    ASSERT_NO_THROW(m.release(spec));
    Class& spec2 = pckg.getPackagedElements().get("specific").as<Class>(); // load specific

    ASSERT_TRUE(gen.getSpecific());
    ASSERT_EQ(*gen.getSpecific(), spec2);
    ASSERT_EQ(gen.getSources().size(), 1);
    ASSERT_EQ(&gen.getSources().front(), &spec2);
    ASSERT_EQ(gen.getRelatedElements().size(), 2);
    ASSERT_TRUE(gen.getRelatedElements().contains(spec2));
    ASSERT_TRUE(gen.getOwner());
    ASSERT_EQ(*gen.getOwner(), spec2);

    ASSERT_TRUE(nestSpec.getNamespace());
    ASSERT_TRUE(nestSpec.getOwner());
    ASSERT_EQ(*nestSpec.getOwner(), spec2);

    ASSERT_EQ(spec2.getGeneralizations().size(), 1);
    ASSERT_EQ(&spec2.getGeneralizations().front(), &gen);
    ASSERT_EQ(spec2.getGenerals().size(), 1);
    ASSERT_EQ(&spec2.getGenerals().front(), &base2);
    ASSERT_EQ(spec2.getInheritedMembers().size(), 2);
    ASSERT_EQ(&spec2.getInheritedMembers().get("property"), &prop);
    ASSERT_EQ(&spec2.getInheritedMembers().get("operation"), &op);
    ASSERT_EQ(spec2.getNestedClassifiers().size(), 1);
    ASSERT_EQ(&spec2.getNestedClassifiers().front(), &nestSpec);
    ASSERT_EQ(spec2.getOwnedMembers().size(), 1);
    ASSERT_EQ(&spec2.getOwnedMembers().front(), &nestSpec);
    ASSERT_EQ(spec2.getMembers().size(), 3);
    ASSERT_EQ(&spec2.getMembers().get("nested"), &nestSpec);
    ASSERT_EQ(&spec2.getMembers().get("property"), &prop);
    ASSERT_EQ(&spec2.getMembers().get("operation"), &op);
    ASSERT_EQ(spec2.getOwnedElements().size(), 2);
    ASSERT_TRUE(spec2.getOwnedElements().contains(gen));
    ASSERT_EQ(spec2.getOwnedElements().get("nested"), nestSpec);

    m.release(gen);
    ASSERT_EQ(spec2.getGeneralizations().size(), 1);
    Generalization& gen2 = spec2.getGeneralizations().front();
    ASSERT_TRUE(gen2.getSpecific());
    ASSERT_EQ(*gen2.getSpecific(), spec2);
    ASSERT_EQ(gen2.getSources().size(), 1);
    ASSERT_EQ(gen2.getSources().front(), spec2);
    ASSERT_EQ(gen2.getRelatedElements().size(), 2);
    ASSERT_TRUE(gen2.getRelatedElements().count(spec2.getID()));
    ASSERT_EQ(gen2.getRelatedElements().get(spec2.getID()), spec2);
    ASSERT_TRUE(gen2.getOwner());
    ASSERT_EQ(*gen2.getOwner(), spec2);
    ASSERT_TRUE(gen2.getGeneral());
    ASSERT_EQ(*gen2.getGeneral(), base2);
    ASSERT_EQ(gen2.getTargets().size(), 1);
    ASSERT_EQ(gen2.getTargets().front(), base2);
    ASSERT_TRUE(gen2.getRelatedElements().count(base2.getID()));
    ASSERT_EQ(gen2.getRelatedElements().get(base2.getID()), base2);

    ID genID = gen2.getID();
    m.release(gen2, spec2);
    Generalization& gen3 = m.aquire(genID)->as<Generalization>();
    ASSERT_TRUE(gen3.getSpecific());
    Class& spec3 = gen3.getSpecific()->as<Class>();
    ASSERT_EQ(gen3.getSources().size(), 1);
    ASSERT_EQ(gen3.getSources().front(), spec3);
    ASSERT_EQ(gen3.getRelatedElements().size(), 2);
    ASSERT_TRUE(gen3.getRelatedElements().count(spec3.getID()));
    ASSERT_EQ(gen3.getRelatedElements().get(spec3.getID()), spec3);
    ASSERT_TRUE(gen3.getOwner());
    ASSERT_EQ(*gen3.getOwner(), spec3);
    ASSERT_TRUE(gen3.getGeneral());
    ASSERT_EQ(*gen3.getGeneral(), base2);
    ASSERT_EQ(gen3.getTargets().size(), 1);
    ASSERT_EQ(gen3.getTargets().front(), base2);
    ASSERT_TRUE(gen3.getRelatedElements().count(base2.getID()));
    ASSERT_EQ(gen3.getRelatedElements().get(base2.getID()), base2);

    m.release(nestSpec);
    Class& nestSpec2 = spec3.getNestedClassifiers().front().as<Class>();
    ASSERT_TRUE(nestSpec2.getNamespace());
    ASSERT_EQ(*nestSpec2.getNamespace(), spec3);
    ASSERT_TRUE(nestSpec2.getOwner());
    ASSERT_EQ(*nestSpec2.getOwner(), spec3);
    
    ASSERT_EQ(spec3.getNestedClassifiers().size(), 1);
    ASSERT_EQ(spec3.getNestedClassifiers().front(), nestSpec2);
    ASSERT_TRUE(spec3.getOwnedMembers().count(nestSpec2.getID()));
    ASSERT_TRUE(spec3.getMembers().count(nestSpec2.getID()));
    ASSERT_TRUE(spec3.getOwnedElements().count(nestSpec2.getID()));

    ID nestSpecID = nestSpec2.getID();
    ID specID = spec3.getID();
    m.release(spec3, nestSpec2);
    ASSERT_FALSE(m.loaded(specID));
    Class& nestSpec3 = m.aquire(nestSpecID)->as<Class>();
    ASSERT_TRUE(nestSpec3.getNamespace());
    Class& spec4 = nestSpec3.getNamespace()->as<Class>();
    ASSERT_EQ(*nestSpec3.getNamespace(), spec4);
    ASSERT_TRUE(nestSpec3.getOwner());
    ASSERT_EQ(*nestSpec3.getOwner(), spec4);
    ASSERT_EQ(spec4.getNestedClassifiers().size(), 1);
    ASSERT_EQ(spec4.getNestedClassifiers().front(), nestSpec3);
    ASSERT_TRUE(spec4.getOwnedMembers().count(nestSpec3.getID()));
    ASSERT_TRUE(spec4.getMembers().count(nestSpec3.getID()));
    ASSERT_TRUE(spec4.getOwnedElements().count(nestSpec3.getID()));

    m.release(op);
    ASSERT_EQ(base2.getOwnedOperations().size(), 1);
    Operation& op2 = base2.getOwnedOperations().front();
    ASSERT_TRUE(op2.getClass());
    ASSERT_EQ(*op2.getClass(), base2);
    ASSERT_TRUE(op2.getFeaturingClassifier());
    ASSERT_EQ(*op2.getFeaturingClassifier(), base2);
    ASSERT_TRUE(op2.getNamespace());
    ASSERT_EQ(*op2.getNamespace(), base2);
    ASSERT_TRUE(op2.getOwner());
    ASSERT_EQ(*op2.getOwner(), base2);

    ASSERT_TRUE(base2.getFeatures().count(op2.getID()));
    ASSERT_TRUE(base2.getOwnedMembers().count(op2.getID()));
    ASSERT_TRUE(base2.getMembers().count(op2.getID()));
    ASSERT_TRUE(base2.getOwnedElements().count(op2.getID()));

    ID opID = op2.getID();
    m.release(base2, op2);
    Operation& op3 = m.aquire(opID)->as<Operation>();
    ASSERT_TRUE(op3.getClass());
    Class& base3 = *op3.getClass();
    ASSERT_TRUE(op3.getFeaturingClassifier());
    ASSERT_EQ(*op3.getFeaturingClassifier(), base3);
    ASSERT_TRUE(op3.getNamespace());
    ASSERT_EQ(*op3.getNamespace(), base3);
    ASSERT_TRUE(op3.getOwner());
    ASSERT_EQ(*op3.getOwner(), base3);

    ASSERT_TRUE(base3.getFeatures().count(op3.getID()));
    ASSERT_TRUE(base3.getOwnedMembers().count(op3.getID()));
    ASSERT_TRUE(base3.getMembers().count(op3.getID()));
    ASSERT_TRUE(base3.getOwnedElements().count(op3.getID()));

    ID propID = prop.getID();
    m.release(prop);
    ASSERT_TRUE(base3.getOwnedAttributes().count(propID));
    Property& prop2 = base3.getOwnedAttributes().front();
    ASSERT_TRUE(prop2.getClass());
    ASSERT_EQ(*prop2.getClass(), base3);
    ASSERT_TRUE(prop2.getFeaturingClassifier());
    ASSERT_EQ(*prop2.getFeaturingClassifier(), base3);
    ASSERT_TRUE(prop2.getNamespace());
    ASSERT_EQ(*prop2.getNamespace(), base3);
    ASSERT_TRUE(prop2.getOwner());
    ASSERT_EQ(*prop2.getOwner(), base3);
    ASSERT_EQ(base3.getOwnedAttributes().front(), prop2);
    ASSERT_EQ(base3.getAttributes().front(), prop2);
    ASSERT_TRUE(base3.getFeatures().contains(prop2));
    ASSERT_TRUE(base3.getOwnedMembers().contains(prop2));
    ASSERT_TRUE(base3.getMembers().contains(prop2));
    ASSERT_TRUE(base3.getMembers().contains(prop2));

    m.release(base3, prop2);
    Property& prop3 = m.aquire(propID)->as<Property>();
    ASSERT_TRUE(prop3.getClass());
    Class& base4 = *prop3.getClass();
    ASSERT_TRUE(prop3.getFeaturingClassifier());
    ASSERT_EQ(*prop3.getFeaturingClassifier(), base4);
    ASSERT_TRUE(prop3.getNamespace());
    ASSERT_EQ(*prop3.getNamespace(), base4);
    ASSERT_TRUE(prop3.getOwner());
    ASSERT_EQ(*prop3.getOwner(), base4);
    ASSERT_TRUE(base4.getOwnedAttributes().contains(prop3));
    ASSERT_EQ(base4.getAttributes().front(), prop3);
    ASSERT_TRUE(base4.getFeatures().contains(prop3));
    ASSERT_TRUE(base4.getOwnedMembers().contains(prop3));
    ASSERT_TRUE(base4.getMembers().contains(prop3));
    ASSERT_TRUE(base4.getOwnedElements().contains(prop3));

    // testing inherited members
    ID baseID = base4.getID();
    m.release(spec4, gen3, base4);
    ASSERT_FALSE(m.loaded(specID));
    ASSERT_FALSE(m.loaded(genID));
    ASSERT_FALSE(m.loaded(baseID));
    Class& spec5 = m.aquire(specID)->as<Class>();
    ASSERT_TRUE(m.loaded(genID));
    ASSERT_TRUE(m.loaded(baseID));
    ASSERT_EQ(spec5.getInheritedMembers().size(), 2);
    ASSERT_TRUE(spec5.getInheritedMembers().contains(prop3));
    ASSERT_TRUE(spec5.getInheritedMembers().contains(op3));
    ASSERT_EQ(spec5.getMembers().size(), 3);
    ASSERT_TRUE(spec5.getMembers().contains(nestSpec3));
    ASSERT_TRUE(spec5.getMembers().contains(prop3));
    ASSERT_TRUE(spec5.getMembers().contains(op3));
}

TEST_F(ClassTest, parseOwnedAttributeTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "structuredClassifierTests/ownedAttributeTest.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::CLASS);
    Class& c = *dynamic_cast<Class*>(el);
    ASSERT_TRUE(c.getOwnedAttributes().size() == 1);
    Property* p = &c.getOwnedAttributes().front();
    ASSERT_TRUE(c.getAttributes().size() == 1);
    ASSERT_TRUE(&c.getAttributes().front() == p);
    ASSERT_TRUE(c.getRoles().size() == 1);
    ASSERT_TRUE(&c.getRoles().front() == p);
    ASSERT_TRUE(c.getFeatures().size() == 1);
    ASSERT_TRUE(&c.getFeatures().front() == p);
    ASSERT_TRUE(c.getOwnedMembers().size() == 1);
    ASSERT_TRUE(&c.getOwnedMembers().front() == p);
    ASSERT_TRUE(c.getMembers().size() == 1);
    ASSERT_TRUE(&c.getMembers().front() == p);

    ASSERT_TRUE(p->getName().compare("test") == 0);
}

TEST_F(ClassTest, partTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "structuredClassifierTests/partTest.yml").ptr());
    Class& c = *dynamic_cast<Class*>(el);
    ASSERT_TRUE(c.getParts().size() == 1);
    ASSERT_TRUE(c.getOwnedAttributes().size() == 1);
    Property* p = &c.getOwnedAttributes().front();
    ASSERT_TRUE(&c.getParts().front() == p);
    ASSERT_TRUE(c.getAttributes().size() == 1);
    ASSERT_TRUE(&c.getAttributes().front() == p);
    ASSERT_TRUE(c.getRoles().size() == 1);
    ASSERT_TRUE(&c.getRoles().front() == p);
    ASSERT_TRUE(c.getFeatures().size() == 1);
    ASSERT_TRUE(&c.getFeatures().front() == p);
    ASSERT_TRUE(c.getOwnedMembers().size() == 1);
    ASSERT_TRUE(&c.getOwnedMembers().front() == p);
    ASSERT_TRUE(c.getMembers().size() == 1);
    ASSERT_TRUE(&c.getMembers().front() == p);

    ASSERT_TRUE(p->getAggregation() == AggregationKind::COMPOSITE);
}

TEST_F(ClassTest, parseConnectorTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "structuredClassifierTests/connector.yml").ptr());

    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package& root = el->as<Package>();
    OpaqueBehavior& contract = root.getPackagedElements().get("contract").as<OpaqueBehavior>();
    Association& association = root.getPackagedElements().get("association").as<Association>();
    Property& assocEnd1 = association.getOwnedEnds().get("end1");
    Property& assocEnd2 = association.getOwnedEnds().get("end2");
    DataType& type1 = root.getPackagedElements().get("type1").as<DataType>();
    DataType& type2 = root.getPackagedElements().get("type2").as<DataType>();
    Class& clazz = root.getPackagedElements().get("clazz").as<Class>();
    ASSERT_EQ(association.getMemberEnds().size(), 2);
    ASSERT_EQ(clazz.getOwnedAttributes().size(), 2);
    Property& prop1 = clazz.getOwnedAttributes().get("prop1");
    Property& prop2 = clazz.getOwnedAttributes().get("prop2");
    Connector& connector = clazz.getOwnedConnectors().front();
    ASSERT_TRUE(connector.getType());
    ASSERT_EQ(*connector.getType(), association);
    ASSERT_EQ(connector.getContracts().size(), 1);
    ASSERT_TRUE(connector.getContracts().contains(contract));
    ASSERT_EQ(connector.getEnds().size(), 2);
    ConnectorEnd& end1 = connector.getEnds().get(ID::fromString("iluwBJEOrucpPYWRjAf2Wl0Y7xJb"));
    ConnectorEnd& end2 = connector.getEnds().get(ID::fromString("Xa_ufGeUWxU5cUPY3f8VfRdmsH1V"));
    ASSERT_TRUE(end1.getRole());
    ASSERT_EQ(*end1.getRole(), prop1);
    ASSERT_TRUE(end1.getDefiningEnd());
    ASSERT_EQ(*end1.getDefiningEnd(), assocEnd1);
    ASSERT_TRUE(end2.getRole());
    ASSERT_EQ(*end2.getRole(), prop2);
    ASSERT_TRUE(end2.getDefiningEnd());
    ASSERT_EQ(*end2.getDefiningEnd(), assocEnd2);
}