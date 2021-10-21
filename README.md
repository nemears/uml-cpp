# uml-cpp
Lightweight UML based MBSE application built on yaml syntax UML config files    
This is a work in progress but has basic functionality working.

All behavior of this api is based off of the uml-2.5 specification. If you have general questions about the api the PDF on the [OMG website](https://www.omg.org/spec/UML/2.5) is a good place to start and it is written by the creators of UML.

## Building
uml-cpp is built with cmake  and has a couple of dependencies. 

### llvm

Before building you will need to have [lib-clang/llvm](https://llvm.org/) installed and findable by cmake, to do this you must 

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
The code currently has an application, but the application will parse a config
file and print it out. Besides that the source can be used as c++ uml library

### uml api
Uml class definitions can be used to reference a model with the api. There is 
also a class, UmlManager, supplied to control loading and saving from the config files
with object pool semantics.

### config files
Most of Uml implemented is mapped to config files, These can be stored persistently on disc or used to exchange model information between applications.

### cpp parsing
Currently just playing around with clang AST and general mapping, want to round out 
UmlManager with emits so that parsing is more practical to user

## TODO
There is a lot to do right now this is just the next couple items I plan on looking into:  
  * longterm:
    * cpp parsers    
      * [use clang](https://shaharmike.com/cpp/libclang/?fbclid=IwAR1Y9PBig4Hd6bxmNERySpGAk2V09iCThrWuZ3Vb31LFAMG33pa1_kGVQZo), [1](https://clang.llvm.org/get_started.html), [index_header](https://clang.llvm.org/doxygen/Index_8h_source.html#l01562)    
    * VSCode extension to realtime graph code as user codes
    * PC App that allows diagramming and realtime code binding (longterm goal)
    * further definition of uml    
      * Need to finish activities, get into interactions, and others  
    * Restful uml-server that can handle requests for data from model (can be used for frontend)
    