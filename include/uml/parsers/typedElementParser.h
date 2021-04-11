#ifndef TYPED_ELEMENT_PARSER_H
#define TYPED_ELEMENT_PARSER_H

#include "yaml-cpp/yaml.h"
#include "uml/typedElement.h"

namespace UML {
    namespace Parsers {
        void parseTypedElement(YAML::Node node, TypedElement& el);
    }
}

#endif