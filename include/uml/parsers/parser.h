#ifndef PARSER_H
#define PARSER_H

#include <exception>
#include "yaml-cpp/yaml.h"
#include "uml/parsers/parserMetaData.h"
#include "uml/element.h"
#include "uml/namedElement.h"
#include "uml/typedElement.h"
#include "uml/property.h"
#include "uml/parameter.h"
#include "uml/package.h"
#include "uml/packageableElement.h"
#include "uml/namespace.h"
#include "uml/classifier.h"
#include "uml/behavior.h"
#include "uml/class.h"
#include "uml/opaqueBehavior.h"
#include "uml/operation.h"
#include "uml/multiplicityElement.h"
#include "uml/primitiveType.h"
#include "uml/instanceSpecification.h"
#include "uml/enumeration.h"
#include "uml/enumerationLiteral.h"
#include "uml/instanceValue.h"
#include "uml/literalBool.h"
#include "uml/literalReal.h"
#include "uml/literalString.h"
#include "uml/activity.h"
#include "uml/literalInt.h"
#include "uml/expression.h"
#include "uml/generalization.h"

namespace UML {
    namespace Parsers {

        class SetTypeFunctor : public AbstractPostProcessFunctor {
            public:
                SetTypeFunctor(Element* el, YAML::Node node) : AbstractPostProcessFunctor(el, node) {};
                void operator()(Element& el) const override;
        };

        class SetClassifierFunctor : public AbstractPostProcessFunctor {
            public:
                SetClassifierFunctor(Element* el, YAML::Node node) : AbstractPostProcessFunctor(el, node) {};
                void operator()(Element& el) const override;
        };

        class SetDefiningFeatureFunctor : public AbstractPostProcessFunctor {
            public:
                SetDefiningFeatureFunctor(Element* el, YAML::Node node) : AbstractPostProcessFunctor(el, node) {};
                void operator()(Element& el) const override;
        };

        class SetInstanceFunctor : public AbstractPostProcessFunctor {
            public:
                SetInstanceFunctor(Element* el, YAML::Node node) : AbstractPostProcessFunctor(el, node) {};
                void operator()(Element& el) const override;
        };

        class SetMergedPackageFunctor : public AbstractPostProcessFunctor {
            public:
                SetMergedPackageFunctor(Element* el, YAML::Node node) : AbstractPostProcessFunctor(el, node) {};
                void operator()(Element& el) const override;
        };

        class SetGeneralFunctor : public AbstractPostProcessFunctor {
            public:
                SetGeneralFunctor(Element* el, YAML::Node node) : AbstractPostProcessFunctor(el, node) {};
                void operator()(Element& el) const override;
        };

        class UmlParserException : public std::exception {
            private:
            std::string m_msg;

            public:
                UmlParserException(std::string msg) : 
                    m_msg(msg)
                    {}
                virtual const char* what() const throw() {
                    return m_msg.c_str();
                }
        };

        /**
         * The idea behind this function is to have one way to parse any uml config file no matter the parent type.
         * To do this the function will have to read the first node and determine type to assign proper parent parser.
         **/
        Element* parse(std::string path);
        Element* parse(ParserMetaData& data);
        std::string emit(Element& el);
        void emit(YAML::Emitter& emitter, Element& el);
        void parseElement(YAML::Node node, Element& el, ParserMetaData& data);
        void emitElement(YAML::Emitter& emitter, Element& el);
        void parseNamedElement(YAML::Node node, NamedElement& el, ParserMetaData& data);
        void emitNamedElement(YAML::Emitter& emitter, NamedElement& el);
        void parseTypedElement(YAML::Node node, TypedElement& el, ParserMetaData& data);
        void emitTypedElement(YAML::Emitter& emitter, TypedElement& el);
        void parseProperty(YAML::Node node, Property& prop, ParserMetaData& data);
        void emitProperty(YAML::Emitter& emitter, Property& prop);
        void parseParameter(YAML::Node node, Parameter& el, ParserMetaData& data);
        void emitParameter(YAML::Emitter& emitter, Parameter& el);
        void parsePackage(YAML::Node node, Package& pckg, ParserMetaData& data);
        void emitPackage(YAML::Emitter& emitter, Package& pckg);
        void parseClassifier(YAML::Node node, Classifier& clazz, ParserMetaData& data);
        void emitClassifier(YAML::Emitter& emitter, Classifier& clazz);
        void parseGeneralization(YAML::Node node, Generalization& general, ParserMetaData& data);
        void emitGeneralization(YAML::Emitter& emitter, Generalization& generalization);
        void parseInstanceSpecification(YAML::Node node, InstanceSpecification& inst, ParserMetaData& data);
        void emitInstanceSpecification(YAML::Emitter& emitter, InstanceSpecification& inst);
        void parseSlot(YAML::Node node, Slot& slot, ParserMetaData& data);
        void emitSlot(YAML::Emitter& emitter, Slot& slot);
        void parseDataType(YAML::Node node, DataType& dataType, ParserMetaData& data);
        void emitDataType(YAML::Emitter& emitter, DataType& dataType);
        void parsePrimitiveType(YAML::Node node, PrimitiveType& type, ParserMetaData& data);
        void emitPrimitiveType(YAML::Emitter& emitter, PrimitiveType& type);
        void parseEnumeration(YAML::Node node, Enumeration& enumeration, ParserMetaData& data);
        void emitEnumeration(YAML::Emitter& emitter, Enumeration& enumeration);
        void parseEnumerationLiteral(YAML::Node node, EnumerationLiteral& literal, ParserMetaData& data);
        void emitEnumerationLiteral(YAML::Emitter& emitter, EnumerationLiteral& literal);
        void parseStructuredClassifier(YAML::Node node, StructuredClassifier& clazz, ParserMetaData& data);
        void emitStructuredClassifier(YAML::Emitter& emitter, StructuredClassifier& clazz);
        void parseBehavior(YAML::Node node, Behavior& bhv, ParserMetaData& data);
        void emitBehavior(YAML::Emitter& emitter, Behavior& bhv);
        void parseClass(YAML::Node node, Class& clazz, ParserMetaData& data);
        void emitClass(YAML::Emitter& emitter, Class& clazz);
        void parseOpaqueBehavior(YAML::Node node, OpaqueBehavior& bhv, ParserMetaData& data);
        void emitOpaqueBehavior(YAML::Emitter& emitter, OpaqueBehavior& bhv);
        void parseOperation(YAML::Node node, Operation& op, ParserMetaData& data);
        void emitOperation(YAML::Emitter& emitter, Operation& op);
        void parseMultiplicityElement(YAML::Node node, MultiplicityElement& el, ParserMetaData& data);
        void parseInstanceValue(YAML::Node node, InstanceValue& val, ParserMetaData& data);
        void parsePackageMerge(YAML::Node node, PackageMerge& merge, ParserMetaData& data);
        void parseLiteralBool(YAML::Node node, LiteralBool& lb, ParserMetaData& data);
        void parseLiteralInt(YAML::Node node, LiteralInt& li, ParserMetaData& data);
        void parseLiteralReal(YAML::Node node, LiteralReal& lr, ParserMetaData& data);
        void parseLiteralString(YAML::Node node, LiteralString& ls, ParserMetaData& data);
        void parseExpression(YAML::Node node, Expression& exp, ParserMetaData& data);
    }
}

#endif