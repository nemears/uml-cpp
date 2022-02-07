#include "gtest/gtest.h"
#include "uml/uml-stable.h"

using namespace UML;

class SignalTest : public ::testing::Test {};

TEST_F(SignalTest, basicSignalAndReceptionTest) {
    UmlManager m;
    Class& clazz = m.create<Class>();
    Signal& signal = m.create<Signal>();
    Reception& reception = m.create<Reception>();
    Property& attr = m.create<Property>();
    signal.getOwnedAttributes().add(attr);
    reception.setSignal(signal);
    clazz.getOwnedReceptions().add(reception);

    ASSERT_EQ(signal.getOwnedAttributes().size(), 1);
    ASSERT_EQ(signal.getAttributes().size(), 1);
    ASSERT_EQ(signal.getFeatures().size(), 1);
    ASSERT_EQ(signal.getOwnedMembers().size(), 1);
    ASSERT_EQ(signal.getMembers().size(), 1);
    ASSERT_EQ(signal.getOwnedElements().size(), 1);

    ASSERT_EQ(signal.getOwnedAttributes().front(), attr);
    ASSERT_EQ(signal.getAttributes().front(), attr);
    ASSERT_EQ(signal.getFeatures().front(), attr);
    ASSERT_EQ(signal.getOwnedMembers().front(), attr);
    ASSERT_EQ(signal.getMembers().front(), attr);
    ASSERT_EQ(signal.getOwnedElements().front(), attr);

    ASSERT_TRUE(attr.hasFeaturingClassifier());
    ASSERT_TRUE(attr.hasNamespace());
    ASSERT_TRUE(attr.hasOwner());
    ASSERT_EQ(attr.getFeaturingClassifierRef(), signal);
    ASSERT_EQ(attr.getNamespaceRef(), signal);
    ASSERT_EQ(attr.getOwnerRef(), signal);

    ASSERT_TRUE(reception.hasSignal());
    ASSERT_EQ(reception.getSignalRef(), signal);

    ASSERT_EQ(clazz.getOwnedReceptions().size(), 1);
    ASSERT_EQ(clazz.getFeatures().size(), 1);
    ASSERT_EQ(clazz.getOwnedMembers().size(), 1);
    ASSERT_EQ(clazz.getMembers().size(), 1);
    ASSERT_EQ(clazz.getOwnedElements().size(), 1);

    ASSERT_TRUE(reception.hasFeaturingClassifier());
    ASSERT_TRUE(reception.hasNamespace());
    ASSERT_TRUE(reception.hasOwner());

    ASSERT_EQ(reception.getFeaturingClassifierRef(), clazz);
    ASSERT_EQ(reception.getNamespaceRef(), clazz);
    ASSERT_EQ(reception.getOwnerRef(), clazz);
}