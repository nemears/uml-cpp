#ifndef UML_MACROS_H
#define UML_MACROS_H

//  deines header definitions for a singleton within a uml element
#define DEFINE_SINGLETON(methodName, singletonName, singletonType, myType) \
protected: \
Singleton<singletonType, myType> singletonName = Singleton<singletonType, myType>(this); \
Set<singletonType, myType>& get ## methodName ## Singleton(); \
public: \
singletonType ## Ptr get ## methodName() const; \
void set ## methodName(singletonType* val); \
void set ## methodName(singletonType& val); \
void set ## methodName(singletonType ## Ptr val); \
void set ## methodName(ID id); \
protected:

// implements all of the singleton acessors and mutators for a uml element
#define IMPLEMENT_SINGLETON(methodName, singletonName, singletonType, myType) \
Set<singletonType, myType>& myType::get ## methodName ## Singleton() { \
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

#endif