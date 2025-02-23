#include "gtest/gtest.h"
#include "uml/types/namedElement.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

using namespace UML;

UML_SET_INTEGRATION_TEST(ClassifierGeneralization, Generalization, Class, getGeneralizations)
// UML_SET_INTEGRATION_TEST(ClassifierPowerTypeExtent, GeneralizationSet, Class, &Classifier::getPowerTypeExtent)
// UML_SINGLETON_INTEGRATION_TEST(ClassifierOwnedTemplateSignature, RedefinableTemplateSignature, OpaqueBehavior, &Classifier::getOwnedTemplateSignature, &Classifier::setOwnedTemplateSignature)
// UML_SINGLETON_INTEGRATION_TEST(RedefinableTemplateSignatureClassifier, PrimitiveType, RedefinableTemplateSignature, &RedefinableTemplateSignature::getClassifier, &RedefinableTemplateSignature::setClassifier)
// UML_SINGLETON_INTEGRATION_TEST(ClassifierTemplateParameter_, ClassifierTemplateParameter, Class, &Classifier::getTemplateParameter, &Classifier::setTemplateParameter)
// UML_SINGLETON_INTEGRATION_TEST(ClassifierTemplateParameterParameteredElement, OpaqueBehavior, ClassifierTemplateParameter, &ClassifierTemplateParameter::getParameteredElement, &ClassifierTemplateParameter::setParameteredElement)
// UML_SET_INTEGRATION_TEST(ClassifierTemplateParameterConstrainingClassifiers, Enumeration, ClassifierTemplateParameter, &ClassifierTemplateParameter::getConstrainingClassifiers)
// UML_SET_INTEGRATION_TEST(RedefinableTemplateSignatureExtendedSignature, RedefinableTemplateSignature, RedefinableTemplateSignature, &RedefinableTemplateSignature::getExtendedSignatures)

class ClassifierTest : public ::testing::Test {};

TEST_F(ClassifierTest, GetOwnedAttributesTest) {
  UmlManager m;
  auto& classifier1 = *m.create<Class>();
  auto& prop = *m.create<Property>();
  classifier1.getOwnedAttributes().add(prop);
  ASSERT_FALSE(classifier1.getOwnedAttributes().empty());
  ASSERT_EQ(classifier1.getOwnedAttributes().size(), 1);
  ASSERT_TRUE(classifier1.getOwnedAttributes().contains(prop));
  ASSERT_FALSE(classifier1.getAttributes().empty());
  ASSERT_EQ(*classifier1.getAttributes().front(), prop);
  ASSERT_TRUE(classifier1.getAttributes().contains(prop));
  ASSERT_EQ(classifier1.getFeatures().size(), 1);
  ASSERT_TRUE(classifier1.getFeatures().contains(prop));
  ASSERT_EQ(classifier1.getOwnedMembers().size(), 1);
  ASSERT_TRUE(classifier1.getOwnedMembers().contains(prop));
  ASSERT_EQ(classifier1.getMembers().size(), 1);
  ASSERT_TRUE(classifier1.getMembers().contains(prop));
  ASSERT_EQ(classifier1.getOwnedElements().size(), 1);
  ASSERT_TRUE(classifier1.getOwnedElements().contains(prop));
  ASSERT_TRUE(prop.getOwner());
  ASSERT_TRUE(prop.getNamespace());
  ASSERT_TRUE(prop.getFeaturingClassifier());
  ASSERT_TRUE(prop.getClass());
  ASSERT_EQ(*prop.getOwner(), classifier1);
  ASSERT_EQ(*prop.getNamespace(), classifier1);
  ASSERT_EQ(*prop.getFeaturingClassifier(), classifier1);
  ASSERT_EQ(*prop.getClass(), classifier1);
}

TEST_F(ClassifierTest, addAttributeFunctorTest) {
  UmlManager m;
  auto& c = *m.create<Class>();
  auto& p = *m.create<Property>();
  c.getOwnedAttributes().add(p);
  ASSERT_TRUE(c.getAttributes().size() == 1);
  ASSERT_TRUE(c.getAttributes().front() == &p);
  ASSERT_TRUE(c.getFeatures().size() == 1);
  ASSERT_TRUE(c.getFeatures().front() == &p);
  ASSERT_TRUE(p.getFeaturingClassifier() == &c);
  ASSERT_TRUE(c.getMembers().contains(p.getID()));
}

