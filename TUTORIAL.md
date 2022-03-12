# Tutorial

Again before trying to use this api I strongly encourage the user to be familiar with the concepts of uml. This tutorial is just for the uml-cpp specifics of this api and not uml itself. I will point the reader to the uml2.5 [specification](https://www.omg.org/spec/UML/2.5), but there are many more resources online to start off with as well.

## UmlManager
Now to the uml-cpp specifics, to control Element creation deletion and persistence there are a family of classes inheriting from `UML::UmlManager`. To start off we will just describe the UmlManager class itself and delve into the others later. The UmlManager class itself can easily be instantiated itself with no values supplied to the constructor, this is the only way to instantiate a UmlManager object.
```
using namespace UML;

UmlManager managerVal;
UmlManager* managerPtr = new UmlManager;
```
To create an element a manager has a template function called `create` that returns an empty pointer to the element type specified as a template parameter. 
```
UmlManager manager;
Class& clazz = *manager.create<Class>();
```
Once an element is created but goes out of scope it can be retreived again through the `get` method. The get method uses the elements ID member to retrieve it.
```
UmlManager manager;
ID clazzID;
{
    Class& clazz = manager.create<Class>();
    clazzID = clazz.getID();
}
Element& sameClazz = manager.get(clazzID);
```
`get` is used a lot internally so you don't have to really use this method when working within the model.

When done wokring with all of the elements, a manager can save them persistently to one corresponding yml file. This requires a root element of the manager to be set as well as a path to be provided. The relevant methods are `setRoot`, `save` and `setPath`.
```
manager.setRoot(clazz);
manager.setPath("~/Desktop/example.yml");
manager.save();
```
Or
```
manager.setRoot(clazz);
manager.save("~/Desktop/example.yml);
```
If you want to access the elements saved prior you can do so with the `open`, `get` and `getRoot` methods.
```
UmlManager newManager;
manager.open("~/Desktop/example.yml");
Element* root = manager.getRoot();
```
If you want to free up memory from your program the manager can be mounted and then it can release and aquire elements as needed with the `mount`, `release` and `aquire` methods.
```
manager.mount("~/Desktop");
manager.release(clazz);
// clazz.getID(); // memory error
Class& aquiredClazz = *manager.aquire(clazzID)->as<Class>();
```

## Elements and their Sets
So to just dive right into how the data of uml-cpp is organized we are going to have to understand the `Set<T,U>` object a little bit. Everything in uml-cpp is a Set or a subtype of Set. To start off lets look at the most basic of uml elements, Element. Element has two major attributes to it besides its ID, its ownedElements and it's owner. It's ownedElements has a multiplicity of 0..*, meanwhile its owner has a multiplicity of 0..1. We've simplified the multiplicity and the other attribute properties of the specification into `Set`'s, `Singleton`'s and `OrderedSet`'s (as well as more to come). It is simple to assume that ownedElements are a set, meanwhile owner is a Singleton. When accessing attributes specified in the specification you will most likey be returned an object of this type or data relating to it.
```
Element& root = *manager.getRoot();
UmlPtr<Element> owner = root.getOwner();
Set<Element,Element>& ownedElements = root.getOwnedElements();
Element& ownedElement = ownedElements.front();
```
These containers have additional functionality other than just organizing the elements, they also enforce the needed parts of uml. For example below is how package and packageable elements enforce the opposite and subsets behavior in the specification (excuse the googletest macro).
```
Package& package = manager.create<Package>();
Class& packagedElement = manager.create<Class>();
package.getPackagedElements().add(packagedElement);
ASSERT_EQ(*packagedElement.getOwningPackage(), package);
ASSERT_EQ(package.getOwnedElements().front(), packagedElement);
```
Below I have a list of the relevant methods to access these data types
|method|description|
|------|-----------|
|**Set<T,U>**|
|`void Set<T,U>::add(T&)`| adds an element to the set|
|`void Set<T,U>::remove(T&)`| removes an element from the set|
|`T& Set<T,U>::front()`| gets the "front" of the set (remember it is a set so order is based on id weight)|
|`T& Set<T,U>::get(int)`| gets the i'th element in the set (again order is based on id weight)|
|`T& Set<T,U>::get(ID)`| gets the element with the given id in the set|
|`T& Set<T,U>::get(std::string)`| gets the element with the specified name (only safe with sets that subset members)|
|`T& Set<T,U>::back()`| gets the last element in the set (same caveat on order)|
|`SetIterator Set<T,U>::begin()`| returns an iterator to the beginning of the set|
|`SetIterator Set<T,U>::end()`| returns an iterator to the end of the set|
|`ID_Set Set<T,U>::ids()`| returns a read only "set" to all of the ids within the set. Has similar methods to set itself|
|**Element access to singleton**|
|`UmlPtr<T> U::get*specName*()`| retrieves a smart UmlPtr\<T\>|
|`void U::set*specName*(T*)`| sets the singleton value (can be null to unset)|
|`void U::set*specName*(T&)`| sets the singleton value to the reference|

All of the warnings about ordering for set can be discounted for OrderedSet.

Notice how the singleton acessors return types of UmlPtr, this is to improve safety of the api. The methods relevant to UmlPtr are.
|method|description|
|------|-----------|
|`T* UmlPtr<T>::ptr()`| access raw pointer|
|`ID UmlPtr<T>::id() const`| access ID of pointer|
|`bool UmlPtr<T>::has() const`| check whether a null pointer, also `operator bool()` can be used|
|`bool UmlPtr<T>::release()`| release the element the pointer is referencing|
|`bool UmlPtr<T>::aquire()`| aquire the element after it has been released|
|`bool UmlPtr<T>::loaded() const`| tell whether ptr is released or aquired|


## UmlClient and UmlServer
A more adaptable and modular way to use a UmlManager is to use the `UmlClient` object in addition to the `uml-server` executable. The Client communicates with the server through UNIX or windows ports to acheive a decoupled memory management with a larger model. The purpose is to take away the slow writing and reading to files from your processes and shift it over to the uml-server (which doesn't have to be on your system). It follows a basic restful api setup. Also feel free to use the UmlServer object within your code to specialize its strategy to release elements from memory. 

If you want to use the `uml-server` executable it has some commandline args listed below
|commandline arg | description |
|----------------|-------------|
|--port, -p | specify the port that the server will listen on, default 8652|
 --mount-path, -m | specify the path that the server will be run on, default the path ran from|
 |--location, -l | load from and save to the path specified|
 |--duration, -d | run for specified duration in ms|
 |--num-els, -n | max number of elements in memory before releasing|

 An example, let's say we ran the following command on our system
 ```
 ./uml-server -p 877 --mount-path=~/Desktop -l ~/Desktop/example.yml
 ```
 This would run the server on port 877, mounted on your desktop and writing to example.yml when told to save. To connect and use this server with a client we would run code with a UmlClient specified as so
 ```
 UmlClient client("127.0.0.1:877");
 client.create<Class>();
 ```
 if a port wasn't specified we could just do this
 ```
 UmlClient client;
 client.create<Class>();
 ```
 ## Caveat
 This repository is under development and early in development. It does not hold all of the uml api, and probably still has some bugs to flush out and features to add. If you want a feature specifically please make an issue or email me at nickmears2@gmail.com.