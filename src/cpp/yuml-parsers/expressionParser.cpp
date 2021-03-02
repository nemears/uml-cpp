#include "yuml-parsers/expressionParser.h"

bool ExpressionParser::parseFeatures(YAML::Node node, Element* el) {
    bool ret = NamedElementParser::parseFeatures(node, el);

    if (!parseTypeFeatures(node, el)) {
        ret = false;
    }

    if (node["operand"]) {
        if (node["operand"].IsScalar()) {
            // If it is a scalar that means it is a uuid or a value as a string
            if (isValidUUID4(node["operand"].as<string>())) {
                boost::uuids::uuid operandID = boost::lexical_cast<boost::uuids::uuid>(node["operand"].as<string>());

                parseNowOrLater(operandID, el->uuid, node, &ExpressionParser::addOperandLater);
            }
        } else if (node["operand"].IsSequence()) {

        } else if (node["operand"].IsMap()) {

        }
    }

    return ret;
}

bool ExpressionParser::emit(YAML::Emitter& emitter, Element* el) {
    return true;
}

Element* ExpressionParser::createElement(){
    theEl = new Expression;
    return theEl;
}