TEST_F(ClassifierTest, setClassifierTest) {
  UmlManager m;
  auto& p = *m.create<Property>();
  auto& c = *m.create<Class>();
  p.setClass(&c);
  ASSERT_TRUE(c.getAttributes().size() == 1);
  ASSERT_EQ(c.getAttributes().front().id(), p.getID());
  ASSERT_TRUE(p.getClass() == &c);
  ASSERT_TRUE(c.getFeatures().size() == 1);
  ASSERT_TRUE(c.getFeatures().front() == &p);
  ASSERT_TRUE(p.getFeaturingClassifier() == &c);
  ASSERT_TRUE(c.getMembers().contains(p.getID()));
}

TEST_F(ClassifierTest, removeAttributeFunctorTest) {
  UmlManager m;
  auto& p = *m.create<Property>();
  auto& c = *m.create<Class>();
  c.getOwnedAttributes().add(p);
  c.getOwnedAttributes().remove(p);
  ASSERT_EQ(c.getAttributes().size(), 0);
  ASSERT_EQ(c.getFeatures().size(), 0);
  ASSERT_EQ(c.getMembers().size(), 0);
  ASSERT_EQ(c.getOwnedElements().size(), 0);
  ASSERT_TRUE(!p.getFeaturingClassifier());
  ASSERT_TRUE(!p.getNamespace());
  ASSERT_TRUE(!p.getOwner());
}

TEST_F(ClassifierTest, inheritedMembersTest) {
  UmlManager m;
  auto& g1 = *m.create<Class>();
  auto& s1 = *m.create<Class>();
  auto& p1 = *m.create<Property>();
  p1.setVisibility(VisibilityKind::PUBLIC);
  g1.getOwnedAttributes().add(p1);
  auto& gen1 = *m.create<Generalization>();
  gen1.setGeneral(&g1);
  s1.getGeneralizations().add(gen1);
  ASSERT_EQ(s1.getInheritedMembers().size(), 1);
  ASSERT_EQ(*s1.getInheritedMembers().front(), p1);
  ASSERT_EQ(s1.getMembers().size(), 1);
  ASSERT_EQ(*s1.getMembers().front(), p1);
  ASSERT_EQ(g1.getInheritedMembers().size(), 0);
  ASSERT_NO_THROW(s1.getGeneralizations().remove(gen1));
  ASSERT_EQ(s1.getInheritedMembers().size(), 0);
  ASSERT_EQ(s1.getMembers().size(), 0);

  auto& g2 = *m.create<Class>();
  auto& s2 = *m.create<Class>();
  auto& p2 = *m.create<Property>();
  p2.setVisibility(VisibilityKind::PROTECTED);
  g2.getOwnedAttributes().add(p2);
  s2.getGenerals().add(g2);
  ASSERT_TRUE(s2.getInheritedMembers().size() == 1);
  ASSERT_TRUE(s2.getInheritedMembers().front() == &p2);
  ASSERT_TRUE(s2.getMembers().size() == 1);
  ASSERT_TRUE(s2.getMembers().front() == &p2);
  ASSERT_EQ(g2.getInheritedMembers().size(), 0);
  ASSERT_NO_THROW(s2.getGenerals().remove(g2));
  ASSERT_TRUE(s2.getInheritedMembers().size() == 0);
  ASSERT_TRUE(s2.getMembers().size() == 0);

  auto& g3 = *m.create<Class>();
  auto& s3 = *m.create<Class>();
  auto& p3 = *m.create<Property>();
  p3.setVisibility(VisibilityKind::PUBLIC);
  g3.getOwnedAttributes().add(p3);
  auto& gen3 = *m.create<Generalization>();
  s3.getGeneralizations().add(gen3);
  gen3.setGeneral(&g3);
  ASSERT_EQ(s3.getInheritedMembers().size(), 1);
  ASSERT_TRUE(s3.getInheritedMembers().front() == &p3);
  ASSERT_TRUE(s3.getMembers().size() == 1);
  ASSERT_TRUE(s3.getMembers().front() == &p3);
  ASSERT_NO_THROW(s3.getGeneralizations().remove(gen3));
  ASSERT_TRUE(s3.getInheritedMembers().size() == 0);
  ASSERT_TRUE(s3.getMembers().size() == 0);

  auto& g4 = *m.create<Class>();
  auto& s4 = *m.create<Class>();
  auto& p4 = *m.create<Property>();
  p4.setVisibility(VisibilityKind::PROTECTED);
  g4.getOwnedAttributes().add(p4);
  p4.setVisibility(VisibilityKind::PRIVATE);
  s4.getGenerals().add(g4);
  ASSERT_TRUE(s4.getInheritedMembers().size() == 0);

  auto& g5 = *m.create<Class>();
  auto& s5 = *m.create<Class>();
  auto& p5 = *m.create<Property>();
  p5.setVisibility(VisibilityKind::PUBLIC);
  g5.getOwnedAttributes().add(p5);
  s5.getGenerals().add(g5);
  p5.setVisibility(VisibilityKind::PRIVATE);
  ASSERT_EQ(s5.getInheritedMembers().size(), 0);
  ASSERT_EQ(s5.getMembers().size(), 0);

  auto& g6 = *m.create<Class>();
  auto& s6 = *m.create<Class>();
  auto& p6 = *m.create<Property>();
  p6.setVisibility(VisibilityKind::PROTECTED);
  s6.getGenerals().add(g6);
  g6.getOwnedAttributes().add(p6);
  ASSERT_EQ(s6.getInheritedMembers().size(), 1);
  ASSERT_EQ(s6.getMembers().size(), 1);
}

