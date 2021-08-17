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
#include "uml/association.h"
#include "uml/artifact.h"
#include "uml/primitiveType.h"
#include "uml/enumeration.h"

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
    Property* prop1 = dynamic_cast<Property*>(&clazz->getAttributes().front());
    Property* prop2 = dynamic_cast<Property*>(&clazz->getAttributes().back());
    ASSERT_TRUE(prop1->getClassifier() == clazz);
    // ASSERT_TRUE(prop1->getNamespace() == clazz);
    // ASSERT_TRUE(prop1->getOwner() == clazz);
    ASSERT_TRUE(&clazz->getMembers().front() == prop1);
    // ASSERT_TRUE(clazz->getOwnedElements().front() == prop1);
    ASSERT_TRUE(prop2->getClassifier() == clazz);
    //ASSERT_TRUE(prop2->getNamespace() == clazz);
    // ASSERT_TRUE(prop2->getOwner() == clazz);
    ASSERT_TRUE(&clazz->getMembers().back() == prop2);
    // ASSERT_TRUE(clazz->getOwnedElements().back() == prop2);
}

TEST_F(ClassParserTest, parseOperation) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "classTests/operation.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::CLASS);
    Class* clazz = dynamic_cast<Class*>(el);
    ASSERT_TRUE(clazz->getOperations().size() == 1);
    Operation* op = &clazz->getOperations().front();
    ASSERT_TRUE(op->getName().compare("isValid") == 0);
    ASSERT_TRUE(op->getMethods().size() == 1);
    OpaqueBehavior* bhv = dynamic_cast<OpaqueBehavior*>(&op->getMethods().front());
    ASSERT_TRUE(bhv->getName().compare("isValid") == 0);
    ASSERT_TRUE(bhv->getBodies().size() == 1);
    ASSERT_TRUE(bhv->getBodies().front().getValue().compare("return true") == 0);
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

TEST_F(ClassParserTest, inheritedMembersTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "classTests/inheritedMembers.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_TRUE(pckg->getPackagedElements().size() == 4);
    ASSERT_TRUE(pckg->getPackagedElements().front().getElementType() == ElementType::CLASS);
    Class* general = dynamic_cast<Class*>(&pckg->getPackagedElements().front());
    ASSERT_TRUE(general->getName().compare("general") == 0);
    ASSERT_TRUE(pckg->getPackagedElements().get(1).getElementType() == ElementType::CLASS);
    Class* specific = dynamic_cast<Class*>(&pckg->getPackagedElements().get(1));
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
    Class* privateGeneral = dynamic_cast<Class*>(&pckg->getPackagedElements().get(2));
    ASSERT_TRUE(pckg->getPackagedElements().get(3).getElementType() == ElementType::CLASS);
    Class* privateSpecific = dynamic_cast<Class*>(&pckg->getPackagedElements().get(3));
    ASSERT_TRUE(privateGeneral->getOwnedAttributes().size() == 1);
    ASSERT_TRUE(privateGeneral->getOwnedAttributes().front().getVisibility() == VisibilityKind::PRIVATE);
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

TEST_F(ClassParserTest, nestedClassifierParsingTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "classTests/nestedClassifiers.yml"));
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

