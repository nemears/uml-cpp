#include "uml/parsers/operationParser.h"
#include "uml/parsers/namedElementParser.h"
#include "uml/parsers/opaqueBehaviorParser.h"

namespace UML {
namespace Parsers {

void parseOperation(YAML::Node node, Operation& op, ParserMetaData& data) {
    parseNamedElement(node, op, data);

    if (node["methods"]) {
        if (node["methods"].IsSequence()) {
            for (size_t i=0; i<node["methods"].size(); i++) {
                if (node["methods"][i]["opaqueBehavior"]) {
                    OpaqueBehavior* bhv = new OpaqueBehavior;
                    parseOpaqueBehavior(node["methods"][i]["opaqueBehavior"], *bhv, data);
                    op.getMethods().add(*bhv);
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