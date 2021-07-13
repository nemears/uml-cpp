#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/class.h"
#include "uml/operation.h"
#include "uml/opaqueBehavior.h"
#include "uml/property.h"
#include "uml/literalString.h"
#include "uml/package.h"
#include "uml/generalization.h"
#include "uml/parameter.h"

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
    ASSERT_TRUE(clazz->getID() == ID::fromString("g8WBwHt6sgOqvS9ZlgKv9XTmHZ&C"));
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
    c.setID("hWVMp5upOkVsWnkrfl0I6O5bQsbO");
    c.setName("Class");
    p.setID("61255etITfg0LgPLZaU1PEByTjo3");
    p.setName("prop");
    p.setVisibility(VisibilityKind::PRIVATE);
    c.getOwnedAttributes().add(p);
    string expectedEmit = R""""(class:
  id: hWVMp5upOkVsWnkrfl0I6O5bQsbO
  name: Class
  ownedAttributes:
    - property:
        id: 61255etITfg0LgPLZaU1PEByTjo3
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
    c.setID("b0XPjtodVDLoVu2YCMwBWYqglsoX");
    c.setName("Class");
    p.setID("kfuX2BvkrRFhMX4UAfchHJgL8sER");
    p.setName("prop");
    p.setVisibility(VisibilityKind::PRIVATE);
    o.setID("ESKTcd5FmF2q4O&WI_Oiu5FrXHeN");
    o.setName("op");
    o.setVisibility(VisibilityKind::PROTECTED);
    c.getOwnedAttributes().add(p);
    c.getOperations().add(o);
    string expectedEmit = R""""(class:
  id: b0XPjtodVDLoVu2YCMwBWYqglsoX
  name: Class
  ownedAttributes:
    - property:
        id: kfuX2BvkrRFhMX4UAfchHJgL8sER
        name: prop
        visibility: PRIVATE
  operations:
    - operation:
        id: ESKTcd5FmF2q4O&WI_Oiu5FrXHeN
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
    b.getParameters().add(p2);
    o.getMethods().add(b);
    c.getOperations().add(o);
    string expectedEmit = R""""(class:
  id: 6cCDjqUmkrXZ46z7CcNaTDso4SfQ
  name: Class
  operations:
    - operation:
        id: Y3WV0c_Wa_zfOTb6zo9BAiIqRhn7
        name: op
        visibility: PROTECTED
        methods:
          - opaqueBehavior:
              id: "&_DLItAl_5ASwPNvNVqXaIwEiLOx"
              name: opaque
              parameters:
                - parameter:
                    id: C7lT8BaQxmMi7cnZLIjjWCVD3k_9
                    name: opaquePee
        ownedParameters:
          - parameter:
              id: s2q_fjRnyV_Gst&gAQ4JTr3crFNU
              name: pee)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(c));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}