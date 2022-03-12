#ifndef _UML_PARSERS_PARSER_H_
#define _UML_PARSERS_PARSER_H_

#include <exception>
#include "yaml-cpp/yaml.h"
#include "uml/parsers/parserMetaData.h"
#include "uml/parsers/emitterMetaData.h"
#include "uml/umlManager.h"
#include "uml/umlPtr.h"
#include "uml/set.h"
#include "uml/singleton.h"

namespace UML {
    
    class ValueSpecification;
    class PrimitiveType;
    class LiteralBool;
    class LiteralReal;
    class LiteralInt;
    class LiteralString;
    class LiteralUnlimitedNatural;

    namespace Parsers {

        class UmlParserException : public std::exception {
            private:
            std::string m_msg;

            public:
                UmlParserException(const std::string msg, const std::string path, YAML::Node node) : 
                    m_msg(msg + " path " + path + " line number " + std::to_string(node.Mark().line))
                    {};
                UmlParserException(std::string msg, std::string path) :
                    m_msg(msg) 
                    {};
                virtual const char* what() const throw() {
                    return m_msg.c_str();
                }
        };

        UmlManager* parse(std::string path);

        ElementPtr parse(ParserMetaData& data);

        ElementPtr parseString(std::string body, ParserMetaData& data);
        ElementPtr parseYAML(YAML::Node node, ParserMetaData& data);

        EmitterMetaData getData(Element& el);
        std::string emit(Element& el);
        void emit(Element& el, YAML::Emitter& emitter);
        std::string emitIndividual(Element& el);
        void emitIndividual(Element& el, YAML::Emitter& emitter);

        void emit(EmitterMetaData& data);

        // sets up yaml emitter for emitting to a new file and emits
        void emitToFile(Element& el, EmitterMetaData& data, std::string path, std::string fileName);

        namespace {
            ElementPtr parseExternalAddToManager(ParserMetaData& data, std::string path); 
        }

        ElementType elementTypeFromString(std::string eType);

        // anonymous functions
        namespace {

            ElementPtr parseNode(YAML::Node node, ParserMetaData& data);

            void emit(YAML::Emitter& emitter, Element& el, EmitterMetaData& data);
            void determineTypeAndEmit(YAML::Emitter& emitter, Element& el, EmitterMetaData& data);
            void emitScope(YAML::Emitter& emitter, Element& el, EmitterMetaData& data);
            void emitElementDefenition(YAML::Emitter& emitter, ElementType eType, std::string yamlName, Element& el, EmitterMetaData& data);
            void emitElementDefenitionEnd(YAML::Emitter& emitter, ElementType eType, Element& el);

