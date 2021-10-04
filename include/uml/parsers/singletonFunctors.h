#ifndef UML_PARSERS_SINGLETON_FUNCTORS_H
#define UML_PARSERS_SINGLETON_FUNCTORS_H
#include "parser.h"

namespace UML {
    namespace Parsers {

        class SetDefaultValue : public parseAndSetSingletonFunctor<ValueSpecification, Property> {
            public:
                SetDefaultValue();
        };

        class SetSpecific : public parseAndSetSingletonFunctor<Classifier, Generalization> {
            public:
                SetSpecific();
        };

        class SetNestingClass : public parseAndSetSingletonFunctor<Class, Classifier> {
            public:
                SetNestingClass();
        };

        class OperationSetClass : public parseAndSetSingletonFunctor<Class, Operation> {
            public:
                OperationSetClass();
        };

        class PropertySetClass : public parseAndSetSingletonFunctor<Class, Property> {
            public:
                PropertySetClass();
        };

    }
}

#endif