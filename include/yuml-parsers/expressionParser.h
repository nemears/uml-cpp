#ifndef EXPRESSION_PARSER_H
#define EXPRESSION_PARSER_H

#include "typedElementParser.h"
#include "uml/expression.h"
#include "uml/instanceValue.h"
#include "uml/literalBool.h"
#include "uml/literalInt.h"
#include "uml/literalReal.h"
#include "uml/literalString.h"

class ExpressionParser : public TypedElementParser {
    protected:
        bool parseFeatures(YAML::Node node, Element* el) override;
        bool parseOperand(YAML::Node node, Element* expression);
    public:
        bool emit(YAML::Emitter& emitter, Element* el) override;
        Element* createElement() override;
        ExpressionParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            TypedElementParser(elements, postParsers), 
            NamedElementParser(elements, postParsers) {
            this->keyword = "expression";
        };
        static void addInstanceOperandLater(YAML::Node node, Element* expression, Element* inst) {
            InstanceValue* operand = new InstanceValue;
            operand->setInstance(dynamic_cast<InstanceSpecification*>(inst));
            dynamic_cast<Expression*>(expression)->operands.push_back(dynamic_cast<ValueSpecification*>(operand));
        };
};

#endif