            void emitModel(YAML::Emitter& emitter, Model& model, EmitterMetaData& data);
            void parseElement(YAML::Node node, Element& el, ParserMetaData& data);
            void emitElement(YAML::Emitter& emitter, Element& el, EmitterMetaData& data);
            void parseNamedElement(YAML::Node node, NamedElement& el, ParserMetaData& data);
            void emitNamedElement(YAML::Emitter& emitter, NamedElement& el, EmitterMetaData& data);
            void emitTypedElement(YAML::Emitter& emitter, TypedElement& el, EmitterMetaData& data);
            ValueSpecification& determineAndParseValueSpecification(YAML::Node node, ParserMetaData& data);
            void emitProperty(YAML::Emitter& emitter, Property& prop, EmitterMetaData& data);
            void parseParameter(YAML::Node node, Parameter& el, ParserMetaData& data);
            void emitParameter(YAML::Emitter& emitter, Parameter& el, EmitterMetaData& data);
            void parsePackage(YAML::Node node, Package& pckg, ParserMetaData& data);
            void emitPackage(YAML::Emitter& emitter, Package& pckg, EmitterMetaData& data);
            void emitClassifier(YAML::Emitter& emitter, Classifier& clazz, EmitterMetaData& data);
            void emitGeneralization(YAML::Emitter& emitter, Generalization& generalization, EmitterMetaData& data);
            void emitInstanceSpecification(YAML::Emitter& emitter, InstanceSpecification& inst, EmitterMetaData& data);
            void emitSlot(YAML::Emitter& emitter, Slot& slot, EmitterMetaData& data);
            void parseDataType(YAML::Node node, DataType& dataType, ParserMetaData& data);
            void emitDataType(YAML::Emitter& emitter, DataType& dataType, EmitterMetaData& data);
            void parsePrimitiveType(YAML::Node node, PrimitiveType& type, ParserMetaData& data);
            void emitPrimitiveType(YAML::Emitter& emitter, PrimitiveType& type, EmitterMetaData& data);
            void parseEnumeration(YAML::Node node, Enumeration& enumeration, ParserMetaData& data);
            void emitEnumeration(YAML::Emitter& emitter, Enumeration& enumeration, EmitterMetaData& data);
            void parseEnumerationLiteral(YAML::Node node, EnumerationLiteral& literal, ParserMetaData& data);
            void emitEnumerationLiteral(YAML::Emitter& emitter, EnumerationLiteral& literal, EmitterMetaData& data);
            void parseStructuredClassifier(YAML::Node node, StructuredClassifier& clazz, ParserMetaData& data);
            void emitStructuredClassifier(YAML::Emitter& emitter, StructuredClassifier& clazz, EmitterMetaData& data);
            void parseBehavior(YAML::Node node, Behavior& bhv, ParserMetaData& data);
            void emitBehavior(YAML::Emitter& emitter, Behavior& bhv, EmitterMetaData& data);
            void parseClass(YAML::Node node, Class& clazz, ParserMetaData& data);
            void emitClass(YAML::Emitter& emitter, Class& clazz, EmitterMetaData& data);
            void parseOpaqueBehavior(YAML::Node node, OpaqueBehavior& bhv, ParserMetaData& data);
            void emitOpaqueBehavior(YAML::Emitter& emitter, OpaqueBehavior& bhv, EmitterMetaData& data);
            void parseOperation(YAML::Node node, Operation& op, ParserMetaData& data);
            void emitOperation(YAML::Emitter& emitter, Operation& op, EmitterMetaData& data);
            void parseMultiplicityElement(YAML::Node node, MultiplicityElement& el, ParserMetaData& data);
            void emitMultiplicityElement(YAML::Emitter& emitter, MultiplicityElement& el, EmitterMetaData& data);
            void emitInstanceValue(YAML::Emitter& emitter, InstanceValue& val, EmitterMetaData& data);
            void emitPackageMerge(YAML::Emitter& emitter, PackageMerge& merge, EmitterMetaData& data);
            void parseLiteralBool(YAML::Node node, LiteralBool& lb, ParserMetaData& data);
            void emitLiteralBool(YAML::Emitter& emitter, LiteralBool& lb, EmitterMetaData& data);
            void parseLiteralInt(YAML::Node node, LiteralInt& li, ParserMetaData& data);
            void emitLiteralInt(YAML::Emitter& emitter, LiteralInt& li, EmitterMetaData& data);
            void parseLiteralReal(YAML::Node node, LiteralReal& lr, ParserMetaData& data);
            void emitLiteralReal(YAML::Emitter& emitter, LiteralReal& lr, EmitterMetaData& data);
            void parseLiteralString(YAML::Node node, LiteralString& ls, ParserMetaData& data);
            void emitLiteralString(YAML::Emitter& emitter, LiteralString& lr, EmitterMetaData& data);
            void parseLiteralUnlimitedNatural(YAML::Node node, LiteralUnlimitedNatural& ln, ParserMetaData& data);
            void emitLiteralUnlimitedNatural(YAML::Emitter& emitter, LiteralUnlimitedNatural& ln, EmitterMetaData& data);
            void parseExpression(YAML::Node node, Expression& exp, ParserMetaData& data);
            void emitExpression(YAML::Emitter& emitter, Expression& exp, EmitterMetaData& data);
            void emitTemplateableElement(YAML::Emitter& emitter, TemplateableElement& el, EmitterMetaData& data);
            void parseTemplateSignature(YAML::Node node, TemplateSignature& signature, ParserMetaData& data);
            void emitTemplateSignature(YAML::Emitter& node, TemplateSignature& signautre, EmitterMetaData& data);
            ParameterableElement& determinAndParseParameterableElement(YAML::Node node, ParserMetaData& data);
            void emitTemplateParameter(YAML::Emitter& emitter, TemplateParameter& parameter, EmitterMetaData& data);
            void emitTemplateBinding(YAML::Emitter& emitter, TemplateBinding& binding, EmitterMetaData& data);
            void emitTemplateParameterSubstitution(YAML::Emitter& emitter, TemplateParameterSubstitution& sub, EmitterMetaData& data);
            void parseAssociation(YAML::Node node, Association& association, ParserMetaData& data);
            void emitAssociation(YAML::Emitter& emitter, Association& association, EmitterMetaData& data);
            void emitExtension(YAML::Emitter& emitter, Extension& extension, EmitterMetaData& data);
            void emitProfileApplication(YAML::Emitter& emitter, ProfileApplication& application, EmitterMetaData& data);
            void parseComment(YAML::Node node, Comment& comment, ParserMetaData& data);
            void emitComment(YAML::Emitter& emitter, Comment& comment, EmitterMetaData& data);
            void parseDependency(YAML::Node node, Dependency& dependency, ParserMetaData& data);
            void emitDependency(YAML::Emitter& emitter, Dependency& dependency, EmitterMetaData& data);
            void parseDeployment(YAML::Node node, Deployment& deployment, ParserMetaData& data);
            void emitDeployment(YAML::Emitter& emitter, Deployment& deployment, EmitterMetaData& data);
            void parseArtifact(YAML::Node node, Artifact& artifact, ParserMetaData& data);
            void emitArtifact(YAML::Emitter& emitter, Artifact& artifact, EmitterMetaData& data);
            void parseDeploymentTarget(YAML::Node node, DeploymentTarget& target, ParserMetaData& data);
            void emitDeploymentTarget(YAML::Emitter& emitter, DeploymentTarget& target, EmitterMetaData& data);
            void emitBehavioredClassifier(YAML::Emitter& emitter, BehavioredClassifier& classifier, EmitterMetaData& data);
            void emitManifestation(YAML::Emitter& emitter, Manifestation& Manifestation, EmitterMetaData& data);
            void parseParameterableElement(YAML::Node node, ParameterableElement& el, ParserMetaData& data);
            void emitParameterableElement(YAML::Emitter& emitter, ParameterableElement& el, EmitterMetaData& data);
            void emitGeneralizationSet(YAML::Emitter& emitter, GeneralizationSet& generalizationSet, EmitterMetaData& data);
            void parseClassifier(YAML::Node node, Classifier& clazz, ParserMetaData& data);
            void parseGeneralization(YAML::Node node, Generalization& general, ParserMetaData& data);
            void parsePackageMerge(YAML::Node node, PackageMerge& merge, ParserMetaData& data);
            void parseTypedElement(YAML::Node node, TypedElement& el, ParserMetaData& data);
            void parseSlot(YAML::Node node, Slot& slot, ParserMetaData& data);
            void parseInstanceValue(YAML::Node node, InstanceValue& val, ParserMetaData& data);
            void parseInstanceSpecification(YAML::Node node, InstanceSpecification& inst, ParserMetaData& data);
            void parseProperty(YAML::Node node, Property& prop, ParserMetaData& data);
            void parseTemplateableElement(YAML::Node node, TemplateableElement& el, ParserMetaData& data);
            void parseTemplateParameter(YAML::Node node, TemplateParameter& parameter, ParserMetaData& data);
            void parseTemplateBinding(YAML::Node node, TemplateBinding& binding, ParserMetaData& data);
            void parseTemplateParameterSubstitution(YAML::Node node, TemplateParameterSubstitution& sub, ParserMetaData& data);
            void parseExtension(YAML::Node node, Extension& extension, ParserMetaData& data);
            void parseProfileApplication(YAML::Node node, ProfileApplication& application, ParserMetaData& data);
            void parseManifestation(YAML::Node node, Manifestation& manifestation, ParserMetaData& data);
            void parseBehavioredClassifier(YAML::Node node, BehavioredClassifier& classifier, ParserMetaData& data);
            void parseGeneralizationSet(YAML::Node node, GeneralizationSet& generalizationSet, ParserMetaData& data);
            void parseConnector(YAML::Node node, Connector& connector, ParserMetaData& data);
            void emitConnector(YAML::Emitter& emitter, Connector& connector, EmitterMetaData& data);
            void parseConnectorEnd(YAML::Node node, ConnectorEnd& end, ParserMetaData& data);
            void emitConnectorEnd(YAML::Emitter& emitter, ConnectorEnd& end, EmitterMetaData& data);
            void parsePort(YAML::Node node, Port& port, ParserMetaData& data);
            void emitPort(YAML::Emitter& emitter, Port& port, EmitterMetaData& data);
            void parseInterface(YAML::Node node, Interface& interface_uml, ParserMetaData& data);
            void emitInterface(YAML::Emitter& emitter, Interface& interface_uml, EmitterMetaData& data);
            void parseInterfaceRealization(YAML::Node node, InterfaceRealization& realization, ParserMetaData& data);
            void emitInterfaceRealization(YAML::Emitter& emitter, InterfaceRealization& realization, EmitterMetaData& data);
            void parseSignal(YAML::Node node, Signal& signal, ParserMetaData& data);
            void emitSignal(YAML::Emitter& emiiter, Signal& signal, EmitterMetaData& data);
            void parseBehavioralFeature(YAML::Node node, BehavioralFeature& feature, ParserMetaData& data);
            void emitBehavioralFeature(YAML::Emitter& emitter, BehavioralFeature& feature, EmitterMetaData& data);
            void parseReception(YAML::Node node, Reception& reception, ParserMetaData& data);
            void emitReception(YAML::Emitter& emitter, Reception& reception, EmitterMetaData& data);
        }
    }
}

#endif