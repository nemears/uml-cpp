#include "uml/parsers/parameterParser.h"
#include "uml/parsers/typedElementParser.h"

namespace UML {
namespace Parsers {

void parseParameter(YAML::Node node, Parameter& el, ParserMetaData& data) {
    parseTypedElement(node, el, data);

    if (node["direction"]) {
        if (node["direction"].IsScalar()) {
            if (node["direction"].as<string>().compare("IN") == 0) {
                el.setDirection(ParameterDirectionKind::IN);
            } else if (node["direction"].as<string>().compare("INOUT") == 0) {
                el.setDirection(ParameterDirectionKind::INOUT);
            } else if (node["direction"].as<string>().compare("OUT") == 0) {
                el.setDirection(ParameterDirectionKind::OUT);
            } else if (node["direction"].as<string>().compare("RETURN") == 0) {
                el.setDirection(ParameterDirectionKind::RETURN);
            } else {
                // error
            }
        } else {
            // error
        }
    }
}

}
}