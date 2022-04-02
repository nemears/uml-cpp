#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/uml-cpp-paths.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

using namespace std;
using namespace UML;

class AssociationParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(AssociationParserTest, parseNavigableOwnedEndAndMemberEndTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "associationTests/navigableEnd.yml").ptr());
    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package& pckg = el->as<Package>();
    ASSERT_EQ(pckg.getPackageMerge().size(), 1);
    PackageMerge& merge = pckg.getPackageMerge().front();
    // TODO?

    ASSERT_EQ(pckg.getPackagedElements().size(), 3);
    ASSERT_EQ(pckg.getPackagedElements().get("type").getElementType(), ElementType::CLASS);
    Class& type = m.get(pckg.getPackagedElements().get("type").getID()).as<Class>();
    ASSERT_EQ(pckg.getPackagedElements().get("hasAssociation").getElementType(), ElementType::CLASS);
    Class& c = *dynamic_cast<Class*>(&pckg.getPackagedElements().get("hasAssociation"));
    ASSERT_EQ(c.getOwnedAttributes().size(), 1);
    Property& p = c.getOwnedAttributes().front();
    ASSERT_TRUE(p.getType());
    ASSERT_EQ(p.getType()->getID(), type.getID());
    ASSERT_EQ(pckg.getPackagedElements().get("association").getElementType(), ElementType::ASSOCIATION);
    Association& ass = dynamic_cast<Association&>(pckg.getPackagedElements().get("association"));
    ASSERT_EQ(ass.getMemberEnds().size(), 2);
    ASSERT_EQ(ass.getMemberEnds().back().getID(), p.getID());
    ASSERT_EQ(ass.getNavigableOwnedEnds().size(), 1);
    Property& ne = ass.getNavigableOwnedEnds().front();
    ASSERT_TRUE(ne.getType());
    ASSERT_EQ(ne.getType()->getID(), ID::fromString("real_aZG&w6yl61bXVWutgeyScN9"));
}

TEST_F(AssociationParserTest, parseOwnedEndAndMemberEndTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "associationTests/ownedEnd.yml").ptr());
    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package& pckg = m.get(el->getID()).as<Package>();
    ASSERT_EQ(pckg.getPackageMerge().size(), 1);
    PackageMerge& merge = pckg.getPackageMerge().front();
    // TODO?

    ASSERT_EQ(pckg.getPackagedElements().size(), 3);
    ASSERT_EQ(pckg.getPackagedElements().get("type").getElementType(), ElementType::CLASS);
    Class& type = m.get(pckg.getPackagedElements().get("type").getID()).as<Class>();
    ASSERT_EQ(pckg.getPackagedElements().get("hasAssociation").getElementType(), ElementType::CLASS);
    Class& c = *dynamic_cast<Class*>(&pckg.getPackagedElements().get("hasAssociation"));
    ASSERT_EQ(c.getOwnedAttributes().size(), 1);
    Property& p = c.getOwnedAttributes().front();
    ASSERT_TRUE(p.getType());
    ASSERT_EQ(p.getType()->getID(), type.getID());
    ASSERT_EQ(pckg.getPackagedElements().get("association").getElementType(), ElementType::ASSOCIATION);
    Association& ass = dynamic_cast<Association&>(pckg.getPackagedElements().get("association"));
    ASSERT_EQ(ass.getMemberEnds().size(), 2);
    ASSERT_EQ(ass.getMemberEnds().get(ID::fromString("KpMoyOIGjkm&tB8F_zGExQejc7T7")).getID(), p.getID());
    ASSERT_EQ(ass.getOwnedEnds().size(), 1);
    Property& ne = ass.getOwnedEnds().front();
    ASSERT_TRUE(ne.getType());
    ASSERT_EQ(ne.getType()->getID(), ID::fromString("real_aZG&w6yl61bXVWutgeyScN9"));
}

