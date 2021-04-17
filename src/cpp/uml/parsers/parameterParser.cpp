#include "uml/parsers/parameterParser.h"
#include "uml/parsers/typedElementParser.h"

namespace UML {
namespace Parsers {

void parseParameter(YAML::Node node, Parameter& el, ParserMetaData& data) {
    parseTypedElement(node, el, data);

    if (node["direction"]) {
        // TODO
    }
}

}
}