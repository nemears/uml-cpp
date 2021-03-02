#include "yuml-parsers/expressionParser.h"

bool ExpressionParser::parseFeatures(YAML::Node node, Element* el) {
    bool ret = NamedElementParser::parseFeatures(node, el);

    if (!parseTypeFeatures(node, el)) {
        ret = false;
    }

    if (node["operand"]) {
        if (node["operand"].IsScalar()) {
            // If it is a scalar that means it is a uuid or a value as a string
            if (!parseOperand(node["operand"], el)) {
                ret = false;
            }
        } else if (node["operand"].IsSequence()) {
            for (std::size_t i = 0; i < node["operand"].size(); i++) {
                if (!parseOperand(node["operand"][i], el)) {
                    ret = false;
                }
            }
        }
    }

    return ret;
}

bool ExpressionParser::parseOperand(YAML::Node node, Element* expression) {
    if (isValidUUID4(node.as<string>())) {
        // if uuid it is an instance value
        boost::uuids::uuid operandID = boost::lexical_cast<boost::uuids::uuid>(node["operand"].as<string>());

        parseNowOrLater(operandID, expression->uuid, node, &ExpressionParser::addInstanceOperandLater);
    } else {
        if (node["expression"]) {
            ExpressionParser op = ExpressionParser(elements, postProcessFlag);
            Element* parsedEl = op.parseElement(node["expression"]);
            dynamic_cast<Expression*>(expression)->operands.push_back(dynamic_cast<ValueSpecification*>(parsedEl));
        } else if (node["instanceValue"]) {
            if (node["instanceValue"].IsScalar()) {
                if (isValidUUID4(node["instanceValue"].as<string>())) {
                    boost::uuids::uuid instanceID = boost::lexical_cast<boost::uuids::uuid>(node["instanceValue"].as<string>());

                    parseNowOrLater(instanceID, expression->uuid, node, &ExpressionParser::addInstanceOperandLater);
                }
            }
        } else if (node["literalBool"]) {
            if (node["literalBool"].IsScalar()) {
                LiteralBool* lb;
                lb->setValue(node["literalBool"].as<bool>());
                dynamic_cast<Expression*>(expression)->operands.push_back(dynamic_cast<ValueSpecification*>(lb));
            }
        } else if (node["literalInt"]) {
            if (node["literalInt"].IsScalar()) {
                LiteralInt* li;
                li->setValue(node["literalInt"].as<int>());
                dynamic_cast<Expression*>(expression)->operands.push_back(dynamic_cast<ValueSpecification*>(li));
            }
        } else if (node["literalReal"]) {
            if (node["literalReal"].IsScalar()) {
                LiteralReal* lr;
                lr->setValue(node["literalReal"].as<double>());
                dynamic_cast<Expression*>(expression)->operands.push_back(dynamic_cast<ValueSpecification*>(lr));
            }
        } else if (node["literalString"]) {
            if (node["literalString"].IsScalar()) {
                LiteralString* ls;
                ls->setValue(node["literalString"].as<string>());
                dynamic_cast<Expression*>(expression)->operands.push_back(dynamic_cast<ValueSpecification*>(ls));
            }
        } else {
            // Error
            return false;
        }
    }
}

bool ExpressionParser::emit(YAML::Emitter& emitter, Element* el) {
    return true;
}

Element* ExpressionParser::createElement(){
    theEl = new Expression;
    return theEl;
}