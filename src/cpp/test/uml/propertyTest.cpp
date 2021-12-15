#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include <test/umlTestUtil.h>
#include "test/yumlParsersTest.h"

using namespace UML;

class PropertyTest : public ::testing::Test {
    public:
        std::string ymlPath = YML_FILES_PATH;
};

TEST_F(PropertyTest, setDefaultValueOfProperTypeTestString) {
  UmlManager m;
    Property& p = m.create<Property>();
    PrimitiveType& stringP = m.create<PrimitiveType>();
    p.setType(&stringP);
    LiteralString& ls = m.create<LiteralString>();
    ls.setValue("test");
    ASSERT_NO_THROW(p.setDefaultValue(&ls));
    ASSERT_TRUE(p.getDefaultValue() == &ls);
    ASSERT_TRUE(p.getOwnedElements().size() == 1);
    ASSERT_TRUE(&p.getOwnedElements().get(ls.getID()) == &ls);
}

TEST_F(PropertyTest, reindexID_forClassiferTest) {
    UmlManager m;
    Class& c = m.create<Class>();
    Property& p = m.create<Property>();
    p.setAggregation(AggregationKind::COMPOSITE);
    c.getOwnedAttributes().add(p);
    ASSERT_NO_THROW(p.setID("190d1cb9_13dc_44e6_a064_1268"));
    ASSERT_NO_THROW(c.getOwnedElements().get(p.getID()));
    ASSERT_NO_THROW(c.getMembers().get(p.getID()));
    ASSERT_NO_THROW(c.getOwnedMembers().get(p.getID()));
    ASSERT_NO_THROW(c.getFeatures().get(p.getID()));
    ASSERT_NO_THROW(c.getAttributes().get(p.getID()));
    ASSERT_NO_THROW(c.getOwnedAttributes().get(p.getID()));
    ASSERT_NO_THROW(c.getRoles().get(p.getID()));
    ASSERT_NO_THROW(c.getParts().get(p.getID()));

    Association& a = m.create<Association>();
    Property& p2 = m.create<Property>();
    a.getNavigableOwnedEnds().add(p2);
    ASSERT_NO_THROW(p2.setID("c0ab87cc_d00b_4afb_9558_5382"));
    ASSERT_NO_THROW(a.getNavigableOwnedEnds().get(p2.getID()));
    ASSERT_NO_THROW(a.getOwnedEnds().get(p2.getID()));
    ASSERT_NO_THROW(a.getMemberEnds().get(p2.getID()));
    ASSERT_NO_THROW(a.getFeatures().get(p2.getID()));
    ASSERT_NO_THROW(a.getOwnedMembers().get(p2.getID()));
    ASSERT_NO_THROW(a.getMembers().get(p2.getID()));
    ASSERT_NO_THROW(a.getOwnedElements().get(p2.getID()));
}

// TEST_F(PropertyTest, reindexNameForClassifierTest) {
//   // Class c;
//   // Property p;
//   // p.setAggregation(AggregationKind::COMPOSITE);
//   // c.getOwnedAttributes().add(p);
//   // ASSERT_NO_THROW(p.setName("test"));
//   // ASSERT_TRUE(c.getOwnedElements().get("test") != 0);
//   // ASSERT_TRUE(c.getMembers().get("test") != 0);
//   // ASSERT_TRUE(c.getOwnedMembers().get("test") != 0);
//   // ASSERT_TRUE(c.getFeatures().get("test") != 0);
//   // ASSERT_TRUE(c.getAttributes().get("test") != NULL);
//   // ASSERT_TRUE(c.getOwnedAttributes().get("test") != 0);
//   // ASSERT_TRUE(c.getRole().get("test") != 0);
//   // ASSERT_TRUE(c.getParts().get("test") != 0);

//   Association a;
//   Property p2;
//   a.getNavigableOwnedEnds().add(p2);
//   ASSERT_NO_THROW(p2.setName("test"));
//   ASSERT_TRUE(a.getNavigableOwnedEnds().get("test"));
//   ASSERT_TRUE(a.getOwnedEnds().get("test"));
//   ASSERT_TRUE(a.getMemberEnds().get("test"));
//   ASSERT_TRUE(a.getFeatures().get("test"));
//   ASSERT_TRUE(a.getOwnedMembers().get("test"));
//   ASSERT_TRUE(a.getMembers().get("test"));
//   ASSERT_TRUE(a.getOwnedElements().get("test"));
// }

