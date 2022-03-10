#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/uml-stable.h"

using namespace std;
using namespace UML;

class SignalParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(SignalParserTest, parseSignalTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "signalTests/basicSignal.yml").ptr());
    ASSERT_EQ(el->getElementType(), ElementType::PACKAGE);
    Package& pckg = el->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 2);
    Signal& signal = pckg.getPackagedElements().get("signal").as<Signal>();
    Class& clazz = pckg.getPackagedElements().get("class").as<Class>();
    ASSERT_EQ(signal.getOwnedAttributes().size(), 1);
    ASSERT_EQ(clazz.getOwnedReceptions().size(), 1);
    Reception& reception = clazz.getOwnedReceptions().front();
    ASSERT_EQ(*reception.getSignal(), signal);
}

TEST_F(SignalParserTest, emitSignalTest) {
    UmlManager m;
    Package& pckg = *m.create<Package>();
    Signal& signal = *m.create<Signal>();
    Property& attr = *m.create<Property>();
    Class& clazz = *m.create<Class>();
    Reception& reception = *m.create<Reception>();
    pckg.setID("IJabcFrKrE9yxVT&qQUQ2&xzVxpd");
    signal.setID("_sgqzW88lsR9bBTk8GyBRjYujfB5");
    attr.setID("DSV8nQG_4VargpMXqb57S2dACThU");
    clazz.setID("HdQGnHEztfzbMvcBURUAEPRWuw7M");
    reception.setID("Y2ANJRtpZRZNCwR7jFo2v_DVm8pZ");
    pckg.getPackagedElements().add(signal, clazz);
    signal.getOwnedAttributes().add(attr);
    clazz.getOwnedReceptions().add(reception);
    reception.setSignal(signal);
    string expectedEmit = R""""(package:
  id: IJabcFrKrE9yxVT&qQUQ2&xzVxpd
  packagedElements:
    - signal:
        id: _sgqzW88lsR9bBTk8GyBRjYujfB5
        ownedAttributes:
          - property:
              id: DSV8nQG_4VargpMXqb57S2dACThU
    - class:
        id: HdQGnHEztfzbMvcBURUAEPRWuw7M
        ownedReceptions:
          - reception:
              id: Y2ANJRtpZRZNCwR7jFo2v_DVm8pZ
              signal: _sgqzW88lsR9bBTk8GyBRjYujfB5)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(pckg));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}