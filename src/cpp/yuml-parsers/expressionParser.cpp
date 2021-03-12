#include "yuml-parsers/expressionParser.h"

bool ExpressionParser::parseFeatures(YAML::Node node, Element* el) {
    bool ret = NamedElementParser::parseFeatures(node, el);

    if (!parseTypeFeatures(node, el)) {
        ret = false;
    }

    if (node["operand"]) {
        if (node["operand"].IsScalar() || node["operand"].IsMap()) {
            // If it is a scalar that means it is a uuid or a value as a string
            if (!parseOperand(node["operand"], el)) {
                ret = false;
            }
        } else if (node["operand"].IsSequence()) {
            for (std::size_t i = 0; i < node["operand"].size(); i++) {
                if (isValidUUID4(node["operand"].as<string>())) {
                    // if uuid it is an instance value
                    boost::uuids::uuid operandID = boost::lexical_cast<boost::uuids::uuid>(node["operand"].as<string>());

                    parseNowOrLater(operandID, el->getID(), node, &ExpressionParser::addInstanceOperandLater);
                } else if (!parseOperand(node["operand"][i], el)) {
                    ret = false;
                }
            }
        }
    }

    if (node["symbol"]) {
        if (!node["symbol"].IsScalar()) {
            throw ElementParser::InvalidNodeTypeException(node["symbol"].Mark().line, "scalar");
        }
        dynamic_cast<Expression*>(el)->setSymbol(node["symbol"].as<string>());
    }

    return ret;
}

bool ExpressionParser::parseOperand(YAML::Node node, Element* expression) {
    
    if (node["expression"]) {
        ExpressionParser op = ExpressionParser(elements, postProcessFlag);
        Element* parsedEl = op.parseElement(node["expression"]);
        dynamic_cast<Expression*>(expression)->operands.push_back(dynamic_cast<ValueSpecification*>(parsedEl));
    } else if (node["instanceValue"]) {
        if (node["instanceValue"].IsScalar()) {
            if (isValidUUID4(node["instanceValue"].as<string>())) {
                boost::uuids::uuid instanceID = boost::lexical_cast<boost::uuids::uuid>(node["instanceValue"].as<string>());

                parseNowOrLater(instanceID, expression->getID(), node, &ExpressionParser::addInstanceOperandLater);
            }
        }
    } else if (node["literalBool"]) {
        if (node["literalBool"].IsScalar()) {
            LiteralBool* lb = new LiteralBool;
            lb->setValue(node["literalBool"].as<bool>());
            dynamic_cast<Expression*>(expression)->operands.push_back(dynamic_cast<ValueSpecification*>(lb));
        }
    } else if (node["literalInt"]) {
        if (node["literalInt"].IsScalar()) {
            LiteralInt* li = new LiteralInt;
            li->setValue(node["literalInt"].as<int>());
            dynamic_cast<Expression*>(expression)->operands.push_back(dynamic_cast<ValueSpecification*>(li));
        }
    } else if (node["literalReal"]) {
        if (node["literalReal"].IsScalar()) {
            LiteralReal* lr = new LiteralReal;
            lr->setValue(node["literalReal"].as<double>());
            dynamic_cast<Expression*>(expression)->operands.push_back(dynamic_cast<ValueSpecification*>(lr));
        }
    } else if (node["literalString"]) {
        if (node["literalString"].IsScalar()) {
            LiteralString* ls = new LiteralString;
            ls->setValue(node["literalString"].as<string>());
            dynamic_cast<Expression*>(expression)->operands.push_back(dynamic_cast<ValueSpecification*>(ls));
        }
    } else {
        // Error
        return false;
    }

    return true;
}

bool ExpressionParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::EXPRESSION) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "expression";
        emitter << YAML::Value << YAML::BeginMap;
    }

    bool ret = TypedElementParser::emit(emitter, el);

    if (!dynamic_cast<Expression*>(el)->operands.empty()) {
        emitter << YAML::Key << "operand";
        emitter << YAML::Value << YAML::BeginSeq;
        for (auto const& operand : dynamic_cast<Expression*>(el)->operands) {
            switch(operand->getElementType()) {
                case ElementType::EXPRESSION : {
                    ExpressionParser ep(elements, postProcessFlag);
                    if (!ep.emit(emitter,operand)) {
                        ret = false;
                    }
                    break;
                }
                case ElementType::LITERAL_BOOL : {
                    emitter << YAML::Value << YAML::BeginMap;
                    emitter << YAML::Key << "literalBool";
                    emitter << YAML::Value << dynamic_cast<LiteralBool*>(operand)->getValue();
                    emitter << YAML::EndMap;
                    break;
                }
                case ElementType::LITERAL_INT : {
                    emitter << YAML::Value << YAML::BeginMap;
                    emitter << YAML::Key << "literalInt";
                    emitter << YAML::Value << dynamic_cast<LiteralInt*>(operand)->getValue();
                    emitter << YAML::EndMap;
                    break;
                }
                case ElementType::LITERAL_REAL : {
                    emitter << YAML::Value << YAML::BeginMap;
                    emitter << YAML::Key << "literalReal";
                    emitter << YAML::Value << dynamic_cast<LiteralReal*>(operand)->getValue();
                    emitter << YAML::EndMap;
                    break;
                }
                case ElementType::LITERAL_STRING : {
                    emitter << YAML::Value << YAML::BeginMap;
                    emitter << YAML::Key << "literalString";
                    emitter << YAML::Value << dynamic_cast<LiteralString*>(operand)->getValue();
                    emitter << YAML::EndMap;
                    break;
                }
                case ElementType::INSTANCE_VALUE : {
                    emitter << YAML::Value << YAML::BeginMap;
                    emitter << YAML::Key << "instanceValue";
                    emitter << YAML::Value << dynamic_cast<InstanceValue*>(operand)->getInstance()->getIDstring();
                    emitter << YAML::EndMap;
                    break;
                }
            }
        }
        emitter << YAML::EndSeq;
    }

    if (dynamic_cast<Expression*>(el)->getSymbol().compare("") != 0) {
        emitter << YAML::Key << "symbol";
        emitter << YAML::Value << dynamic_cast<Expression*>(el)->getSymbol();
    }

    if (el->getElementType() == ElementType::EXPRESSION) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}

Element* ExpressionParser::createElement(){
    theEl = new Expression;
    return theEl;
}