TEST_F(PropertyTest, overwriteClassifierTest) {
    UmlManager m;
    Class p1 = m.create<Class>();
    Class p2 = m.create<Class>();
    Property c = m.create<Property>();
    p1.getOwnedAttributes().add(c);
    c.setClass(&p2);
    ASSERT_EQ(p2.getAttributes().size(), 1);
    ASSERT_EQ(p2.getAttributes().front(), c);
    ASSERT_TRUE(c.hasFeaturingClassifier());
    ASSERT_TRUE(c.getFeaturingClassifier());
    ASSERT_EQ(c.getFeaturingClassifierRef(), p2);
    ASSERT_EQ(p1.getAttributes().size(), 0);
}

TEST_F(PropertyTest, overwriteClassifierByAttributesAddTest) {
    UmlManager m;
    Class p1 = m.create<Class>();
    Class p2 = m.create<Class>();
    Property c = m.create<Property>();
    p1.getOwnedAttributes().add(c);
    p2.getOwnedAttributes().add(c);
    ASSERT_EQ(p2.getAttributes().size(), 1);
    ASSERT_EQ(p2.getAttributes().front(), c);
    ASSERT_TRUE(c.hasFeaturingClassifier());
    ASSERT_TRUE(c.getFeaturingClassifier());
    ASSERT_EQ(c.getFeaturingClassifierRef(), p2);
    ASSERT_EQ(p1.getAttributes().size(), 0);
}

TEST_F(PropertyTest, copyPropertyTest) {
    UmlManager m;
    Property p = m.create<Property>();
    p.setName("test");
    p.setStatic(true);
    Class c = m.create<Class>();
    c.getOwnedAttributes().add(p);
    Class t = m.create<Class>();
    p.setType(t);
    Property p2 = p;
    ASSERT_EQ(p2, p);
    ASSERT_EQ(p.getName(), p2.getName());
    ASSERT_TRUE(p2.hasType());
    ASSERT_TRUE(p2.getType());
    ASSERT_EQ(p2.getTypeRef(), t);
    ASSERT_TRUE(p2.hasClass());
    ASSERT_TRUE(p2.getClass());
    ASSERT_EQ(p2.getClassRef(), c);
    ASSERT_TRUE(p2.hasNamespace());
    ASSERT_EQ(p2.getNamespaceRef(), c);
    ASSERT_TRUE(p2.hasFeaturingClassifier());
    ASSERT_EQ(p2.getFeaturingClassifierRef(), c);
    ASSERT_TRUE(p2.hasOwner());
    ASSERT_EQ(p2.getOwnerRef(), c);
    ASSERT_TRUE(p2.isStatic());
}

TEST_F(PropertyTest, redefinePropertyTest) {
  UmlManager m;
  Property& prop = m.create<Property>();
  Property& redefined = m.create<Property>();
  Class& b = m.create<Class>();
  Class& s = m.create<Class>();
  Generalization& gen = m.create<Generalization>();
  s.getGeneralizations().add(gen);
  gen.setGeneral(&b);
  b.getOwnedAttributes().add(redefined);
  s.getOwnedAttributes().add(prop);
  ASSERT_NO_THROW(prop.getRedefinedProperties().add(redefined));
  ASSERT_EQ(prop.getRedefinedProperties().size(), 1);
  ASSERT_EQ(prop.getRedefinedProperties().front().getID(), redefined.getID());
  ASSERT_EQ(prop.getRedefinitionContext().size(), 1);
  ASSERT_EQ(prop.getRedefinitionContext().front().getID(), s.getID());
  ASSERT_EQ(prop.getRedefinedElements().size(), 1);
  ASSERT_EQ(prop.getRedefinedElements().front().getID(), redefined.getID());
  // TODO : restore below
//   Property& notRelated = m.create<Property>();
//   ASSERT_THROW(prop.getRedefinedProperties().add(notRelated), ImproperRedefinitionException);
}

