#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/literalUnlimitedNatural.h"
#include "uml/literalNull.h"
#include "uml/package.h"

using namespace std;
using namespace UML;

class LiteralsParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(LiteralsParserTest, testParsingSomeLiterals) {
    UmlManager m;
    Element* el;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "literalsTests/someLiterals.yml"));
    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package* pckg = dynamic_cast<Package*>(el);
    ASSERT_EQ(pckg->getPackagedElements().size(), 3);
    ASSERT_EQ(pckg->getPackagedElements().front()->getElementType(), ElementType::LITERAL_UNLIMITED_NATURAL);
    LiteralUnlimitedNatural* n1 = dynamic_cast<LiteralUnlimitedNatural*>(pckg->getPackagedElements().front());
    ASSERT_EQ(n1->getName(), "infinity");
    ASSERT_EQ(n1->isInfinite(), true);
    ASSERT_EQ(pckg->getPackagedElements().get(1)->getElementType(), ElementType::LITERAL_UNLIMITED_NATURAL);
    LiteralUnlimitedNatural* n2 = dynamic_cast<LiteralUnlimitedNatural*>(pckg->getPackagedElements().get(1));
    ASSERT_EQ(n2->getName(), "number");
    ASSERT_EQ(n2->isInfinite(), false);
    ASSERT_EQ(n2->getNumberValue(), 9);
    ASSERT_EQ(pckg->getPackagedElements().back()->getElementType(), ElementType::LITERAL_NULL);
    LiteralNull* n3 = dynamic_cast<LiteralNull*>(pckg->getPackagedElements().back());
    ASSERT_EQ(n3->getName(), "nullLiteral");
}

TEST_F(LiteralsParserTest, testEmitLiteralUnlimitedNatural) {
    UmlManager m;
    Package& p = m.create<Package>();
    LiteralUnlimitedNatural& l1 = m.create<LiteralUnlimitedNatural>();
    LiteralUnlimitedNatural& l2 = m.create<LiteralUnlimitedNatural>();
    LiteralUnlimitedNatural& l3 = m.create<LiteralUnlimitedNatural>();
    LiteralNull& ln = m.create<LiteralNull>();
    p.setID("e_ob7tgbN16Plhj_sTAOVD5ijLrL");
    l1.setID("7bYUY3yFUBrfPmzKKrV2NJmXuECA");
    l2.setID("puJaUTZsLPdGJkJSJtdX51MIA2ch");
    l3.setID("8&K_0aLhvQDM12ZeYg9nPiSrexHo");
    ln.setID("4gA4RgL9vKTRYd61D99y1d_Yggj6");
    l2.setInfinite();
    l3.setNumberValue(8889728897389425);
    p.getPackagedElements().add(l1);
    p.getPackagedElements().add(l2);
    p.getPackagedElements().add(l3);
    p.getPackagedElements().add(ln);

    string expectedEmit = R""""(package:
  id: e_ob7tgbN16Plhj_sTAOVD5ijLrL
  packagedElements:
    - literalUnlimitedNatural:
        id: 7bYUY3yFUBrfPmzKKrV2NJmXuECA
        value: 0
    - literalUnlimitedNatural:
        id: puJaUTZsLPdGJkJSJtdX51MIA2ch
        value: "*"
    - literalUnlimitedNatural:
        id: 8&K_0aLhvQDM12ZeYg9nPiSrexHo
        value: 8889728897389425
    - literalNull:
        id: 4gA4RgL9vKTRYd61D99y1d_Yggj6)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(p));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}