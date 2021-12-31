# uml-cpp

![example branch parameter](https://github.com/nemears/uml-cpp/actions/workflows/cmake.yml/badge.svg?branch=master)

Lightweight UML based MBSE application built on yaml syntax UML config files    
This is a work in progress but has basic functionality working.

All behavior of this api is based off of the uml-2.5 specification. If you have general questions about the api the PDF on the [OMG website](https://www.omg.org/spec/UML/2.5) is a good place to start and it is written by the creators of UML.

The goal behind the repository is to allow for a modular and lightweight aproach to set up a model programatically that is able to interface between multiple programs. Most modeling environments currently are bulky, locked behind paywalls and mostly used for drawing in the end. Although not as complete this is the start to opening access to modelling and meta programming to a wider audience.

## Building
uml-cpp is built with cmake  and has a couple of dependencies. 

### llvm

Before building you will need to have [lib-clang/llvm](https://llvm.org/) installed and findable by cmake, atleast if you want to build the c parsing ability of the api. To get llvm you must 

#### On Windows:

The only way to obtain llvm on windows currently is to [build from source](https://github.com/llvm/llvm-project/) or there may be a way to use [some other method](https://llvm.org/builds/)

#### On Debian
`apt-get install libclang-10-dev` should be all you need, but I needed an additional `apt-get install llvm` on Linux Mint 20

#### On Mac
Have not built yet on mac, should be possible.

### Running cmake

 When ready to generate build files run the following commands in a terminal in the repository's top level directory, make sure to replace `<generator type>` with your preffered method of compiling e.g: `Unix makefiles`, `Visual Studio 16`, `Ninja`, `Xcode` etc.
```
mkdir build
cd build
cmake -G <generator type> ..
cmake .. --build .
```
You can also choose to build from within your IDE or with whatever generator type make system you specified.
## Currently Implemented

### uml api
Uml class definitions can be used to reference a model with the api. To include the uml definitions into your code all of the relevant headers necessary are aquired through the `uml/uml-stable.h` header. This includes a class, UmlManager, supplied to control instantiation, loading and saving of uml objects from the config files with object pool semantics through the `create`, `aquire` and `release` methods.

There are two experimental classes in `uml/managers` called `UmlServer` and `UmlClient`. These two classes follow restful api semantics with `GET`, `PUT`, `POST`, and `DELETE` (the method is called `erase()`). `get()` can be used with element's id's or a "url" a.k.a the NamedElement's qualifiedNamespace. There is a build target called uml-server that will produce an executable `uml-server`, this executable can be run to start the server talking on port `8652`. **WARNING: Only works for POSIX**

Most of Uml implemented is mapped to config files, These can be stored persistently on disc or used to exchange model information between applications. The `UmlManager` class and all subclasses have the ability to `open` a path, or `save` to a path where model information has been saved.

### cpp parsing
Currently just playing around with clang AST and general mapping. Eventual goal is to fully roundtrip the c group of languages with this api. This will allow for 1:1 changes to code from a program that uses the api. 

## TODO
There is a lot to do right now this is just the next couple items I plan on looking into:  
  * longterm:
    * cpp parsers    
      * [use clang](https://shaharmike.com/cpp/libclang/?fbclid=IwAR1Y9PBig4Hd6bxmNERySpGAk2V09iCThrWuZ3Vb31LFAMG33pa1_kGVQZo), [1](https://clang.llvm.org/get_started.html), [index_header](https://clang.llvm.org/doxygen/Index_8h_source.html#l01562), [cpp interface (not currently used)](https://clang.llvm.org/doxygen/Parse_2Parser_8h_source.html)    
    * VSCode extension to realtime graph code as user codes
    * PC App that allows diagramming and realtime code binding (longterm goal)
    * further definition of uml    
      * Need to finish activities, get into interactions, and others  
    * redesign sequences (again)... They can go down on memory by implementing subsets and redefines literally to get rid of duplicate references
    * integrate with emf for interchangability between the two api's (legacy use cases)
      * ideas on how to do this: Persistent storage conversion (emf xml -> uml-cpp yml), make own java api (java code that can interchange between objects and yml)
    