// TEST_F(PropertyTest, copyAndEditTest) {
//     UmlManager m;
//     Class& gen = m.create<Class>();
//     Class& spec = m.create<Class>();
//     Property& og = m.create<Property>();
//     Property& red = m.create<Property>();
//     Generalization generalization = m.create<Generalization>();
//     Package& pckg = m.create<Package>();
//     Class& type = m.create<Class>();
//     InstanceSpecification& typeInst = m.create<InstanceSpecification>();
//     InstanceValue& val = m.create<InstanceValue>();
//     gen.getOwnedAttributes().add(og);
//     spec.getOwnedAttributes().add(red);
//     red.getRedefinedProperties().add(og);
//     red.setType(&type);
//     typeInst.setClassifier(&type);
//     val.setInstance(&typeInst);
//     red.setDefaultValue(&val);
//     og.setType(&type);
//     pckg.getPackagedElements().add(gen, spec, type, typeInst);
//     Property red2 = red;
//     ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SINGLETON_CORRECTLY(red2, red, &Property::getDefaultValue, &TypedElement::getType, &Property::getClass, &Property::getClassifier,& Property::getStructuredClassifier, &Feature::getFeaturingClassifier, &NamedElement::getNamespace, &Element::getOwner));
//     ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SEQUENCE_CORRECTLY(red2, red, &Property::getRedefinedProperties, &RedefinableElement::getRedefinedElements, &RedefinableElement::getRedefinitionContext, &NamedElement::getMemberNamespace));
//     red2.getRedefinedProperties().remove(og);
//     red2.setDefaultValue(0);
//     red2.setType(0);
//     spec.getOwnedAttributes().remove(red2);
//     ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SINGLETON_CORRECTLY(red2, red, &Property::getDefaultValue, &TypedElement::getType, &Property::getClass, &Property::getClassifier, &Property::getStructuredClassifier, &Feature::getFeaturingClassifier, &NamedElement::getNamespace, &Element::getOwner));
//     ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SEQUENCE_CORRECTLY(red2, red, &Property::getRedefinedProperties, &RedefinableElement::getRedefinedElements, &RedefinableElement::getRedefinitionContext, &NamedElement::getMemberNamespace));
//     red2.setType(&type);
//     red2.setDefaultValue(&val);
//     spec.getOwnedAttributes().add(red2);
//     red2.getRedefinedProperties().add(og);
//     ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SINGLETON_CORRECTLY(red2, red, &Property::getDefaultValue, &TypedElement::getType, &Property::getClass, &Property::getClassifier, &Property::getStructuredClassifier, &Feature::getFeaturingClassifier, &NamedElement::getNamespace, &Element::getOwner));
//     ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SEQUENCE_CORRECTLY(red2, red, &Property::getRedefinedProperties, &RedefinableElement::getRedefinedElements, &RedefinableElement::getRedefinitionContext, &NamedElement::getMemberNamespace));
// }

// TEST_F(PropertyTest, reindexRedefinedPropertyTest) {
//     UmlManager m;
//     Class b = m.create<Class>();
//     Class s = m.create<Class>();
//     Generalization g = m.create<Generalization>();
//     Property red = m.create<Property>();
//     Property ov = m.create<Property>(); // override
//     Package root = m.create<Package>();
//     s.getGeneralizations().add(g);
//     g.setGeneral(&b);
//     b.getOwnedAttributes().add(red);
//     s.getOwnedAttributes().add(ov);
//     root.getPackagedElements().add(b, s);
//     ov.getRedefinedProperties().add(red);
//     ID id = ID::fromString("cLvWpn5ofnVR_f2lob3ofVyLu0Fc");
//     red.setID(id);
//     m.setRoot(&root);
//     ASSERT_EQ(ov.getRedefinedProperties().size(), 1);
//     ASSERT_EQ(ov.getRedefinedProperties().front().getID(), id);
//     ASSERT_NO_THROW(ov.getRedefinedProperties().get(id));
//     ASSERT_EQ(ov.getRedefinedElements().size(), 1);
//     ASSERT_EQ(ov.getRedefinedElements().front().getID(), id);
//     ASSERT_NO_THROW(ov.getRedefinedElements().get(id));
//     m.mount(ymlPath + "propertyTests");
//     m.release(red);
//     ASSERT_EQ(ov.getRedefinedProperties().size(), 1);
//     ASSERT_EQ(ov.getRedefinedProperties().front().getID(), id);
//     ASSERT_NO_THROW(ov.getRedefinedProperties().get(id));
//     ASSERT_EQ(ov.getRedefinedElements().size(), 1);
//     ASSERT_EQ(ov.getRedefinedElements().front().getID(), id);
//     ASSERT_NO_THROW(ov.getRedefinedElements().get(id));
// }