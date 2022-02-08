#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

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

TEST_F(ClassParserTest, parseOperation) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "classTests/operation.yml"));
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

TEST_F(ClassParserTest, emitClassWAttribute) {
    UmlManager m;
    Class& c = m.create<Class>();
    Property& p = m.create<Property>();
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
    UmlManager m;
    Class& c = m.create<Class>();
    Property& p = m.create<Property>();
    Operation& o = m.create<Operation>();
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
    string expectedEmit = R""""(class:
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
    string generatedEmit;
    generatedEmit = Parsers::emit(c);
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(ClassParserTest, emitFilledInOperation) {
    UmlManager m;
    Class& c = m.create<Class>();
    Operation& o = m.create<Operation>();
    OpaqueBehavior& b = m.create<OpaqueBehavior>();
    Parameter& p = m.create<Parameter>();
    Parameter& p2 = m.create<Parameter>();
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
    string expectedEmit = R""""(class:
  id: 6cCDjqUmkrXZ46z7CcNaTDso4SfQ
  name: Class
  ownedBehaviors:
    - opaqueBehavior:
        id: "&_DLItAl_5ASwPNvNVqXaIwEiLOx"
        name: opaque
        parameters:
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
    // std::cout << "!!!!!!!!!!\nTODO uncomment me por favor\n!!!!!!!!!!!!" << std::endl;
    UmlManager m;
    Package& pckg = m.create<Package>();
    Class& base = m.create<Class>();
    Class& spec = m.create<Class>();
    Class& nestSpec = m.create<Class>();
    Property& prop = m.create<Property>();
    Operation& op = m.create<Operation>();
    Generalization& gen = m.create<Generalization>();
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
    string mountPath = ymlPath + "classTests";
    ASSERT_NO_THROW(m.mount(mountPath));
    ASSERT_NO_FATAL_FAILURE(ASSERT_PROPER_MOUNT(pckg, ymlPath + "classTests"));

    ASSERT_NO_THROW(m.release(base.getID()));
    ASSERT_TRUE(prop.getOwner() != 0);
    Class& base2 = prop.getOwner()->as<Class>();
    ASSERT_TRUE(prop.getClass() != 0);
    ASSERT_EQ(prop.getClass(), &base2);
    ASSERT_TRUE(prop.getFeaturingClassifier() != 0);
    ASSERT_EQ(prop.getFeaturingClassifier(), &base2);
    ASSERT_TRUE(prop.getNamespace() != 0);
    ASSERT_EQ(prop.getNamespace(), &base2);

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

    ASSERT_TRUE(op.getClass() != 0);
    ASSERT_EQ(op.getClass(), &base2);
    ASSERT_TRUE(op.getFeaturingClassifier() != 0);
    ASSERT_EQ(op.getFeaturingClassifier(), &base2);
    ASSERT_TRUE(op.getNamespace() != 0);
    ASSERT_EQ(op.getNamespace(), &base2);
    ASSERT_TRUE(op.getOwner() != 0);
    ASSERT_EQ(op.getOwner(), &base2);

    ASSERT_EQ(base2.getOwnedOperations().size(), 1);
    ASSERT_EQ(&base2.getOwnedOperations().front(), &op);
    ASSERT_EQ(&base2.getFeatures().get("operation"), &op);
    ASSERT_EQ(&base2.getOwnedMembers().get("operation"), &op);
    ASSERT_EQ(&base2.getMembers().get("operation"), &op);
    ASSERT_EQ(base2.getOwnedElements().get("operation"), op);

    ASSERT_TRUE(gen.getGeneral() != 0);
    ASSERT_EQ(gen.getGeneral(), &base2);
    ASSERT_EQ(spec.getGenerals().size(), 1);
    ASSERT_EQ(&spec.getGenerals().front(), &base2);

    /** TODO: anything else to test with base class? **/
    /** release specific **/
    ASSERT_NO_THROW(m.release(spec));
    Class& spec2 = pckg.getPackagedElements().get("specific").as<Class>(); // load specific

    ASSERT_TRUE(gen.getSpecific() != 0);
    ASSERT_EQ(gen.getSpecific(), &spec2);
    ASSERT_EQ(gen.getSources().size(), 1);
    ASSERT_EQ(&gen.getSources().front(), &spec2);
    ASSERT_EQ(gen.getRelatedElements().size(), 2);
    ASSERT_EQ(&gen.getRelatedElements().back(), &spec2);
    ASSERT_TRUE(gen.getOwner() != 0);
    ASSERT_EQ(gen.getOwner(), &spec2);

    ASSERT_TRUE(nestSpec.getNamespace() != 0);
    ASSERT_TRUE(nestSpec.getOwner() != 0);
    ASSERT_EQ(nestSpec.getOwner(), &spec2);

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
    ASSERT_TRUE(gen2.hasSpecific());
    ASSERT_EQ(gen2.getSpecificRef(), spec2);
    ASSERT_EQ(gen2.getSources().size(), 1);
    ASSERT_EQ(gen2.getSources().front(), spec2);
    ASSERT_EQ(gen2.getRelatedElements().size(), 2);
    ASSERT_TRUE(gen2.getRelatedElements().count(spec2.getID()));
    ASSERT_EQ(gen2.getRelatedElements().get(spec2.getID()), spec2);
    ASSERT_TRUE(gen2.hasOwner());
    ASSERT_EQ(gen2.getOwnerRef(), spec2);
    ASSERT_TRUE(gen2.hasGeneral());
    ASSERT_EQ(gen2.getGeneralRef(), base2);
    ASSERT_EQ(gen2.getTargets().size(), 1);
    ASSERT_EQ(gen2.getTargets().front(), base2);
    ASSERT_TRUE(gen2.getRelatedElements().count(base2.getID()));
    ASSERT_EQ(gen2.getRelatedElements().get(base2.getID()), base2);

    ID genID = gen2.getID();
    m.release(gen2, spec2);
    Generalization& gen3 = m.aquire(genID)->as<Generalization>();
    ASSERT_TRUE(gen3.hasSpecific());
    Class& spec3 = gen3.getSpecificRef().as<Class>();
    ASSERT_EQ(gen3.getSources().size(), 1);
    ASSERT_EQ(gen3.getSources().front(), spec3);
    ASSERT_EQ(gen3.getRelatedElements().size(), 2);
    ASSERT_TRUE(gen3.getRelatedElements().count(spec3.getID()));
    ASSERT_EQ(gen3.getRelatedElements().get(spec3.getID()), spec3);
    ASSERT_TRUE(gen3.hasOwner());
    ASSERT_EQ(gen3.getOwnerRef(), spec3);
    ASSERT_TRUE(gen3.hasGeneral());
    ASSERT_EQ(gen3.getGeneralRef(), base2);
    ASSERT_EQ(gen3.getTargets().size(), 1);
    ASSERT_EQ(gen3.getTargets().front(), base2);
    ASSERT_TRUE(gen3.getRelatedElements().count(base2.getID()));
    ASSERT_EQ(gen3.getRelatedElements().get(base2.getID()), base2);

    m.release(nestSpec);
    Class& nestSpec2 = spec3.getNestedClassifiers().front().as<Class>();
    ASSERT_TRUE(nestSpec2.hasNamespace());
    ASSERT_EQ(nestSpec2.getNamespaceRef(), spec3);
    ASSERT_TRUE(nestSpec2.hasOwner());
    ASSERT_EQ(nestSpec2.getOwnerRef(), spec3);
    
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
    ASSERT_TRUE(nestSpec3.hasNamespace());
    Class& spec4 = nestSpec3.getNamespaceRef().as<Class>();
    ASSERT_EQ(nestSpec3.getNamespaceRef(), spec4);
    ASSERT_TRUE(nestSpec3.hasOwner());
    ASSERT_EQ(nestSpec3.getOwnerRef(), spec4);
    ASSERT_EQ(spec4.getNestedClassifiers().size(), 1);
    ASSERT_EQ(spec4.getNestedClassifiers().front(), nestSpec3);
    ASSERT_TRUE(spec4.getOwnedMembers().count(nestSpec3.getID()));
    ASSERT_TRUE(spec4.getMembers().count(nestSpec3.getID()));
    ASSERT_TRUE(spec4.getOwnedElements().count(nestSpec3.getID()));

    m.release(op);
    ASSERT_EQ(base2.getOwnedOperations().size(), 1);
    Operation& op2 = base2.getOwnedOperations().front();
    ASSERT_TRUE(op2.hasClass());
    ASSERT_EQ(op2.getClassRef(), base2);
    ASSERT_TRUE(op2.hasFeaturingClassifier());
    ASSERT_EQ(op2.getFeaturingClassifierRef(), base2);
    ASSERT_TRUE(op2.hasNamespace());
    ASSERT_EQ(op2.getNamespaceRef(), base2);
    ASSERT_TRUE(op2.hasOwner());
    ASSERT_EQ(op2.getOwnerRef(), base2);

    ASSERT_TRUE(base2.getFeatures().count(op2.getID()));
    ASSERT_TRUE(base2.getOwnedMembers().count(op2.getID()));
    ASSERT_TRUE(base2.getMembers().count(op2.getID()));
    ASSERT_TRUE(base2.getOwnedElements().count(op2.getID()));

    ID opID = op2.getID();
    m.release(base2, op2);
    Operation& op3 = m.aquire(opID)->as<Operation>();
    ASSERT_TRUE(op3.hasClass());
    Class& base3 = op3.getClassRef();
    ASSERT_TRUE(op3.hasFeaturingClassifier());
    ASSERT_EQ(op3.getFeaturingClassifierRef(), base3);
    ASSERT_TRUE(op3.hasNamespace());
    ASSERT_EQ(op3.getNamespaceRef(), base3);
    ASSERT_TRUE(op3.hasOwner());
    ASSERT_EQ(op3.getOwnerRef(), base3);

    ASSERT_TRUE(base3.getFeatures().count(op3.getID()));
    ASSERT_TRUE(base3.getOwnedMembers().count(op3.getID()));
    ASSERT_TRUE(base3.getMembers().count(op3.getID()));
    ASSERT_TRUE(base3.getOwnedElements().count(op3.getID()));

    ID propID = prop.getID();
    m.release(prop);
    ASSERT_TRUE(base3.getOwnedAttributes().count(propID));
    Property& prop2 = base3.getOwnedAttributes().front();
    ASSERT_TRUE(prop2.hasClass());
    ASSERT_EQ(prop2.getClassRef(), base3);
    ASSERT_TRUE(prop2.hasFeaturingClassifier());
    ASSERT_EQ(prop2.getFeaturingClassifierRef(), base3);
    ASSERT_TRUE(prop2.hasNamespace());
    ASSERT_EQ(prop2.getNamespaceRef(), base3);
    ASSERT_TRUE(prop2.hasOwner());
    ASSERT_EQ(prop2.getOwnerRef(), base3);
    ASSERT_EQ(base3.getOwnedAttributes().front(), prop2);
    ASSERT_EQ(base3.getAttributes().front(), prop2);
    ASSERT_TRUE(base3.getFeatures().contains(prop2));
    ASSERT_TRUE(base3.getOwnedMembers().contains(prop2));
    ASSERT_TRUE(base3.getMembers().contains(prop2));
    ASSERT_TRUE(base3.getMembers().contains(prop2));

    m.release(base3, prop2);
    Property& prop3 = m.aquire(propID)->as<Property>();
    ASSERT_TRUE(prop3.hasClass());
    Class& base4 = prop3.getClassRef();
    ASSERT_TRUE(prop3.hasFeaturingClassifier());
    ASSERT_EQ(prop3.getFeaturingClassifierRef(), base4);
    ASSERT_TRUE(prop3.hasNamespace());
    ASSERT_EQ(prop3.getNamespaceRef(), base4);
    ASSERT_TRUE(prop3.hasOwner());
    ASSERT_EQ(prop3.getOwnerRef(), base4);
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