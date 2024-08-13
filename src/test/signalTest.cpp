#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"
#include "test/uml-cpp-paths.h"

using namespace UML;

UML_SET_INTEGRATION_TEST(SignalOwnedAttributes, Property, Signal, &Signal::getOwnedAttributes)
UML_SINGLETON_INTEGRATION_TEST(ReceptionSignal, Signal, Reception, &Reception::getSignal, &Reception::setSignal)

class SignalTest : public ::testing::Test {
    public:
        std::string ymlPath = YML_FILES_PATH;
};

TEST_F(SignalTest, basicSignalAndReceptionTest) {
    Manager<> m;
    Class& clazz = *m.create<Class>();
    Signal& signal = *m.create<Signal>();
    Reception& reception = *m.create<Reception>();
    Property& attr = *m.create<Property>();
    signal.getOwnedAttributes().add(attr);
    reception.setSignal(signal);
    clazz.getOwnedReceptions().add(reception);

    ASSERT_EQ(signal.getOwnedAttributes().size(), 1);
    ASSERT_EQ(signal.getAttributes().size(), 1);
    ASSERT_EQ(signal.getFeatures().size(), 1);
    ASSERT_EQ(signal.getOwnedMembers().size(), 1);
    ASSERT_EQ(signal.getMembers().size(), 1);
    ASSERT_EQ(signal.getOwnedElements().size(), 1);

    ASSERT_EQ(signal.getOwnedAttributes().front(), &attr);
    ASSERT_EQ(signal.getAttributes().front(), &attr);
    ASSERT_EQ(signal.getFeatures().front(), &attr);
    ASSERT_EQ(signal.getOwnedMembers().front(), &attr);
    ASSERT_EQ(signal.getMembers().front(), &attr);
    ASSERT_EQ(signal.getOwnedElements().front(), &attr);

    ASSERT_TRUE(attr.getFeaturingClassifier());
    ASSERT_TRUE(attr.getNamespace());
    ASSERT_TRUE(attr.getOwner());
    ASSERT_EQ(*attr.getFeaturingClassifier(), signal);
    ASSERT_EQ(*attr.getNamespace(), signal);
    ASSERT_EQ(*attr.getOwner(), signal);

    ASSERT_TRUE(reception.getSignal());
    ASSERT_EQ(*reception.getSignal(), signal);

    ASSERT_EQ(clazz.getOwnedReceptions().size(), 1);
    ASSERT_EQ(clazz.getFeatures().size(), 1);
    ASSERT_EQ(clazz.getOwnedMembers().size(), 1);
    ASSERT_EQ(clazz.getMembers().size(), 1);
    ASSERT_EQ(clazz.getOwnedElements().size(), 1);

    ASSERT_TRUE(reception.getFeaturingClassifier());
    ASSERT_TRUE(reception.getNamespace());
    ASSERT_TRUE(reception.getOwner());

    ASSERT_EQ(*reception.getFeaturingClassifier(), clazz);
    ASSERT_EQ(*reception.getNamespace(), clazz);
    ASSERT_EQ(*reception.getOwner(), clazz);
}

TEST_F(SignalTest, parseSignalTest) {
    Manager<> m;
    ASSERT_NO_THROW(m.open(ymlPath + "signalTests/basicSignal.yml"));
    ASSERT_EQ(m.getRoot()->getElementType(), ElementType::PACKAGE);
    Package& pckg = m.getRoot()->as<Package>();
    ASSERT_EQ(pckg.getPackagedElements().size(), 2);
    Signal& signal = pckg.getPackagedElements().get("signal")->as<Signal>();
    Class& clazz = pckg.getPackagedElements().get("class")->as<Class>();
    ASSERT_EQ(signal.getOwnedAttributes().size(), 1);
    ASSERT_EQ(clazz.getOwnedReceptions().size(), 1);
    Reception& reception = *clazz.getOwnedReceptions().front();
    ASSERT_EQ(*reception.getSignal(), signal);
}

TEST_F(SignalTest, emitSignalTest) {
    Manager<> m;
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
    std::string expectedEmit = R""""(Package:
  id: "IJabcFrKrE9yxVT&qQUQ2&xzVxpd"
  packagedElements:
    - Class:
        id: HdQGnHEztfzbMvcBURUAEPRWuw7M
        ownedReceptions:
          - Reception:
              id: Y2ANJRtpZRZNCwR7jFo2v_DVm8pZ
              signal: _sgqzW88lsR9bBTk8GyBRjYujfB5
    - Signal:
        id: _sgqzW88lsR9bBTk8GyBRjYujfB5
        ownedAttributes:
          - Property:
              id: DSV8nQG_4VargpMXqb57S2dACThU)"""";
    std::string generatedEmit;
    EmitterData data;
    data.mode = SerializationMode::WHOLE;
    ASSERT_NO_THROW(generatedEmit = emit(pckg, data));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}
