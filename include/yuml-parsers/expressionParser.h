#ifndef EXPRESSION_PARSER_H
#define EXPRESSION_PARSER_H

#include "typedElementParser.h"
#include "uml/expression.h"

class ExpressionParser : public TypedElementParser {
    protected:
        bool parseFeatures(YAML::Node node, Element* el) override;
    public:
        bool emit(YAML::Emitter& emitter, Element* el) override;
        Element* createElement() override;
        ExpressionParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            TypedElementParser(elements, postParsers), 
            NamedElementParser(elements, postParsers) {
            this->keyword = "expression";
        };
        static void addOperandLater(YAML::Node node, Element* expression, Element* operand) {
            dynamic_cast<Expression*>(expression)->operands.push_back(dynamic_cast<ValueSpecification*>(operand));
        }
};

#endif