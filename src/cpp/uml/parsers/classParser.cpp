#include "uml/parsers/classParser.h"
#include "uml/parsers/classifierParser.h"
#include "uml/parsers/operationParser.h"

namespace UML {
namespace Parsers {

void parseClass(YAML::Node node, Class& clazz, ParserMetaData& data) {
    parseClassifier(node, clazz, data);

    if (node["operations"]) {
        if (node["operations"].IsSequence()) {
            for (size_t i=0; i<node["operations"].size(); i++) {
                if (node["operations"][i]["operation"]) {
                    Operation * op = new Operation;
                    parseOperation(node["operations"][i]["operation"], *op, data);
                    clazz.getOperations().add(*op);
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