// TEST_F(ClassifierTest, redefinableTemplateSignatureAddAndRemoveParametersWhenExtendedTest) {
//   UmlManager m;
//   RedefinableTemplateSignaturePtr extendedSig = m.create<RedefinableTemplateSignature>();
//   RedefinableTemplateSignaturePtr sigExtending = m.create<RedefinableTemplateSignature>();
//   TemplateParameterPtr parameter = m.create<TemplateParameter>();
//   extendedSig->getParameters().add(*parameter);
//   sigExtending->getExtendedSignatures().add(*extendedSig);
//   ASSERT_EQ(sigExtending->getInheritedParameters().size(), 1);
//   ASSERT_EQ(sigExtending->getInheritedParameters().front(), parameter);
//   sigExtending->getExtendedSignatures().remove(*extendedSig);
//   ASSERT_EQ(sigExtending->getInheritedParameters().size(), 0);
//   RedefinableTemplateSignaturePtr otherSig = m.create<RedefinableTemplateSignature>();
//   otherSig->getParameters().add(*parameter);
//   sigExtending->getExtendedSignatures().add(*extendedSig);
//   sigExtending->getExtendedSignatures().add(*otherSig);
//   ASSERT_EQ(sigExtending->getInheritedParameters().size(), 1);
//   ASSERT_EQ(sigExtending->getInheritedParameters().front(), parameter);
//   sigExtending->getExtendedSignatures().remove(*extendedSig);
//   ASSERT_EQ(sigExtending->getInheritedParameters().size(), 1);
//   ASSERT_EQ(sigExtending->getInheritedParameters().front(), parameter);
//   sigExtending->getExtendedSignatures().remove(*otherSig);
//   ASSERT_EQ(sigExtending->getInheritedParameters().size(), 0);
//   extendedSig->getParameters().remove(*parameter);
//   sigExtending->getExtendedSignatures().add(*extendedSig);
//   ASSERT_EQ(sigExtending->getInheritedParameters().size(), 0);
//   extendedSig->getParameters().add(*parameter);
//   ASSERT_EQ(sigExtending->getInheritedParameters().size(), 1);
//   ASSERT_EQ(sigExtending->getInheritedParameters().front(), parameter);
//   extendedSig->getParameters().remove(*parameter);
//   ASSERT_EQ(sigExtending->getInheritedParameters().size(), 0);
//   sigExtending->getExtendedSignatures().add(*otherSig);
//   ASSERT_EQ(sigExtending->getInheritedParameters().size(), 1);
//   ASSERT_EQ(sigExtending->getInheritedParameters().front(), parameter);
//   extendedSig->getParameters().add(*parameter);
//   ASSERT_EQ(sigExtending->getInheritedParameters().size(), 1);
//   ASSERT_EQ(sigExtending->getInheritedParameters().front(), parameter);
//   extendedSig->getParameters().remove(*parameter);
//   ASSERT_EQ(sigExtending->getInheritedParameters().size(), 1);
//   ASSERT_EQ(sigExtending->getInheritedParameters().front(), parameter);
// }