TEST_F(ClassParserTest, nestedClassifierEmitTest) {
    UmlManager m;
    Class& clazz = m.create<Class>();
    Artifact& artifact = m.create<Artifact>();
    Association& association = m.create<Association>();
    Class& nestedClazz = m.create<Class>();
    DataType& dataType = m.create<DataType>();
    Enumeration& enumeration = m.create<Enumeration>();
    OpaqueBehavior& opaqueBehavior = m.create<OpaqueBehavior>();
    PrimitiveType& primitiveType = m.create<PrimitiveType>();
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
    string expectedEmit = R""""(class:
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
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(clazz));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(ClassParserTest, mountFullClassTest) {
    UmlManager m;
    Package& pckg = m.create<Package>();
    Class& base = m.create<Class>();
    Class& spec = m.create<Class>();
    Property& prop = m.create<Property>();
    Operation& op = m.create<Operation>();
    Generalization& gen = m.create<Generalization>();
    base.getOwnedAttributes().add(prop);
    base.getOperations().add(op);
    gen.setGeneral(&base);
    gen.setSpecific(&spec);
    pckg.getPackagedElements().add(base);
    pckg.getPackagedElements().add(spec);
    m.setRoot(&pckg);
    string mountPath = ymlPath + "classTests";
    ASSERT_NO_THROW(m.mount(mountPath));
    string mountPath1 = mountPath + "/mount";
    ASSERT_TRUE(filesystem::exists(mountPath1));
    string pckgDirPath = mountPath1 + "/" + pckg.getID().string();
    ASSERT_TRUE(filesystem::exists(pckgDirPath));
    string pckgFilePPath = pckgDirPath + "/" + pckg.getID().string() + ".yml";
    ASSERT_TRUE(filesystem::exists(pckgFilePPath));
    string baseDirPath = pckgDirPath + "/" + pckg.getPackagedElements().front().getID().string();
    ASSERT_TRUE(filesystem::exists(baseDirPath));
    ASSERT_TRUE(filesystem::exists(baseDirPath + "/" + pckg.getPackagedElements().front().getID().string() + ".yml"));
    // TODO finish

    ASSERT_NO_THROW(m.release(base.getID()));
    ASSERT_TRUE(prop.getOwner() != 0);
    Class& base2 = prop.getOwner()->as<Class>();
    ASSERT_TRUE(prop.getClass() != 0);
    ASSERT_EQ(prop.getClass(), &base2);
    ASSERT_TRUE(prop.getClassifier() != 0);
    ASSERT_EQ(prop.getClassifier(), &base2);
    ASSERT_TRUE(prop.getStructuredClassifier() != 0);
    ASSERT_EQ(prop.getStructuredClassifier(), &base2);
    ASSERT_TRUE(prop.getFeaturingClassifier() != 0);
    ASSERT_EQ(prop.getFeaturingClassifier(), &base2);
    ASSERT_TRUE(prop.getNamespace() != 0);
    ASSERT_EQ(prop.getNamespace(), &base2);
    ASSERT_EQ(prop.getMemberNamespace().size(), 2);
    ASSERT_EQ(&prop.getMemberNamespace().front(), &base2);

    ASSERT_EQ(base2.getOwnedAttributes().size(), 1);
    ASSERT_EQ(&base2.getOwnedAttributes().front(), &prop);
    ASSERT_EQ(base2.getAttributes().size(), 1);
    ASSERT_EQ(&base2.getOwnedAttributes().front(), &prop);
    ASSERT_EQ(base2.getFeatures().size(), 2);
    ASSERT_EQ(&base2.getFeatures().front(), &prop);
    ASSERT_EQ(base2.getOwnedMembers().size(), 2);
    ASSERT_EQ(&base2.getOwnedMembers().front(), &prop);
    ASSERT_EQ(base2.getMembers().size(), 2);
    ASSERT_EQ(&base2.getMembers().front(), &prop);
    ASSERT_EQ(base2.getOwnedElements().size(), 2);
    ASSERT_EQ(&base2.getOwnedElements().front(), &prop);

    ASSERT_TRUE(op.getClass() != 0);
    ASSERT_EQ(op.getClass(), &base2);
    ASSERT_TRUE(op.getFeaturingClassifier() != 0);
    ASSERT_EQ(op.getFeaturingClassifier(), &base2);
    ASSERT_TRUE(op.getNamespace() != 0);
    ASSERT_EQ(op.getNamespace(), &base2);
    ASSERT_EQ(op.getMemberNamespace().size(), 2);
    ASSERT_EQ(&op.getMemberNamespace().front(), &base2);
    ASSERT_TRUE(op.getOwner() != 0);
    ASSERT_EQ(op.getOwner(), &base2);

    ASSERT_EQ(base2.getOperations().size(), 1);
    ASSERT_EQ(&base2.getOperations().front(), &op);
    ASSERT_EQ(&base2.getFeatures().get(1), &op);
    ASSERT_EQ(&base2.getOwnedMembers().get(1), &op);
    ASSERT_EQ(&base2.getMembers().get(1), &op);
    ASSERT_EQ(&base2.getOwnedElements().get(1), &op);

    ASSERT_TRUE(gen.getGeneral() != 0);
    ASSERT_EQ(gen.getGeneral(), &base2);
    ASSERT_EQ(spec.getGenerals().size(), 1);
    ASSERT_EQ(&spec.getGenerals().front(), &base2);

    /** TODO: anything else to test with base class? **/
    /** release specific **/
    ASSERT_NO_THROW(m.release(spec.getID()));
    Class& spec2 = pckg.getPackagedElements().get(1).as<Class>(); // load specific

    ASSERT_TRUE(gen.getSpecific() != 0);
    ASSERT_EQ(gen.getSpecific(), &spec2);
    ASSERT_EQ(gen.getSources().size(), 1);
    ASSERT_EQ(&gen.getSources().front(), &spec2);
    ASSERT_EQ(gen.getRelatedElements().size(), 2);
    ASSERT_EQ(&gen.getRelatedElements().back(), &spec2);
    ASSERT_TRUE(gen.getOwner() != 0);
    ASSERT_EQ(gen.getOwner(), &spec2);

    ASSERT_EQ(spec2.getGeneralizations().size(), 1);
    ASSERT_EQ(&spec2.getGeneralizations().front(), &gen);
    ASSERT_EQ(spec2.getDirectedRelationships().size(),1);
    ASSERT_EQ(&spec2.getDirectedRelationships().front(), &gen);
    ASSERT_EQ(spec2.getRelationships().size(), 1);
    ASSERT_EQ(&spec2.getRelationships().front(), &gen);
    ASSERT_EQ(spec2.getGenerals().size(), 1);
    ASSERT_EQ(&spec2.getGenerals().front(), &base2);
    ASSERT_EQ(spec2.getInheritedMembers().size(), 2);
    ASSERT_EQ(&spec2.getInheritedMembers().front(), &prop);
    ASSERT_EQ(&spec2.getInheritedMembers().get(1), &op);
    ASSERT_EQ(spec2.getMembers().size(), 2);
    ASSERT_EQ(&spec2.getMembers().front(), &prop);
    ASSERT_EQ(&spec2.getMembers().get(1), &op);
}