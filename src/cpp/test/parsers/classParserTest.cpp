#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/class.h"
#include "uml/operation.h"
#include "uml/opaqueBehavior.h"

using namespace std;
using namespace UML;

class ClassParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(ClassParserTest, parseID_andName) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "classTests/class_w_id_and_name.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::CLASS);
    Class* clazz = dynamic_cast<Class*>(el);
    ASSERT_TRUE(clazz->getID() == ID::fromString("54e8f139-9581-48a4-8021-32ff00606c93"));
    ASSERT_TRUE(clazz->getName().compare("test") == 0);
}

TEST_F(ClassParserTest, parseBasicProperty) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "classTests/classWithAttributes.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::CLASS);
    Class* clazz = dynamic_cast<Class*>(el);
    ASSERT_TRUE(clazz->getAttributes().size() == 2);
    Property* prop1 = dynamic_cast<Property*>(clazz->getAttributes().front());
    Property* prop2 = dynamic_cast<Property*>(clazz->getAttributes().back());
    ASSERT_TRUE(prop1->getClassifier() == clazz);
    // ASSERT_TRUE(prop1->getNamespace() == clazz);
    // ASSERT_TRUE(prop1->getOwner() == clazz);
    ASSERT_TRUE(clazz->getMembers().front() == prop1);
    // ASSERT_TRUE(clazz->getOwnedElements().front() == prop1);
    ASSERT_TRUE(prop2->getClassifier() == clazz);
    //ASSERT_TRUE(prop2->getNamespace() == clazz);
    // ASSERT_TRUE(prop2->getOwner() == clazz);
    ASSERT_TRUE(clazz->getMembers().back() == prop2);
    // ASSERT_TRUE(clazz->getOwnedElements().back() == prop2);
}

TEST_F(ClassParserTest, parseOperation) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "classTests/operation.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::CLASS);
    Class* clazz = dynamic_cast<Class*>(el);
    ASSERT_TRUE(clazz->getOperations().size() == 1);
    Operation* op = clazz->getOperations().front();
    ASSERT_TRUE(op->getName().compare("isValid") == 0);
    ASSERT_TRUE(op->getMethods().size() == 1);
    OpaqueBehavior* bhv = dynamic_cast<OpaqueBehavior*>(op->getMethods().front());
    ASSERT_TRUE(bhv->getName().compare("isValid") == 0);
    ASSERT_TRUE(bhv->getBodies().size() == 1);
    ASSERT_TRUE(bhv->getBodies().front()->getValue().compare("return true") == 0);
    ASSERT_TRUE(bhv->getParameters().size() == 1);
}

TEST_F(ClassParserTest, properErrors) {
    Element* el;
    UmlManager m;
    ASSERT_THROW(el = m.parse(ymlPath + "classTests/improperOperationIdentifier.yml"), Parsers::UmlParserException);
    ASSERT_THROW(el = m.parse(ymlPath + "classTests/operationsNotSequence.yml"), Parsers::UmlParserException);
}

TEST_F(ClassParserTest, basicGeneralizationTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "classTests/basicGeneralization.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 2);
    ASSERT_TRUE(pckg->getPackagedElements().front()->getElementType() == ElementType::CLASS);
    Class* general = dynamic_cast<Class*>(pckg->getPackagedElements().front());
    ASSERT_TRUE(general->getName().compare("general") == 0);
    ASSERT_TRUE(pckg->getPackagedElements().back()->getElementType() == ElementType::CLASS);
    Class* specific = dynamic_cast<Class*>(pckg->getPackagedElements().back());
    ASSERT_TRUE(specific->getName().compare("specific") == 0);
    ASSERT_TRUE(specific->getGeneralizations().size() == 1);
    ASSERT_TRUE(specific->getGenerals().size() == 1);
    ASSERT_TRUE(specific->getGenerals().front() == general);
    Generalization* g = specific->getGeneralizations().front();
    ASSERT_TRUE(g->getGeneral() == general);
    ASSERT_TRUE(g->getSpecific() == specific);
}

