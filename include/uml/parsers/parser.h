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
#include "uml/model.h"
#include "uml/literalUnlimitedNatural.h"
#include "uml/templateBinding.h"

namespace UML {
    namespace Parsers {

        class UmlParserException : public std::exception {
            private:
            std::string m_msg;
            std::string m_path;
            YAML::Node m_node;

            public:
                UmlParserException(std::string msg, std::string path, YAML::Node node) : 
                    m_msg(msg) , m_path(path), m_node(node)
                    {}
                UmlParserException(std::string msg, std::string path) :
                    m_msg(msg), m_path(path) 
                    {}
                virtual const char* what() const throw() {
                    return (m_msg + " path " + m_path + " line number " + std::to_string(m_node.Mark().line)).c_str();
                }
        };

        std::string emit(Element& el);

        class ManagerFriendFunctor {
            public:
                void operator()(UmlManager* manager, Model* model) const;
        };

        Model* parseModel(UmlManager* manager);

        Model* parseModel(ParserMetaData& data);

        UmlManager* parse(std::string path);

        Element* parse(ParserMetaData& data);

        Element* parseNode(YAML::Node node, ParserMetaData& data);
        
        void emit(YAML::Emitter& emitter, Element& el);

        // anonymous functions
        namespace {

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

            class AddTemplateParmeterFunctor : public AbstractPostProcessFunctor {
                public:
                    AddTemplateParmeterFunctor(Element* el, YAML::Node node) : AbstractPostProcessFunctor(el, node) {};
                    void operator()(Element& el) const override;
            };

            class SetParameteredElementFunctor : public AbstractPostProcessFunctor {
                public:
                    SetParameteredElementFunctor(Element* el, YAML::Node node) : AbstractPostProcessFunctor(el, node) {};
                    void operator()(Element& el) const override;
            };

            class SetSignatureFunctor : public AbstractPostProcessFunctor {
                public:
                    SetSignatureFunctor(Element* el, YAML::Node node) : AbstractPostProcessFunctor(el, node) {};
                    void operator()(Element& el) const override;
            };

            class SetFormalFunctor : public AbstractPostProcessFunctor {
                public:
                    SetFormalFunctor(Element* el, YAML::Node node) : AbstractPostProcessFunctor(el, node) {};
                    void operator()(Element& el) const override;
            };
            void emitModel(YAML::Emitter& emitter, Model& model);
            void parseElement(YAML::Node node, Element& el, ParserMetaData& data);
            void emitElement(YAML::Emitter& emitter, Element& el);
            void parseNamedElement(YAML::Node node, NamedElement& el, ParserMetaData& data);
            void emitNamedElement(YAML::Emitter& emitter, NamedElement& el);
            void parseTypedElement(YAML::Node node, TypedElement& el, ParserMetaData& data);
            void emitTypedElement(YAML::Emitter& emitter, TypedElement& el);
            ValueSpecification& determineAndParseValueSpecification(YAML::Node node, ParserMetaData& data);
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
            void emitMultiplicityElement(YAML::Emitter& emitter, MultiplicityElement& el);
            void parseInstanceValue(YAML::Node node, InstanceValue& val, ParserMetaData& data);
            void emitInstanceValue(YAML::Emitter& emitter, InstanceValue& val);
            void parsePackageMerge(YAML::Node node, PackageMerge& merge, ParserMetaData& data);
            void emitPackageMerge(YAML::Emitter& emitter, PackageMerge& merge);
            void parseLiteralBool(YAML::Node node, LiteralBool& lb, ParserMetaData& data);
            void emitLiteralBool(YAML::Emitter& emitter, LiteralBool& lb);
            void parseLiteralInt(YAML::Node node, LiteralInt& li, ParserMetaData& data);
            void emitLiteralInt(YAML::Emitter& emitter, LiteralInt& li);
            void parseLiteralReal(YAML::Node node, LiteralReal& lr, ParserMetaData& data);
            void emitLiteralReal(YAML::Emitter& emitter, LiteralReal& lr);
            void parseLiteralString(YAML::Node node, LiteralString& ls, ParserMetaData& data);
            void emitLiteralString(YAML::Emitter& emitter, LiteralString& lr);
            void parseLiteralUnlimitedNatural(YAML::Node node, LiteralUnlimitedNatural& ln, ParserMetaData& data);
            void emitLiteralUnlimitedNatural(YAML::Emitter& emitter, LiteralUnlimitedNatural& ln);
            void parseExpression(YAML::Node node, Expression& exp, ParserMetaData& data);
            void emitExpression(YAML::Emitter& emitter, Expression& exp);
            void parseTemplateableElement(YAML::Node node, TemplateableElement& el, ParserMetaData& data);
            void parseTemplateSignature(YAML::Node node, TemplateSignature& signature, ParserMetaData& data);
            ParameterableElement& determinAndParseParameterableElement(YAML::Node node, ParserMetaData& data);
            void parseTemplateParameter(YAML::Node node, TemplateParameter& parameter, ParserMetaData& data);
            void parseTemplateBinding(YAML::Node node, TemplateBinding& binding, ParserMetaData& data);
            void parseTemplateParameterSubstitution(YAML::Node node, TemplateParameterSubstitution& sub, ParserMetaData& data);
        }
    }
}

#endif