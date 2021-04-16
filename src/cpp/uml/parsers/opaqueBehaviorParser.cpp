#include "uml/parsers/opaqueBehaviorParser.h"
#include "uml/parsers/behaviorParser.h"
#include "uml/literalString.h"

namespace UML {
namespace Parsers {

void parseOpaqueBehavior(YAML::Node node, OpaqueBehavior& bhv, ParserMetaData& data) {
    parseBehavior(node, bhv, data);

    if (node["bodies"]) {
        if (node["bodies"].IsSequence()) {
            for (size_t i=0; i<node["parameters"].size(); i++) {
                if (node["bodies"][i].IsMap()) {
                    if (node["bodies"][i]["literalString"]) {
                        // TODO literal string parsers
                    } else {
                        // error
                    }
                } else if (node["bodies"].IsScalar()) {
                    string body = node["bodies"][i].as<string>();
                    if (isValidUUID4(body)) {
                        // TODO
                    } else {
                        // make literal string with value
                        LiteralString* bodyUML = new LiteralString;
                        bodyUML->setValue(body);
                        bhv.getBodies().add(*bodyUML);
                    }
                } else {
                    // error
                }
            }
        } else {
            // error
        }
    }
}

}
}