TEST_F(ClassParserTest, inheritedMembersTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "classTests/inheritedMembers.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 4);
    ASSERT_TRUE(pckg->getPackagedElements().front()->getElementType() == ElementType::CLASS);
    Class* general = dynamic_cast<Class*>(pckg->getPackagedElements().front());
    ASSERT_TRUE(general->getName().compare("general") == 0);
    ASSERT_TRUE(pckg->getPackagedElements().get(1)->getElementType() == ElementType::CLASS);
    Class* specific = dynamic_cast<Class*>(pckg->getPackagedElements().get(1));
    ASSERT_TRUE(specific->getName().compare("specific") == 0);
    ASSERT_TRUE(specific->getGeneralizations().size() == 1);
    ASSERT_TRUE(specific->getGenerals().size() == 1);
    ASSERT_TRUE(specific->getGenerals().front() == general);
    Generalization* g = specific->getGeneralizations().front();
    ASSERT_TRUE(g->getGeneral() == general);
    ASSERT_TRUE(g->getSpecific() == specific);
    ASSERT_TRUE(general->getOwnedAttributes().size() == 1);
    Property* gProp = general->getOwnedAttributes().front();
    ASSERT_TRUE(specific->getInheritedMembers().size() == 1);
    ASSERT_TRUE(specific->getInheritedMembers().front() == gProp);
    
    ASSERT_TRUE(pckg->getPackagedElements().get(2)->getElementType() == ElementType::CLASS);
    Class* privateGeneral = dynamic_cast<Class*>(pckg->getPackagedElements().get(2));
    ASSERT_TRUE(pckg->getPackagedElements().get(3)->getElementType() == ElementType::CLASS);
    Class* privateSpecific = dynamic_cast<Class*>(pckg->getPackagedElements().get(3));
    ASSERT_TRUE(privateGeneral->getOwnedAttributes().size() == 1);
    ASSERT_TRUE(privateGeneral->getOwnedAttributes().front()->getVisibility() == VisibilityKind::PRIVATE);
    ASSERT_TRUE(privateSpecific->getInheritedMembers().size() == 0);
}

TEST_F(ClassParserTest, emitClassWAttribute) {
    Class c;
    Property p;
    c.setID("b278ca8a-9d1d-45d1-970f-403bc60998b3");
    c.setName("Class");
    p.setID("9c744c8c-ed4e-4c71-9c97-5d3e6115bc24");
    p.setName("prop");
    p.setVisibility(VisibilityKind::PRIVATE);
    c.getOwnedAttributes().add(p);
    string expectedEmit = R""""(class:
  id: b278ca8a-9d1d-45d1-970f-403bc60998b3
  name: Class
  ownedAttributes:
    - property:
        id: 9c744c8c-ed4e-4c71-9c97-5d3e6115bc24
        name: prop
        visibility: PRIVATE)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(c));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(ClassParserTest, emitClassWAttributeNOperation) {
    Class c;
    Property p;
    Operation o;
    c.setID("b278ca8a-9d1d-45d1-970f-403bc60998b3");
    c.setName("Class");
    p.setID("9c744c8c-ed4e-4c71-9c97-5d3e6115bc24");
    p.setName("prop");
    p.setVisibility(VisibilityKind::PRIVATE);
    o.setID("d2a0bcbd-a1aa-4953-9d95-b10a9a322fe3");
    o.setName("op");
    o.setVisibility(VisibilityKind::PROTECTED);
    c.getOwnedAttributes().add(p);
    c.getOperations().add(o);
    string expectedEmit = R""""(class:
  id: b278ca8a-9d1d-45d1-970f-403bc60998b3
  name: Class
  ownedAttributes:
    - property:
        id: 9c744c8c-ed4e-4c71-9c97-5d3e6115bc24
        name: prop
        visibility: PRIVATE
  operations:
    - operation:
        id: d2a0bcbd-a1aa-4953-9d95-b10a9a322fe3
        name: op
        visibility: PROTECTED)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(c));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(ClassParserTest, emitFilledInOperation) {
    Class c;
    Operation o;
    OpaqueBehavior b;
    Parameter p;
    Parameter p2;
    c.setID("b278ca8a-9d1d-45d1-970f-403bc60998b3");
    c.setName("Class");
    o.setID("d2a0bcbd-a1aa-4953-9d95-b10a9a322fe3");
    o.setName("op");
    o.setVisibility(VisibilityKind::PROTECTED);
    b.setID("9c744c8c-ed4e-4c71-9c97-5d3e6115bc24");
    b.setName("opaque");
    p.setID("893245c3-a589-4816-ab7f-a52ab2b98b32");
    p.setName("pee");
    p2.setID("8823326b-4882-460c-a4b2-3a1052ef6eb3");
    p2.setName("opaquePee");
    o.getOwnedParameters().add(p);
    b.getParameters().add(p2);
    o.getMethods().add(b);
    c.getOperations().add(o);
    string expectedEmit = R""""(class:
  id: b278ca8a-9d1d-45d1-970f-403bc60998b3
  name: Class
  operations:
    - operation:
        id: d2a0bcbd-a1aa-4953-9d95-b10a9a322fe3
        name: op
        visibility: PROTECTED
        methods:
          - opaqueBehavior:
              id: 9c744c8c-ed4e-4c71-9c97-5d3e6115bc24
              name: opaque
              parameters:
                - parameter:
                    id: 8823326b-4882-460c-a4b2-3a1052ef6eb3
                    name: opaquePee
        ownedParameters:
          - parameter:
              id: 893245c3-a589-4816-ab7f-a52ab2b98b32
              name: pee)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(c));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}