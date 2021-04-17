#ifndef PARSER_H
#define PARSER_H

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

#include "uml/literalString.h"
#include "uml/activity.h"
#include "uml/literalInt.h"

namespace UML {
    namespace Parsers {

        class SetTypeFunctor : public AbstractPostProcessFunctor {
            public:
                SetTypeFunctor(Element* el, YAML::Node node) : AbstractPostProcessFunctor(el, node) {};
                void operator()(Element& el) const override;
        };

        /**
         * The idea behind this function is to have one way to parse any uml config file no matter the parent type.
         * To do this the function will have to read the first node and determine type to assign proper parent parser.
         **/
        Element* parse(YAML::Node node);
        void parseElement(YAML::Node node, Element& el, ParserMetaData& data);
        void parseNamedElement(YAML::Node node, NamedElement& el, ParserMetaData& data);
        void parseTypedElement(YAML::Node node, TypedElement& el, ParserMetaData& data);
        void parseProperty(YAML::Node node, Property& prop, ParserMetaData& data);
        void parseParameter(YAML::Node node, Parameter& el, ParserMetaData& data);
        void parsePackage(YAML::Node node, Package& pckg, ParserMetaData& data);
        void parsePackageableElement(YAML::Node node, PackageableElement& el, ParserMetaData& data);
        void parseNamespace(YAML::Node node, Namespace& nmspc, ParserMetaData& data);
        void parseClassifier(YAML::Node node, Classifier& clazz, ParserMetaData& data);
        void parseBehavior(YAML::Node node, Behavior& bhv, ParserMetaData& data);
        void parseClass(YAML::Node node, Class& clazz, ParserMetaData& data);
        void parseOpaqueBehavior(YAML::Node node, OpaqueBehavior& bhv, ParserMetaData& data);
        void parseOperation(YAML::Node node, Operation& op, ParserMetaData& data);
        void parseMultiplicityElement(YAML::Node node, MultiplicityElement& el, ParserMetaData& data);
    }
}

#endif