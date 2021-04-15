#include "uml/parsers/behaviorParser.h"
#include "uml/parsers/classParser.h"

namespace UML {
namespace Parsers {

void parseBehavior(YAML::Node node, Behavior& bhv, ParserMetaData& data) {
    parseClass(node, bhv, data);

    if (node["parameters"]) {
        if (node["parameters"].IsSequence()) {
            for (size_t i=0; i<node["parameters"].size(); i++) {
                if (node["parameters"][i].IsMap()) {
                    // TODO parameter parser
                } else {
                    // error? or is scalar allowed parameters shared(?)
                }
            }
        } else {
            // error
        }
    }
}

}
}