#ifndef TYPED_ELEMENT_PARSER_H
#define TYPED_ELEMENT_PARSER_H

#include "yaml-cpp/yaml.h"
#include "uml/typedElement.h"
#include "uml/parsers/parserMetaData.h"

namespace UML {
    namespace Parsers {

        class SetTypeFunctor : public AbstractPostProcessFunctor {
            public:
                SetTypeFunctor(Element* el) : AbstractPostProcessFunctor(el) {};
                void operator()(Element& el) const override;
        };

        void parseTypedElement(YAML::Node node, TypedElement& el, ParserMetaData& data);
    }
}

#endif