TEST_F(AssociationParserTest, emitAssociationTest) {
    UmlManager m;
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
    string expectedEmit = R""""(package:
  id: zN&UM2AHrXX07rAiNxTmmMwLYI1O
  packagedElements:
    - class:
        id: mGbq9i_gGHuMFYg0y3tMzcmHx1B3
        ownedAttributes:
          - property:
              id: FqaulNq6bCe_8J5M0Ff2oCCaQD05
              type: m8K65o0wEqtIznmEPmuXaTph2JJu
              association: FZeUbleSO7P_Zqwn2&r8HKnEbSU5
    - primitiveType:
        id: m8K65o0wEqtIznmEPmuXaTph2JJu
    - association:
        id: FZeUbleSO7P_Zqwn2&r8HKnEbSU5
        navigableOwnedEnds:
          - property:
              id: k&CQ7BNYYbkhtw_g7NaNY8wUHXYs
              type: mGbq9i_gGHuMFYg0y3tMzcmHx1B3
              association: FZeUbleSO7P_Zqwn2&r8HKnEbSU5
        memberEnds:
          - FqaulNq6bCe_8J5M0Ff2oCCaQD05)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(pckg));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(AssociationParserTest, mountAndEditAssociation) {
    UmlManager m;
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
    Property& aProp2 = m.aquire(aPropID)->as<Property>();
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
    ASSERT_EQ(association.getNavigableOwnedEnds().front(), aProp2);
    ASSERT_EQ(association.getOwnedEnds().size(), 1);
    ASSERT_EQ(association.getOwnedEnds().front(), aProp2);
    ASSERT_EQ(association.getMemberEnds().size(), 2);
    ASSERT_EQ(association.getMemberEnds().back(), aProp2);
    ASSERT_EQ(association.getOwnedMembers().size(), 1);
    ASSERT_EQ(association.getOwnedMembers().front(), aProp2);
    ASSERT_EQ(association.getMembers().size(), 2);
    ASSERT_TRUE(association.getMembers().contains(aProp2));
    ASSERT_EQ(association.getOwnedElements().size(), 1);
    ASSERT_EQ(*association.getOwnedElements().begin(), aProp2);
    ASSERT_EQ(association.getEndTypes().size(), 2);
    ASSERT_EQ(association.getEndTypes().back(), clazz);

    ID associationID = association.getID();
    m.release(aProp2, association);
    Association& association2 = m.aquire(associationID)->as<Association>();
    ASSERT_TRUE(m.loaded(aPropID));
    Property& aProp3 = m.get(aPropID).as<Property>();
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
    ASSERT_EQ(association2.getNavigableOwnedEnds().front(), aProp3);
    ASSERT_EQ(association2.getOwnedEnds().size(), 1);
    ASSERT_EQ(association2.getOwnedEnds().front(), aProp3);
    ASSERT_EQ(association2.getMemberEnds().size(), 2);
    ASSERT_EQ(association2.getMemberEnds().front(), aProp3);
    ASSERT_EQ(association2.getOwnedMembers().size(), 1);
    ASSERT_EQ(association2.getOwnedMembers().front(), aProp3);
    ASSERT_EQ(association2.getMembers().size(), 2);
    ASSERT_TRUE(association2.getMembers().contains(aProp3));
    ASSERT_EQ(association2.getOwnedElements().size(), 1);
    ASSERT_TRUE(association2.getOwnedElements().contains(aProp3));
    ASSERT_EQ(association2.getEndTypes().size(), 2);
    ASSERT_TRUE(association2.getEndTypes().contains(type));
    ASSERT_TRUE(association2.getEndTypes().contains(clazz));

    ID cPropID = cProp.getID();
    m.release(cProp);
    Property& cProp2 = m.aquire(cPropID)->as<Property>();
    ASSERT_TRUE(cProp2.isComposite());
    ASSERT_TRUE(cProp2.getAssociation());
    ASSERT_EQ(*cProp2.getAssociation(), association2);
    ASSERT_EQ(association2.getMemberEnds().size(), 2);
    ASSERT_EQ(association2.getMemberEnds().back(), cProp2);
    ASSERT_EQ(association2.getMembers().size(), 2);
    ASSERT_TRUE(association2.getMembers().contains(cProp2));
    ASSERT_EQ(association2.getEndTypes().size(), 2);
    ASSERT_TRUE(association2.getEndTypes().contains(type));
    ASSERT_TRUE(association2.getEndTypes().contains(clazz));
    ASSERT_EQ(clazz.getParts().size(), 1);
    ASSERT_EQ(clazz.getParts().front(), cProp2);
    ASSERT_EQ(clazz.getOwnedAttributes().size(), 1);
    ASSERT_EQ(clazz.getOwnedAttributes().front(), cProp2);
    ASSERT_EQ(clazz.getAttributes().size(), 1);
    ASSERT_EQ(clazz.getAttributes().front(), cProp2);
    ASSERT_EQ(clazz.getFeatures().size(), 1);
    ASSERT_EQ(clazz.getFeatures().front(), cProp2);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(cProp2, clazz));

    m.release(association2, cProp2);
    Association& association3 = m.aquire(associationID)->as<Association>();
    ASSERT_TRUE(m.loaded(cPropID));
    Property& cProp3 = m.get(cPropID).as<Property>();
    ASSERT_TRUE(cProp3.isComposite());
    ASSERT_TRUE(cProp3.getAssociation());
    ASSERT_EQ(*cProp3.getAssociation(), association3);
    ASSERT_EQ(association3.getMemberEnds().size(), 2);
    ASSERT_EQ(association3.getMemberEnds().back(), cProp3);
    ASSERT_EQ(association3.getMembers().size(), 2);
    ASSERT_TRUE(association3.getMembers().contains(cProp3));
    ASSERT_EQ(association3.getEndTypes().size(), 2);
    ASSERT_TRUE(association3.getEndTypes().contains(type));
    ASSERT_TRUE(association3.getEndTypes().contains(clazz));

    ID clazzID = clazz.getID();
    m.release(association3, clazz, aProp3, cProp3);
    Association& association4 = m.aquire(associationID)->as<Association>();
    ASSERT_EQ(association4.getEndTypes().size(), 2);
    ASSERT_TRUE(m.loaded(aPropID));
    ASSERT_FALSE(m.loaded(clazzID));
    Property& aProp4 = association4.getNavigableOwnedEnds().front();
    ASSERT_TRUE(aProp4.getType());
    Class& clazz2 = m.aquire(clazzID)->as<Class>();
    ASSERT_EQ(*aProp4.getType(), clazz2);
    Property& cProp4 = m.get(cPropID).as<Property>();
    ASSERT_EQ(clazz2.getParts().size(), 1);
    ASSERT_EQ(clazz2.getParts().front(), cProp4);
    ASSERT_EQ(clazz2.getOwnedAttributes().size(), 1);
    ASSERT_EQ(clazz2.getOwnedAttributes().front(), cProp4);
    ASSERT_EQ(clazz2.getAttributes().size(), 1);
    ASSERT_EQ(clazz2.getAttributes().front(), cProp4);
    ASSERT_EQ(clazz2.getFeatures().size(), 1);
    ASSERT_EQ(clazz2.getFeatures().front(), cProp4);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(cProp4, clazz2));

    m.release(clazz2, cProp4);
    Class& clazz3 = m.aquire(clazzID)->as<Class>();
    ASSERT_TRUE(m.loaded(cPropID));
    Property& cProp5 = m.get(cPropID).as<Property>();
    ASSERT_EQ(clazz3.getParts().size(), 1);
    ASSERT_EQ(clazz3.getParts().front(), cProp5);
    ASSERT_EQ(clazz3.getOwnedAttributes().size(), 1);
    ASSERT_EQ(clazz3.getOwnedAttributes().front(), cProp5);
    ASSERT_EQ(clazz3.getAttributes().size(), 1);
    ASSERT_EQ(clazz3.getAttributes().front(), cProp5);
    ASSERT_EQ(clazz3.getFeatures().size(), 1);
    ASSERT_EQ(clazz3.getFeatures().front(), cProp5);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(cProp5, clazz3));
}