# uml-cpp
Lightweight UML based MBSE application built on yaml syntax UML config files    
This is a work in progress but has basic functionality working

## Currently Implemented
The code currently has an application, but the application will parse a config
file and print it out. Besides that the source can be used as c++ uml library

### uml api
Uml class definitions can be used to reference a model with the api. There is 
also a class, UmlManager, supplied to control loading and saving from the config files
with object pool semantics.

### config files
All of Uml implemented except for activites is mapped to config files, working on
having more dynamic emits to break up files

### cpp parsing
Currently just playing around with clang AST and general mapping, want to round out 
UmlManager with emits so that parsing is more practical to user

### python parsing
Ditching for now, not sure if PyBind11 is way to go, will hold off until PyBind11 is
more mature and cpp parsing is mostly complete

## TODO
There is a lot to do right now this is just the next couple items I plan on looking into:  
  * longterm:      
    * front-end implementation (look into options, ask for opinions)    
      * possibilities : Qt, react js, python libs(short term)    
    * cpp parsers    
      * [use clang](https://shaharmike.com/cpp/libclang/?fbclid=IwAR1Y9PBig4Hd6bxmNERySpGAk2V09iCThrWuZ3Vb31LFAMG33pa1_kGVQZo), [1](https://clang.llvm.org/get_started.html), [index_header](https://clang.llvm.org/doxygen/Index_8h_source.html#l01562)    
    * python parsers  
      * [use pybind11](https://realpython.com/python-bindings-overview/) to port our uml code to python and [use ast](https://docs.python.org/3/library/ast.html#module-ast)  to parse python with it  
      * this will help in making a [front end](https://wiki.python.org/moin/WebFrameworks) quickly with some ptyhon library for MVP demo    
    * VSCode extension to realtime graph code as user codes (longterm goal)   
    * PC App that allows diagramming and realtime code binding (longterm goal)
    * further definition of uml    
      * Need to finish activities, get into interactions and artifacts    
    * runtime database implementation for UmlManager to query and further acheive object pool design
  * short-term:    
    * test test test    
    