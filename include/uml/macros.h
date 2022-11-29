#ifndef UML_MACROS_H
#define UML_MACROS_H

//  deines header definitions for a singleton within a uml element
#define DEFINE_SINGLETON_W_POLICIES(methodName, singletonName, singletonType, myType, addPolicy, removePolicy) \
protected: \
CustomSingleton<singletonType, myType, addPolicy, removePolicy> singletonName = CustomSingleton<singletonType, myType, addPolicy, removePolicy>(this); \
TypedSet<singletonType, myType>& get ## methodName ## Singleton(); \
public: \
singletonType ## Ptr get ## methodName() const; \
void set ## methodName(singletonType* val); \
void set ## methodName(singletonType& val); \
void set ## methodName(singletonType ## Ptr val); \
void set ## methodName(ID id); \
protected:

// implements all of the singleton acessors and mutators for a uml element
#define IMPLEMENT_SINGLETON(methodName, singletonName, singletonType, myType) \
TypedSet<singletonType, myType>& myType::get ## methodName ## Singleton() { \
    return singletonName; \
} \
singletonType ## Ptr myType::get ## methodName() const { \
    return singletonName.get(); \
} \
void myType::set ## methodName(singletonType* val) { \
    singletonName.set(val); \
} \
void myType::set ## methodName(singletonType& val) { \
    singletonName.set(val); \
} \
void myType::set ## methodName(singletonType ## Ptr val) { \
    singletonName.set(val); \
} \
void myType::set ## methodName(ID id) { \
    singletonName.set(id); \
} \

#define DEFINE_SINGLETON(methodName, singletonName, singletonType, myType) \
typedef DoNothingAdd<singletonType, myType> _ ## singletonType ## _ ## myType ## DoNothingAddPolicy; \
typedef DoNothingRemove<singletonType, myType> _ ## singletonType ## _ ## myType ## DoNothingRemovePolicy; \
DEFINE_SINGLETON_W_POLICIES(methodName, singletonName, singletonType, myType, _ ## singletonType ## _ ## myType ## DoNothingAddPolicy, _ ## singletonType ## _ ## myType ## DoNothingRemovePolicy) \

#define FRIEND_ALL_UML() \
friend class Action; \
friend class ActivityEdge; \
friend class ActivityGroup; \
friend class ActivityNode; \
friend class ActivityParameterNode; \
friend class ActivityPartition; \
friend class Association; \
friend class Behavior; \
friend class BehavioredClassifier; \
friend class BehavioralFeature; \
friend class CallBehaviorAction; \
friend class ClassifierTemplateParameter; \
friend class Classifier; \
friend class Comment; \
friend class ConnectableElement; \
friend class Connector; \
friend class ConnectorEnd; \
friend class Constraint; \
friend class DecisionNode; \
friend class Dependency; \
friend class DirectedRelationship; \
friend class Element; \
friend class ElementImport; \
friend class EncapsulatedClassifier; \
friend class ExceptionHandler; \
friend class Feature; \
friend class Generalization; \
friend class GeneralizationSet; \
friend class InstanceSpecification; \
friend class InstanceValue; \
friend class InterfaceRealization; \
friend class InterruptibleActivityRegion; \
friend class InvocationAction; \
friend class MultiplicityElement; \
friend class NamedElement; \
friend class Namespace; \
friend class ObjectFlow; \
friend class ObjectNode; \
friend class PackageImport; \
friend class PackageMerge; \
friend class ParameterableElement; \
friend class Parameter; \
friend class ParameterSet; \
friend class Port; \
friend class ProfileApplication; \
friend class Property; \
friend class Reception; \
friend class RedefinableElement; \
friend class RedefinableTemplateSignature; \
friend class Relationship; \
friend class Slot; \
friend class StructuredClassifier; \
friend class TypedElement; \
friend class TemplateBinding;  \
friend class TemplateParameter; \
friend class TemplateParameterSubstitution; \
friend class TemplateSignature; \
friend class Usage; \


#endif