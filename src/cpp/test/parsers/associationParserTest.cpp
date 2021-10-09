#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/association.h"
#include "uml/packageMerge.h"
#include "uml/package.h"
#include "uml/class.h"
#include "uml/property.h"
#include "uml/primitiveType.h"

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
    ASSERT_NO_THROW(el = m.parse(ymlPath + "associationTests/navigableEnd.yml"));
    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package& pckg = m.get<Package>(el->getID());
    ASSERT_EQ(pckg.getPackageMerge().size(), 1);
    PackageMerge& merge = pckg.getPackageMerge().front();
    // TODO?

    ASSERT_EQ(pckg.getPackagedElements().size(), 3);
    ASSERT_EQ(pckg.getPackagedElements().front().getElementType(), ElementType::CLASS);
    Class& type = m.get<Class>(pckg.getPackagedElements().front().getID());
    ASSERT_EQ(pckg.getPackagedElements().get(1).getElementType(), ElementType::CLASS);
    Class& c = *dynamic_cast<Class*>(&pckg.getPackagedElements().get(1));
    ASSERT_EQ(c.getOwnedAttributes().size(), 1);
    Property& p = c.getOwnedAttributes().front();
    ASSERT_TRUE(p.getType() != 0);
    ASSERT_EQ(p.getType()->getID(), type.getID());
    ASSERT_EQ(pckg.getPackagedElements().back().getElementType(), ElementType::ASSOCIATION);
    Association& ass = dynamic_cast<Association&>(pckg.getPackagedElements().back());
    ASSERT_EQ(ass.getMemberEnds().size(), 2);
    ASSERT_EQ(ass.getMemberEnds().back().getID(), p.getID());
    ASSERT_EQ(ass.getNavigableOwnedEnds().size(), 1);
    Property& ne = ass.getNavigableOwnedEnds().front();
    ASSERT_TRUE(ne.getType() != 0);
    ASSERT_EQ(ne.getType()->getID(), ID::fromString("real_aZG&w6yl61bXVWutgeyScN9"));
}

TEST_F(AssociationParserTest, parseOwnedEndAndMemberEndTest) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "associationTests/ownedEnd.yml"));
    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package& pckg = m.get<Package>(el->getID());
    ASSERT_EQ(pckg.getPackageMerge().size(), 1);
    PackageMerge& merge = pckg.getPackageMerge().front();
    // TODO?

    ASSERT_EQ(pckg.getPackagedElements().size(), 3);
    ASSERT_EQ(pckg.getPackagedElements().front().getElementType(), ElementType::CLASS);
    Class& type = m.get<Class>(pckg.getPackagedElements().front().getID());
    ASSERT_EQ(pckg.getPackagedElements().back().getElementType(), ElementType::CLASS);
    Class& c = *dynamic_cast<Class*>(&pckg.getPackagedElements().back());
    ASSERT_EQ(c.getOwnedAttributes().size(), 1);
    Property& p = c.getOwnedAttributes().front();
    ASSERT_TRUE(p.getType() != 0);
    ASSERT_EQ(p.getType()->getID(), type.getID());
    ASSERT_EQ(pckg.getPackagedElements().get(1).getElementType(), ElementType::ASSOCIATION);
    Association& ass = dynamic_cast<Association&>(pckg.getPackagedElements().get(1));
    ASSERT_EQ(ass.getMemberEnds().size(), 2);
    ASSERT_EQ(ass.getMemberEnds().back().getID(), p.getID());
    ASSERT_EQ(ass.getOwnedEnds().size(), 1);
    Property& ne = ass.getOwnedEnds().front();
    ASSERT_TRUE(ne.getType() != 0);
    ASSERT_EQ(ne.getType()->getID(), ID::fromString("real_aZG&w6yl61bXVWutgeyScN9"));
}

TEST_F(AssociationParserTest, emitAssociationTest) {
    UmlManager m;
    Package& pckg = m.create<Package>();
    Class& clazz = m.create<Class>();
    Property& prop = m.create<Property>();
    PrimitiveType& type = m.create<PrimitiveType>();
    Association& associtaion = m.create<Association>();
    Property& end = m.create<Property>();
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
    - primitiveType:
        id: m8K65o0wEqtIznmEPmuXaTph2JJu
    - association:
        id: FZeUbleSO7P_Zqwn2&r8HKnEbSU5
        navigableOwnedEnds:
          - property:
              id: k&CQ7BNYYbkhtw_g7NaNY8wUHXYs
              type: mGbq9i_gGHuMFYg0y3tMzcmHx1B3
        memberEnds:
          - FqaulNq6bCe_8J5M0Ff2oCCaQD05)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(pckg));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(AssociationParserTest, mountAndEditAssociation) {
    UmlManager m;
    Package& root = m.create<Package>();
    Class& clazz = m.create<Class>();
    Class& type = m.create<Class>();
    Property cProp = m.create<Property>();
    Property aProp = m.create<Property>();
    Association& association = m.create<Association>();
    cProp.setAggregation(AggregationKind::COMPOSITE);
    cProp.setType(type);
    association.getMemberEnds().add(cProp);
    aProp.setType(clazz);
    association.getNavigableOwnedEnds().add(aProp);
    clazz.getOwnedAttributes().add(cProp);
    root.getPackagedElements().add(clazz, type, association);
    m.setRoot(&root);
    m.mount(ymlPath + "associationTests");

    ID aPropID = aProp.getID();
    m.release(aProp);
    Property& aProp2 = m.aquire(aPropID)->as<Property>();
    ASSERT_TRUE(aProp2.hasOwningAssociation());
    ASSERT_EQ(aProp2.getOwningAssociationRef(), association);
    ASSERT_TRUE(aProp2.hasAssociation());
    ASSERT_EQ(aProp2.getAssociationRef(), association);
    ASSERT_TRUE(aProp2.hasFeaturingClassifier());
    ASSERT_EQ(aProp2.getFeaturingClassifierRef(), association);
    ASSERT_TRUE(aProp2.hasNamespace());
    ASSERT_EQ(aProp2.getNamespaceRef(), association);
    ASSERT_TRUE(aProp2.getMemberNamespace().count(association.getID()));
    ASSERT_TRUE(aProp2.hasOwner());
    ASSERT_EQ(aProp2.getOwnerRef(), association);
}