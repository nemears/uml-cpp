# yuml
Lightweight UML based MBSE application built on yaml syntax UML config files__
currently two directories hold two different code bases:__
    yuml-parsers: parse yaml config files for uml representation__
    uml: cpp uml api


## Currently Implemented

### yaml parsing
Currently Implemented__
    -Element__
        -id and child__
    -NamedElement__
        -name__
    -Classifier__
        -attributes list__
    -Property__
        -type (started default values)__
    -Type__
    -Class__
    -PrimitveType (not parsed)__
        -created enum__
    -ValueSpecification (not parsed)__
    -Namespace__
    -Model__
    -Literals__
    -Operations__
    -OpaqueBehaviors


## TODO
There is a lot to do right now this is just the next couple items I plan on looking into:__
    longterm:__
        Jenkins integration__
        front-end implementation (look into options, ask for opinions)__
            -possibilities : Qt, react js__
        cpp parsers__
            -[use clang](https://shaharmike.com/cpp/libclang/?fbclid=IwAR1Y9PBig4Hd6bxmNERySpGAk2V09iCThrWuZ3Vb31LFAMG33pa1_kGVQZo)[1](https://clang.llvm.org/get_started.html)__
        further definition of uml__
    short-term:__
        test test test__
        emit to yml__
    