#include "gtest/gtest.h"
#include "uml/primitiveType.h"
#include "uml/property.h"
#include "uml/literalString.h"
#include "uml/class.h"
#include "uml/dataType.h"
#include "uml/generalization.h"
#include "uml/umlPtr.h"
#include "test/yumlParsersTest.h"

#include "uml/uml-stable.h"

using namespace UML;

class PropertyTest : public ::testing::Test {
    public:
        std::string ymlPath = YML_FILES_PATH;
};

TEST_F(PropertyTest, setDefaultValueOfProperTypeTestString) {
    UmlManager m;
    Property& p = *m.create<Property>();
    PrimitiveType& stringP = *m.create<PrimitiveType>();
    p.setType(&stringP);
    LiteralString& ls = *m.create<LiteralString>();
    ls.setValue("test");
    ASSERT_NO_THROW(p.setDefaultValue(&ls));
    ASSERT_TRUE(p.getDefaultValue() == &ls);
    ASSERT_TRUE(p.getOwnedElements().size() == 1);
    ASSERT_TRUE(&p.getOwnedElements().get(ls.getID()) == &ls);
}

TEST_F(PropertyTest, reindexID_forClassiferTest) {
    UmlManager m;
    Class& c = *m.create<Class>();
    Property& p = *m.create<Property>();
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

    Association& a = *m.create<Association>();
    Property& p2 = *m.create<Property>();
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

TEST_F(PropertyTest, reindexNameForClassifierTest) {
    UmlManager m;
    Class& c = *m.create<Class>();
    Property& p = *m.create<Property>();
    p.setAggregation(AggregationKind::COMPOSITE);
    c.getOwnedAttributes().add(p);
    ASSERT_NO_THROW(p.setName("test"));
    ASSERT_EQ(c.getOwnedElements().get("test"), p);
    ASSERT_EQ(c.getMembers().get("test"), p);
    ASSERT_EQ(c.getOwnedMembers().get("test"), p);
    ASSERT_EQ(c.getFeatures().get("test"), p);
    ASSERT_EQ(c.getAttributes().get("test"), p);
    ASSERT_EQ(c.getOwnedAttributes().get("test"), p);
    ASSERT_EQ(c.getRoles().get("test"), p);
    ASSERT_EQ(c.getParts().get("test"), p);

    Association& a = *m.create<Association>();
    Property& p2 = *m.create<Property>();;
    a.getNavigableOwnedEnds().add(p2);
    ASSERT_NO_THROW(p2.setName("test"));
    ASSERT_EQ(a.getNavigableOwnedEnds().get("test"), p2);
    ASSERT_EQ(a.getOwnedEnds().get("test"), p2);
    ASSERT_EQ(a.getMemberEnds().get("test"), p2);
    ASSERT_EQ(a.getFeatures().get("test"), p2);
    ASSERT_EQ(a.getOwnedMembers().get("test"), p2);
    ASSERT_EQ(a.getMembers().get("test"), p2);
    ASSERT_EQ(a.getOwnedElements().get("test"), p2);
}

TEST_F(PropertyTest, overwriteClassifierTest) {
    UmlManager m;
    Class& p1 = *m.create<Class>();
    Class& p2 = *m.create<Class>();
    Property& c = *m.create<Property>();
    p1.getOwnedAttributes().add(c);
    c.setClass(&p2);
    ASSERT_EQ(p2.getAttributes().size(), 1);
    ASSERT_EQ(p2.getAttributes().front(), c);
    ASSERT_TRUE(c.getFeaturingClassifier());
    ASSERT_EQ(*c.getFeaturingClassifier(), p2);
    ASSERT_EQ(p1.getAttributes().size(), 0);
}

TEST_F(PropertyTest, overwriteClassifierByAttributesAddTest) {
    UmlManager m;
    Class& p1 = *m.create<Class>();
    Class& p2 = *m.create<Class>();
    Property& c = *m.create<Property>();
    p1.getOwnedAttributes().add(c);
    p2.getOwnedAttributes().add(c);
    ASSERT_EQ(p2.getAttributes().size(), 1);
    ASSERT_EQ(p2.getAttributes().front(), c);
    ASSERT_TRUE(c.getFeaturingClassifier());
    ASSERT_EQ(*c.getFeaturingClassifier(), p2);
    ASSERT_EQ(p1.getAttributes().size(), 0);
}

TEST_F(PropertyTest, redefinePropertyTest) {
    UmlManager m;
    Property& prop = *m.create<Property>();
    prop.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAB");
    Property& redefined = *m.create<Property>();
    prop.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAC");
    Class& b = *m.create<Class>();
    Class& s = *m.create<Class>();
    Generalization& gen = *m.create<Generalization>();
    s.getGeneralizations().add(gen);
    gen.setGeneral(b);
    b.getOwnedAttributes().add(redefined);
    s.getOwnedAttributes().add(prop);
    ASSERT_NO_THROW(prop.getRedefinedProperties().add(redefined));
    ASSERT_EQ(prop.getRedefinedProperties().size(), 1);
    ASSERT_EQ(prop.getRedefinedProperties().front().getID(), redefined.getID());
    ASSERT_EQ(prop.getRedefinitionContext().size(), 1);
    ASSERT_EQ(prop.getRedefinitionContext().front().getID(), s.getID());
    ASSERT_EQ(prop.getRedefinedElements().size(), 1);
    ASSERT_EQ(prop.getRedefinedElements().front().getID(), redefined.getID());

    ASSERT_EQ(b.getOwnedAttributes().size(), 1);
    ASSERT_TRUE(b.getOwnedAttributes().contains(redefined));
    ASSERT_EQ(b.getAttributes().size(), 1);
    ASSERT_TRUE(b.getAttributes().contains(redefined));
    ASSERT_EQ(b.getFeatures().size(), 1);
    ASSERT_TRUE(b.getFeatures().contains(redefined));
    ASSERT_EQ(b.getOwnedMembers().size(), 1);
    ASSERT_TRUE(b.getOwnedMembers().contains(redefined));
    ASSERT_EQ(b.getMembers().size(), 1);
    ASSERT_TRUE(b.getMembers().contains(redefined));
    ASSERT_EQ(b.getOwnedElements().size(), 1);
    ASSERT_TRUE(b.getOwnedElements().contains(redefined));

    ASSERT_EQ(s.getOwnedAttributes().size(), 1);
    ASSERT_TRUE(s.getOwnedAttributes().contains(prop));
    ASSERT_EQ(s.getAttributes().size(), 1);
    ASSERT_TRUE(s.getAttributes().contains(prop));
    ASSERT_EQ(s.getFeatures().size(), 1);
    ASSERT_TRUE(s.getFeatures().contains(prop));
    ASSERT_EQ(s.getOwnedMembers().size(), 1);
    ASSERT_TRUE(s.getOwnedMembers().contains(prop));
    ASSERT_EQ(s.getMembers().size(), 2);
    ASSERT_TRUE(s.getMembers().contains(prop));
    ASSERT_EQ(s.getOwnedElements().size(), 2);
    ASSERT_TRUE(s.getOwnedElements().contains(prop));

  // TODO : restore below
//   Property& notRelated = m.create<Property>();
//   ASSERT_THROW(prop.getRedefinedProperties().add(notRelated), ImproperRedefinitionException);
}

TEST_F(PropertyTest, reindexRedefinedPropertyTest) {
    UmlManager m;
    Class& b = *m.create<Class>();
    Class& s = *m.create<Class>();
    Generalization& g = *m.create<Generalization>();
    Property& red = *m.create<Property>();
    Property& ov = *m.create<Property>(); // override
    Package& root = *m.create<Package>();
    s.getGeneralizations().add(g);
    g.setGeneral(b);
    b.getOwnedAttributes().add(red);
    s.getOwnedAttributes().add(ov);
    root.getPackagedElements().add(b, s);
    ov.getRedefinedProperties().add(red);
    ID id = ID::fromString("cLvWpn5ofnVR_f2lob3ofVyLu0Fc");
    red.setID(id);
    m.setRoot(&root);
    ASSERT_EQ(ov.getRedefinedProperties().size(), 1);
    ASSERT_EQ(ov.getRedefinedProperties().front().getID(), id);
    ASSERT_NO_THROW(ov.getRedefinedProperties().get(id));
    ASSERT_EQ(ov.getRedefinedElements().size(), 1);
    ASSERT_EQ(ov.getRedefinedElements().front().getID(), id);
    ASSERT_NO_THROW(ov.getRedefinedElements().get(id));
    m.mount(ymlPath + "propertyTests");
    m.release(red);
    ASSERT_EQ(ov.getRedefinedProperties().size(), 1);
    ASSERT_EQ(ov.getRedefinedProperties().front().getID(), id);
    ASSERT_NO_THROW(ov.getRedefinedProperties().get(id));
    ASSERT_EQ(ov.getRedefinedElements().size(), 1);
    ASSERT_EQ(ov.getRedefinedElements().front().getID(), id);
    ASSERT_NO_THROW(ov.getRedefinedElements().get(id));
}