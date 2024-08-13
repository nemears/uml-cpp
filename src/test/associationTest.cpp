#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"
#include "test/uml-cpp-paths.h"

using namespace UML;

UML_SET_INTEGRATION_TEST(AssociationOwnedEnd, Property, Association, &Association::getOwnedEnds)
UML_SINGLETON_INTEGRATION_TEST(PropertyOwningAssociation, Association, Property, &Property::getOwningAssociation, &Property::setOwningAssociation)
UML_SET_INTEGRATION_TEST(AssociationNavigableOwnedEnd, Property, Association, &Association::getNavigableOwnedEnds)
UML_SET_INTEGRATION_TEST(AssociationMemberEnd, Property, Association, &Association::getMemberEnds)
UML_SINGLETON_INTEGRATION_TEST(PropertyAssociation, Association, Property, &Property::getAssociation, &Property::setAssociation)

class AssociationTest : public ::testing::Test {
   public:
        std::string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(AssociationTest, addMemberEndFunctorTest) {
    Manager<> m;
    Property& p = *m.create<Property>();
    Class& c = *m.create<Class>();
    Association& a = *m.create<Association>();
    p.setType(c);
    ASSERT_NO_THROW(a.getMemberEnds().add(p));
    ASSERT_TRUE(a.getMemberEnds().size() == 1);
    ASSERT_TRUE(a.getMemberEnds().front() == &p);
    ASSERT_TRUE(a.getMembers().size() == 1);
    ASSERT_TRUE(a.getMembers().front() == &p);
    ASSERT_TRUE(a.getEndTypes().size() == 1);
    ASSERT_TRUE(a.getEndTypes().front() == &c);

    ASSERT_TRUE(p.getAssociation() == &a);
}

TEST_F(AssociationTest, setAssociationTest) {
    Manager<> m;
    Property& p = *m.create<Property>();
    Association& a = *m.create<Association>();
    p.setAssociation(a);
    ASSERT_TRUE(a.getMemberEnds().size() == 1);
    ASSERT_TRUE(a.getMemberEnds().front() == &p);
    ASSERT_TRUE(a.getMembers().size() == 1);
    ASSERT_TRUE(a.getMembers().front() == &p);

    ASSERT_EQ(*p.getAssociation(), a);
}

TEST_F(AssociationTest, removeMemberEndFunctor) {
    Manager<> m;
    Property& p = *m.create<Property>();
    Association& a = *m.create<Association>();
    ASSERT_NO_THROW(p.setAssociation(&a));
    ASSERT_NO_THROW(a.getMemberEnds().remove(p));
    ASSERT_EQ(a.getMemberEnds().size(), 0);
    ASSERT_EQ(a.getMembers().size(), 0);
    ASSERT_FALSE(p.getAssociation());
}

TEST_F(AssociationTest, overwriteAssociationW_NullTest) {
    Manager<> m;
    Property& p = *m.create<Property>();
    Association& a = *m.create<Association>();
    ASSERT_NO_THROW(a.getMemberEnds().add(p));
    ASSERT_NO_THROW(p.setAssociation(0));
    ASSERT_EQ(a.getMemberEnds().size(), 0);
    ASSERT_EQ(a.getMembers().size(), 0);
    ASSERT_FALSE(p.getAssociation());
}

TEST_F(AssociationTest, overwriteAssociationW_OtherTest) {
    Manager<> m;
    Property& p = *m.create<Property>();
    Association& a = *m.create<Association>();
    Association& a2 = *m.create<Association>();
    ASSERT_NO_THROW(a.getMemberEnds().add(p));
    ASSERT_NO_THROW(p.setAssociation(a2));
    ASSERT_EQ(a2.getMemberEnds().size(), 1);
    ASSERT_EQ(a2.getMemberEnds().front(), &p);
    ASSERT_EQ(a2.getMembers().size(), 1);
    ASSERT_EQ(a2.getMembers().front(), &p);

    ASSERT_EQ(*p.getAssociation(), a2);

    ASSERT_EQ(a.getMembers().size(), 0);
}

TEST_F(AssociationTest, addOwnedEndTest) {
    Manager<> m;
    Property& p = *m.create<Property>();
    Association& a = *m.create<Association>();
    ASSERT_NO_THROW(a.getOwnedEnds().add(p));
    ASSERT_TRUE(a.getOwnedEnds().size() == 1);
    ASSERT_TRUE(a.getOwnedEnds().front() == &p);
    ASSERT_TRUE(a.getOwnedMembers().size() == 1);
    ASSERT_TRUE(a.getOwnedMembers().front() == &p);
    ASSERT_TRUE(a.getOwnedElements().size() == 1);
    ASSERT_TRUE(*a.getOwnedElements().begin() == p);
    ASSERT_TRUE(a.getFeatures().size() == 1);
    ASSERT_TRUE(a.getFeatures().front() == &p);
    ASSERT_TRUE(a.getMemberEnds().size() == 1);
    ASSERT_TRUE(a.getMemberEnds().front() == &p);

    ASSERT_EQ(*p.getOwningAssociation(), a);
    ASSERT_EQ(*p.getAssociation(), a);
    ASSERT_EQ(*p.getFeaturingClassifier(), a);
    ASSERT_EQ(*p.getNamespace(), a);
    ASSERT_EQ(*p.getOwner(), a);
}

TEST_F(AssociationTest, setOwningAssociationTest) {
    Manager<> m;
    Property& p = *m.create<Property>();
    Association& a = *m.create<Association>();
    ASSERT_NO_THROW(p.setOwningAssociation(a));
    ASSERT_TRUE(a.getOwnedEnds().size() == 1);
    ASSERT_TRUE(a.getOwnedEnds().front() == &p);
    ASSERT_TRUE(a.getOwnedMembers().size() == 1);
    ASSERT_TRUE(a.getOwnedMembers().front() == &p);
    ASSERT_TRUE(a.getOwnedElements().size() == 1);
    ASSERT_TRUE(*a.getOwnedElements().begin() == p);
    ASSERT_TRUE(a.getFeatures().size() == 1);
    ASSERT_TRUE(a.getFeatures().front() == &p);
    ASSERT_TRUE(a.getMemberEnds().size() == 1);
    ASSERT_TRUE(a.getMemberEnds().front() == &p);

    ASSERT_EQ(*p.getOwningAssociation(), a);
    ASSERT_EQ(*p.getAssociation(), a);
    ASSERT_EQ(*p.getFeaturingClassifier(), a);
    ASSERT_EQ(*p.getNamespace(), a);
    ASSERT_EQ(*p.getOwner(), a);
}

TEST_F(AssociationTest, removeOwnedEndTest) {
    Manager<> m;
    Property& p = *m.create<Property>();
    Association& a = *m.create<Association>();
    a.getOwnedEnds().add(p);
    ASSERT_NO_THROW(a.getOwnedEnds().remove(p));
    ASSERT_TRUE(a.getOwnedEnds().size() == 0);
    ASSERT_TRUE(a.getMemberEnds().size() == 0);
    ASSERT_TRUE(a.getFeatures().size() == 0);
    ASSERT_TRUE(a.getOwnedMembers().size() == 0);
    ASSERT_TRUE(a.getMembers().size() == 0);
    ASSERT_TRUE(a.getOwnedElements().size() == 0);

    ASSERT_FALSE(p.getOwningAssociation());
    ASSERT_FALSE(p.getAssociation());
    ASSERT_FALSE(p.getFeaturingClassifier());
    ASSERT_FALSE(p.getNamespace());
    ASSERT_FALSE(p.getOwner());
}

TEST_F(AssociationTest, setOwningAssociationW_NullTest) {
    Manager<> m;
    Property& p = *m.create<Property>();
    Association& a = *m.create<Association>();
    a.getOwnedEnds().add(p);
    ASSERT_NO_THROW(p.setOwningAssociation(0));
    ASSERT_TRUE(a.getOwnedEnds().size() == 0);
    ASSERT_TRUE(a.getMemberEnds().size() == 0);
    ASSERT_TRUE(a.getFeatures().size() == 0);
    ASSERT_TRUE(a.getOwnedMembers().size() == 0);
    ASSERT_TRUE(a.getMembers().size() == 0);
    ASSERT_TRUE(a.getOwnedElements().size() == 0);

    ASSERT_FALSE(p.getOwningAssociation());
    ASSERT_FALSE(p.getAssociation());
    ASSERT_FALSE(p.getFeaturingClassifier());
    ASSERT_FALSE(p.getNamespace());
    ASSERT_FALSE(p.getOwner());
}

TEST_F(AssociationTest, overwriteOwningAssociationTest) {
    Manager<> m;
    Property& p = *m.create<Property>();
    Association& a = *m.create<Association>();
    Association& a2 = *m.create<Association>();
    a.getOwnedEnds().add(p);
    ASSERT_NO_THROW(p.setOwningAssociation(&a2));
    ASSERT_TRUE(a.getOwnedEnds().size() == 0);
    ASSERT_TRUE(a.getMemberEnds().size() == 0);
    ASSERT_TRUE(a.getFeatures().size() == 0);
    ASSERT_TRUE(a.getOwnedMembers().size() == 0);
    ASSERT_TRUE(a.getMembers().size() == 0);
    ASSERT_TRUE(a.getOwnedElements().size() == 0);

    ASSERT_TRUE(a2.getOwnedEnds().size() == 1);
    ASSERT_TRUE(a2.getOwnedEnds().front() == &p);
    ASSERT_TRUE(a2.getOwnedMembers().size() == 1);
    ASSERT_TRUE(a2.getOwnedMembers().front() == &p);
    ASSERT_TRUE(a2.getOwnedElements().size() == 1);
    ASSERT_TRUE(*a2.getOwnedElements().begin() == p);
    ASSERT_TRUE(a2.getFeatures().size() == 1);
    ASSERT_TRUE(a2.getFeatures().front() == &p);
    ASSERT_TRUE(a2.getMemberEnds().size() == 1);
    ASSERT_TRUE(a2.getMemberEnds().front() == &p);

    ASSERT_EQ(*p.getOwningAssociation(), a2);
    ASSERT_EQ(*p.getAssociation(), a2);
    ASSERT_EQ(*p.getFeaturingClassifier(), a2);
    ASSERT_EQ(*p.getNamespace(), a2);
    ASSERT_EQ(*p.getOwner(), a2);
}

TEST_F(AssociationTest, addNavigableOwnedEndTest) {
    Manager<> m;
    Property& p = *m.create<Property>();
    Association& a = *m.create<Association>();
    a.getNavigableOwnedEnds().add(p);
    ASSERT_TRUE(a.getNavigableOwnedEnds().size() == 1);
    ASSERT_TRUE(a.getNavigableOwnedEnds().front() == &p);
    ASSERT_TRUE(a.getOwnedEnds().size() == 1);
    ASSERT_TRUE(a.getOwnedEnds().front() == &p);
    ASSERT_TRUE(a.getOwnedMembers().size() == 1);
    ASSERT_TRUE(a.getOwnedMembers().front() == &p);
    ASSERT_TRUE(a.getOwnedElements().size() == 1);
    ASSERT_TRUE(*a.getOwnedElements().begin() == p);
    ASSERT_TRUE(a.getFeatures().size() == 1);
    ASSERT_TRUE(a.getFeatures().front() == &p);
    ASSERT_TRUE(a.getMemberEnds().size() == 1);
    ASSERT_TRUE(a.getMemberEnds().front() == &p);

    ASSERT_EQ(*p.getOwningAssociation(), a);
    ASSERT_EQ(*p.getAssociation(), a);
    ASSERT_EQ(*p.getFeaturingClassifier(), a);
    ASSERT_EQ(*p.getNamespace(), a);
    ASSERT_EQ(*p.getOwner(), a);
}

TEST_F(AssociationTest, removeNavigableOwnedEndTest) {
    Manager<> m;
    Property& p = *m.create<Property>();
    Association& a = *m.create<Association>();
    a.getNavigableOwnedEnds().add(p);
    ASSERT_NO_THROW(a.getNavigableOwnedEnds().remove(p));
    ASSERT_TRUE(a.getNavigableOwnedEnds().size() == 0);
    ASSERT_TRUE(a.getOwnedEnds().size() == 0);
    ASSERT_TRUE(a.getMemberEnds().size() == 0);
    ASSERT_TRUE(a.getFeatures().size() == 0);
    ASSERT_TRUE(a.getOwnedMembers().size() == 0);
    ASSERT_TRUE(a.getMembers().size() == 0);
    ASSERT_TRUE(a.getOwnedElements().size() == 0);

    ASSERT_FALSE(p.getOwningAssociation());
    ASSERT_FALSE(p.getAssociation());
    ASSERT_FALSE(p.getFeaturingClassifier());
    ASSERT_FALSE(p.getNamespace());
    ASSERT_FALSE(p.getOwner());
}

TEST_F(AssociationTest, navigableOwnedEndSetOwningAssociationW_NullTest) {
    Manager<> m;
    Property& p = *m.create<Property>();
    Association& a = *m.create<Association>();
    a.getNavigableOwnedEnds().add(p);
    ASSERT_NO_THROW(p.setOwningAssociation(0));
    ASSERT_TRUE(a.getNavigableOwnedEnds().size() == 0);
    ASSERT_TRUE(a.getOwnedEnds().size() == 0);
    ASSERT_TRUE(a.getMemberEnds().size() == 0);
    ASSERT_TRUE(a.getFeatures().size() == 0);
    ASSERT_TRUE(a.getOwnedMembers().size() == 0);
    ASSERT_TRUE(a.getMembers().size() == 0);
    ASSERT_TRUE(a.getOwnedElements().size() == 0);

    ASSERT_FALSE(p.getOwningAssociation());
    ASSERT_FALSE(p.getAssociation());
    ASSERT_FALSE(p.getFeaturingClassifier());
    ASSERT_FALSE(p.getNamespace());
    ASSERT_FALSE(p.getOwner());
}

TEST_F(AssociationTest, navigableOwnedEndOverwriteOwningAssociationTest) {
    Manager<> m;
    Property& p = *m.create<Property>();
    Association& a = *m.create<Association>();
    Association& a2 = *m.create<Association>();
    a.getNavigableOwnedEnds().add(p);
    ASSERT_NO_THROW(p.setOwningAssociation(&a2));
    ASSERT_TRUE(a.getNavigableOwnedEnds().size() == 0);
    ASSERT_TRUE(a.getOwnedEnds().size() == 0);
    ASSERT_TRUE(a.getMemberEnds().size() == 0);
    ASSERT_TRUE(a.getFeatures().size() == 0);
    ASSERT_TRUE(a.getOwnedMembers().size() == 0);
    ASSERT_TRUE(a.getMembers().size() == 0);
    ASSERT_TRUE(a.getOwnedElements().size() == 0);

    ASSERT_TRUE(a2.getNavigableOwnedEnds().size() == 0);
    ASSERT_TRUE(a2.getOwnedEnds().size() == 1);
    ASSERT_TRUE(a2.getOwnedEnds().front() == &p);
    ASSERT_TRUE(a2.getOwnedMembers().size() == 1);
    ASSERT_TRUE(a2.getOwnedMembers().front() == &p);
    ASSERT_TRUE(a2.getOwnedElements().size() == 1);
    ASSERT_TRUE(*a2.getOwnedElements().begin() == p);
    ASSERT_TRUE(a2.getFeatures().size() == 1);
    ASSERT_TRUE(a2.getFeatures().front() == &p);
    ASSERT_TRUE(a2.getMemberEnds().size() == 1);
    ASSERT_TRUE(a2.getMemberEnds().front() == &p);

    ASSERT_EQ(*p.getOwningAssociation(), a2);
    ASSERT_EQ(*p.getAssociation(), a2);
    ASSERT_EQ(*p.getFeaturingClassifier(), a2);
    ASSERT_EQ(*p.getNamespace(), a2);
    ASSERT_EQ(*p.getOwner(), a2);
}

TEST_F(AssociationTest, reindexTypeTest) {
    Manager<> m;
    Property& p = *m.create<Property>();
    Association& a = *m.create<Association>();
    Class& c = *m.create<Class>();
    a.getMemberEnds().add(p);
    ASSERT_NO_THROW(p.setType(c));

    ASSERT_TRUE(a.getMemberEnds().size() == 1);
    ASSERT_TRUE(a.getMemberEnds().front() == &p);
    ASSERT_TRUE(a.getMembers().size() == 1);
    ASSERT_TRUE(a.getMembers().front() == &p);
    ASSERT_TRUE(a.getEndTypes().size() == 1);
    ASSERT_TRUE(a.getEndTypes().front() == &c);

    ASSERT_EQ(*p.getAssociation(), a);
}

TEST_F(AssociationTest, parseNavigableOwnedEndAndMemberEndTest) {
    Manager<> m;
    m.open(ymlPath + "uml/primitiveTypes.yml");
    ASSERT_NO_THROW(m.open(ymlPath + "associationTests/navigableEnd.yml"));
    ASSERT_EQ(m.getRoot()->getElementType(), ElementType::PACKAGE);
    Package& pckg = m.getRoot()->as<Package>();
    ASSERT_EQ(pckg.getPackageMerge().size(), 1);
    // PackageMerge& merge = pckg.getPackageMerge().front();
    // TODO?

    ASSERT_EQ(pckg.getPackagedElements().size(), 3);
    ASSERT_EQ(pckg.getPackagedElements().get("type")->getElementType(), ElementType::CLASS);
    Class& type = m.get(pckg.getPackagedElements().get("type")->getID())->as<Class>();
    ASSERT_EQ(pckg.getPackagedElements().get("hasAssociation")->getElementType(), ElementType::CLASS);
    Class& c = pckg.getPackagedElements().get("hasAssociation")->as<Class>();;
    ASSERT_EQ(c.getOwnedAttributes().size(), 1);
    Property& p = *c.getOwnedAttributes().front();
    ASSERT_TRUE(p.getType());
    ASSERT_EQ(p.getType()->getID(), type.getID());
    ASSERT_EQ(pckg.getPackagedElements().get("association")->getElementType(), ElementType::ASSOCIATION);
    Association& ass = pckg.getPackagedElements().get("association")->as<Association>();
    ASSERT_EQ(ass.getMemberEnds().size(), 2);
    ASSERT_TRUE(ass.getMemberEnds().contains(p));
    // ASSERT_EQ(ass.getMemberEnds().back().getID(), p.getID());
    ASSERT_EQ(ass.getNavigableOwnedEnds().size(), 1);
    Property& ne = *ass.getNavigableOwnedEnds().front();
    ASSERT_TRUE(ne.getType());
    ASSERT_EQ(ne.getType()->getID(), ID::fromString("real_aZG&w6yl61bXVWutgeyScN9"));
}

TEST_F(AssociationTest, parseOwnedEndAndMemberEndTest) {
    Manager<> m;
    m.open(ymlPath + "uml/primitiveTypes.yml");
    ASSERT_NO_THROW(m.open(ymlPath + "associationTests/ownedEnd.yml"));
    ASSERT_EQ(m.getRoot()->getElementType(), ElementType::PACKAGE);
    Package& pckg = m.getRoot()->as<Package>();
    ASSERT_EQ(pckg.getPackageMerge().size(), 1);
    // PackageMerge& merge = pckg.getPackageMerge().front();
    // TODO?

    ASSERT_EQ(pckg.getPackagedElements().size(), 3);
    ASSERT_EQ(pckg.getPackagedElements().get("type")->getElementType(), ElementType::CLASS);
    Class& type = m.get(pckg.getPackagedElements().get("type")->getID())->as<Class>();
    ASSERT_EQ(pckg.getPackagedElements().get("hasAssociation")->getElementType(), ElementType::CLASS);
    Class& c = pckg.getPackagedElements().get("hasAssociation")->as<Class>();
    ASSERT_EQ(c.getOwnedAttributes().size(), 1);
    Property& p = *c.getOwnedAttributes().front();
    ASSERT_TRUE(p.getType());
    ASSERT_EQ(p.getType()->getID(), type.getID());
    ASSERT_EQ(pckg.getPackagedElements().get("association")->getElementType(), ElementType::ASSOCIATION);
    Association& ass = pckg.getPackagedElements().get("association")->as<Association>();
    ASSERT_EQ(ass.getMemberEnds().size(), 2);
    ASSERT_EQ(ass.getMemberEnds().get(ID::fromString("KpMoyOIGjkm&tB8F_zGExQejc7T7"))->getID(), p.getID());
    ASSERT_EQ(ass.getOwnedEnds().size(), 1);
    Property& ne = *ass.getOwnedEnds().front();
    ASSERT_TRUE(ne.getType());
    ASSERT_EQ(ne.getType()->getID(), ID::fromString("real_aZG&w6yl61bXVWutgeyScN9"));
}

TEST_F(AssociationTest, emitAssociationTest) {
    Manager<> m;
    Package& pckg = *m.create<Package>();
    Class& clazz = *m.create<Class>();
    Property& prop = *m.create<Property>();
    PrimitiveType& type = *m.create<PrimitiveType>();
    Association& associtaion = *m.create<Association>();
    Property& end = *m.create<Property>();
    pckg.setID("zN&UM2AHrXX07rAiNxTmmMwLYI1O");
    clazz.setID("mGbq9i_gGHuMFYg0y3tMzcmHx1B3");
    prop.setID("FqaulNq6bCe_8J5M0Ff2oCCaQD05");
    type.setID("m8K65o0wEqtIznmEPmuXaTph2JJu");
    associtaion.setID("FZeUbleSO7P_Zqwn2&r8HKnEbSU5");
    end.setID("k&CQ7BNYYbkhtw_g7NaNY8wUHXYs");
    clazz.getOwnedAttributes().add(prop);
    prop.setType(&type);
    associtaion.getMemberEnds().add(prop);
    end.setType(&clazz);
    associtaion.getNavigableOwnedEnds().add(end);
    pckg.getPackagedElements().add(clazz);
    pckg.getPackagedElements().add(type);
    pckg.getPackagedElements().add(associtaion);
    std::string expectedEmit = R""""(Package:
  id: "zN&UM2AHrXX07rAiNxTmmMwLYI1O"
  packagedElements:
    - Class:
        id: mGbq9i_gGHuMFYg0y3tMzcmHx1B3
        ownedAttributes:
          - Property:
              id: FqaulNq6bCe_8J5M0Ff2oCCaQD05
              type: m8K65o0wEqtIznmEPmuXaTph2JJu
              association: "FZeUbleSO7P_Zqwn2&r8HKnEbSU5"
    - Association:
        id: "FZeUbleSO7P_Zqwn2&r8HKnEbSU5"
        memberEnds:
          - FqaulNq6bCe_8J5M0Ff2oCCaQD05
        navigableOwnedEnds:
          - Property:
              id: "k&CQ7BNYYbkhtw_g7NaNY8wUHXYs"
              type: mGbq9i_gGHuMFYg0y3tMzcmHx1B3
    - PrimitiveType:
        id: m8K65o0wEqtIznmEPmuXaTph2JJu)"""";
    std::string generatedEmit;
    EmitterData data;
    data.mode = SerializationMode::WHOLE;
    ASSERT_NO_THROW(generatedEmit = emit(pckg, data));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(AssociationTest, mountAndEditAssociation) {
    Manager<> m;
    Package& root = *m.create<Package>();
    Class& clazz = *m.create<Class>();
    Class& type = *m.create<Class>();
    Property& cProp = *m.create<Property>();
    Property& aProp = *m.create<Property>();
    Association& association = *m.create<Association>();
    cProp.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAD");
    cProp.setAggregation(AggregationKind::COMPOSITE);
    cProp.setType(type);
    association.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAC");
    association.getMemberEnds().add(cProp);
    aProp.setID("AAAAAAAAAAAAAAAAAAAAAAAAAAAB");
    aProp.setType(clazz);
    association.getNavigableOwnedEnds().add(aProp);
    clazz.getOwnedAttributes().add(cProp);
    root.getPackagedElements().add(clazz, type, association);
    m.setRoot(&root);
    m.mount(ymlPath + "associationTests");

    ID aPropID = aProp.getID();
    m.release(aProp);
    Property& aProp2 = m.get(aPropID)->as<Property>();
    ASSERT_TRUE(aProp2.getOwningAssociation());
    ASSERT_EQ(*aProp2.getOwningAssociation(), association);
    ASSERT_TRUE(aProp2.getAssociation());
    ASSERT_EQ(*aProp2.getAssociation(), association);
    ASSERT_TRUE(aProp2.getFeaturingClassifier());
    ASSERT_EQ(*aProp2.getFeaturingClassifier(), association);
    ASSERT_TRUE(aProp2.getNamespace());
    ASSERT_EQ(*aProp2.getNamespace(), association);
    ASSERT_TRUE(aProp2.getOwner());
    ASSERT_EQ(*aProp2.getOwner(), association);
    ASSERT_TRUE(aProp2.getType());
    ASSERT_EQ(*aProp2.getType(), clazz);
    ASSERT_EQ(association.getNavigableOwnedEnds().size(), 1);
    ASSERT_EQ(association.getNavigableOwnedEnds().front(), &aProp2);
    ASSERT_EQ(association.getOwnedEnds().size(), 1);
    ASSERT_EQ(association.getOwnedEnds().front(), &aProp2);
    ASSERT_EQ(association.getMemberEnds().size(), 2);
    ASSERT_TRUE(association.getMemberEnds().contains(aProp2));
    ASSERT_EQ(association.getOwnedMembers().size(), 1);
    ASSERT_EQ(association.getOwnedMembers().front(), &aProp2);
    ASSERT_EQ(association.getMembers().size(), 2);
    ASSERT_TRUE(association.getMembers().contains(aProp2));
    ASSERT_EQ(association.getOwnedElements().size(), 1);
    ASSERT_EQ(*association.getOwnedElements().begin(), aProp2);
    ASSERT_EQ(association.getEndTypes().size(), 2);
    ASSERT_TRUE(association.getEndTypes().contains(clazz));

    ID associationID = association.getID();
    m.release(aProp2, association);
    Association& association2 = m.get(associationID)->as<Association>();
    ASSERT_TRUE(m.loaded(aPropID));
    Property& aProp3 = m.get(aPropID)->as<Property>();
    ASSERT_TRUE(aProp3.getOwningAssociation());
    ASSERT_EQ(*aProp3.getOwningAssociation(), association2);
    ASSERT_TRUE(aProp3.getAssociation());
    ASSERT_EQ(*aProp3.getAssociation(), association2);
    ASSERT_TRUE(aProp3.getFeaturingClassifier());
    ASSERT_EQ(*aProp3.getFeaturingClassifier(), association2);
    ASSERT_TRUE(aProp3.getNamespace());
    ASSERT_EQ(*aProp3.getNamespace(), association2);
    ASSERT_TRUE(aProp3.getOwner());
    ASSERT_EQ(*aProp3.getOwner(), association2);
    ASSERT_EQ(association2.getNavigableOwnedEnds().size(), 1);
    ASSERT_EQ(association2.getNavigableOwnedEnds().front(), &aProp3);
    ASSERT_EQ(association2.getOwnedEnds().size(), 1);
    ASSERT_TRUE(association2.getOwnedEnds().contains(aProp3));
    ASSERT_EQ(association2.getMemberEnds().size(), 2);
    ASSERT_TRUE(association2.getMemberEnds().contains(aProp3));
    ASSERT_TRUE(association2.getOwnedMembers().contains(aProp3));
    ASSERT_EQ(association2.getMembers().size(), 2);
    ASSERT_TRUE(association2.getMembers().contains(aProp3));
    ASSERT_EQ(association2.getOwnedElements().size(), 1);
    ASSERT_TRUE(association2.getOwnedElements().contains(aProp3));
    ASSERT_EQ(association2.getEndTypes().size(), 2);
    ASSERT_TRUE(association2.getEndTypes().contains(type));
    ASSERT_TRUE(association2.getEndTypes().contains(clazz));

    ID cPropID = cProp.getID();
    m.release(cProp);
    Property& cProp2 = m.get(cPropID)->as<Property>();
    ASSERT_TRUE(cProp2.isComposite());
    ASSERT_TRUE(cProp2.getAssociation());
    ASSERT_EQ(*cProp2.getAssociation(), association2);
    ASSERT_EQ(association2.getMemberEnds().size(), 2);
    ASSERT_TRUE(association2.getMemberEnds().contains(cProp2));
    ASSERT_EQ(association2.getMembers().size(), 2);
    ASSERT_TRUE(association2.getMembers().contains(cProp2));
    ASSERT_EQ(association2.getEndTypes().size(), 2);
    ASSERT_TRUE(association2.getEndTypes().contains(type));
    ASSERT_TRUE(association2.getEndTypes().contains(clazz));
    ASSERT_EQ(clazz.getParts().size(), 1);
    ASSERT_EQ(clazz.getParts().front(), &cProp2);
    ASSERT_EQ(clazz.getOwnedAttributes().size(), 1);
    ASSERT_EQ(clazz.getOwnedAttributes().front(), &cProp2);
    ASSERT_EQ(clazz.getAttributes().size(), 1);
    ASSERT_EQ(clazz.getAttributes().front(), &cProp2);
    ASSERT_EQ(clazz.getFeatures().size(), 1);
    ASSERT_EQ(clazz.getFeatures().front(), &cProp2);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(cProp2, clazz));

    m.release(association2, cProp2);
    Association& association3 = m.get(associationID)->as<Association>();
    ASSERT_TRUE(m.loaded(cPropID));
    Property& cProp3 = m.get(cPropID)->as<Property>();
    ASSERT_TRUE(cProp3.isComposite());
    ASSERT_TRUE(cProp3.getAssociation());
    ASSERT_EQ(*cProp3.getAssociation(), association3);
    ASSERT_EQ(association3.getMemberEnds().size(), 2);
    ASSERT_TRUE(association3.getMemberEnds().contains(cProp3));
    ASSERT_EQ(association3.getMembers().size(), 2);
    ASSERT_TRUE(association3.getMembers().contains(cProp3));
    ASSERT_EQ(association3.getEndTypes().size(), 2);
    ASSERT_TRUE(association3.getEndTypes().contains(type));
    ASSERT_TRUE(association3.getEndTypes().contains(clazz));

    ID clazzID = clazz.getID();
    m.release(association3, clazz, aProp3, cProp3);
    Association& association4 = m.get(associationID)->as<Association>();
    ASSERT_EQ(association4.getEndTypes().size(), 2);
    ASSERT_TRUE(m.loaded(aPropID));
    ASSERT_FALSE(m.loaded(clazzID));
    Property& aProp4 = *association4.getNavigableOwnedEnds().front();
    ASSERT_TRUE(aProp4.getType());
    Class& clazz2 = m.get(clazzID)->as<Class>();
    ASSERT_EQ(*aProp4.getType(), clazz2);
    Property& cProp4 = m.get(cPropID)->as<Property>();
    ASSERT_EQ(clazz2.getParts().size(), 1);
    ASSERT_EQ(clazz2.getParts().front(), &cProp4);
    ASSERT_EQ(clazz2.getOwnedAttributes().size(), 1);
    ASSERT_EQ(clazz2.getOwnedAttributes().front(), &cProp4);
    ASSERT_EQ(clazz2.getAttributes().size(), 1);
    ASSERT_EQ(clazz2.getAttributes().front(), &cProp4);
    ASSERT_EQ(clazz2.getFeatures().size(), 1);
    ASSERT_EQ(clazz2.getFeatures().front(), &cProp4);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(cProp4, clazz2));

    m.release(clazz2, cProp4);
    Class& clazz3 = m.get(clazzID)->as<Class>();
    ASSERT_TRUE(m.loaded(cPropID));
    Property& cProp5 = m.get(cPropID)->as<Property>();
    ASSERT_EQ(clazz3.getParts().size(), 1);
    ASSERT_EQ(clazz3.getParts().front(), &cProp5);
    ASSERT_EQ(clazz3.getOwnedAttributes().size(), 1);
    ASSERT_EQ(clazz3.getOwnedAttributes().front(), &cProp5);
    ASSERT_EQ(clazz3.getAttributes().size(), 1);
    ASSERT_EQ(clazz3.getAttributes().front(), &cProp5);
    ASSERT_EQ(clazz3.getFeatures().size(), 1);
    ASSERT_EQ(clazz3.getFeatures().front(), &cProp5);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(cProp5, clazz3));
}
