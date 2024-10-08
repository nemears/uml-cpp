# uml-cpp

![example branch parameter](https://github.com/nemears/uml-cpp/actions/workflows/cmake.yml/badge.svg?branch=master)

Lightweight UML based MBSE api built on yaml/json syntax UML config files    
This is a work in progress but has basic functionality working. Serves as part of backend for https://uml.cafe

All behavior of this api is based off of the uml-2.5 specification. If you have general questions about the api the PDF on the [OMG website](https://www.omg.org/spec/UML/2.5) is a good place to start and it is written by the creators of UML.

The goal behind the repository is to allow for a modular and lightweight aproach to set up a model programatically that is able to interface between multiple programs. Most modeling environments currently are bulky, locked behind paywalls and mostly used for drawing in the end. Although not as complete this is the start to opening access to modelling and meta programming to a wider audience.

## Currently Implemented

### uml api
Uml class definitions can be used to reference a model with the api. To include the uml definitions into your code all of the relevant headers necessary are aquired through the `uml/uml-stable.h` header. This includes a class, `Manager<>`, supplied to control instantiation, loading and saving of uml objects from the config files with object pool semantics through the `create`, `aquire` and `release` methods.

Most of Uml implemented is mapped to config files, These can be stored persistently on disc or used to exchange model information between applications. The `Manager<>` class and all subclasses have the ability to `open` a path, or `save` to a path where model information has been saved.

### client server managers
There are two classes in `uml/managers/protocol` called `UmlServer` and `UmlClient` to offer a quicker way to aquire and release elements. These two classes follow restful api semantics with `GET`, `PUT`, `POST`, and `DELETE` (the method is called `erase()`). `get()` can be used with element's id's or a "url" a.k.a the NamedElement's qualifiedNamespace. There is a build target called uml-server that will produce an executable `uml-server`, this executable can be run to start the server talking on port `8652`. If you don't want to build the experimental managers set the cmake option `UML_CLIENT_SERVER` to NO.

### Tutorial
A basic tutorial on how to use the uml-cpp api can be found here at the [TUTORIAL.md](TUTORIAL.md) Needs updating
    
### future development
This project serves as the main backend for the uml-cafe service hosted at https://uml.cafe. The development will move and progress with the needs of that, as well as any other related projects that may also be developed using this technology. Known features that wan´t to be implemented include:
- Serialization abstraction based on profile
- MetaClasses and api generation
- Finishing the complete spec of UML 2.5.2  

If you are interested in these features or have specific features that you want implemented contact Em (em@uml.cafe) for inquiries.

## Contributing
Please feel free to contribute to uml-cpp, just take a look at the [CONTRIBUTING.md](CONTRIBUTING.md) before you make a pull request.

## Questions
email me at em@uml.cafe or make an issue on our issue board here in github!