#ifndef PARSER_H
#define PARSER_H

#include <exception>
#include "yaml-cpp/yaml.h"
#include "uml/parsers/parserMetaData.h"
#include "uml/parsers/emitterMetaData.h"
#include "uml/element.h"

namespace UML {

    class PrimitiveType;
    class EnumerationLiteral;
    class OpaqueBehavior;
    class InstanceValue;
    class LiteralBool;
    class LiteralInt;
    class LiteralReal;
    class LiteralString;
    class LiteralUnlimitedNatural;
    class ParameterableElement;
    class TypedElement;
    class MultiplicityElement;
    class PackageMerge;
    class Extension;
    class ProfileApplication;

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

        Element* parse(ParserMetaData& data);

        std::string emit(Element& el);

        void emit(EmitterMetaData& data);

        void emitToFile(Element& el, EmitterMetaData& data, std::string path, std::string fileName);

        // anonymous functions
        namespace {

            Element* parseNode(YAML::Node node, ParserMetaData& data);

            Element* parseExternalAddToManager();

            void emit (YAML::Emitter& emitter, Element& el, EmitterMetaData& data);

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

            class SetActualFunctor : public AbstractPostProcessFunctor {
                public:
                    SetActualFunctor(Element* el, YAML::Node node) : AbstractPostProcessFunctor(el, node) {};
                    void operator()(Element& el) const override;
            };
            
            class AddMemberEndFunctor : public AbstractPostProcessFunctor {
                public:
                    AddMemberEndFunctor(Element* el, YAML::Node node) : AbstractPostProcessFunctor(el, node) {};
                    void operator()(Element& el) const override;
            };

            class SetAppliedProfileFunctor : public AbstractPostProcessFunctor {
                public:
                    SetAppliedProfileFunctor(Element* el, YAML::Node node) : AbstractPostProcessFunctor(el, node) {};
                    void operator()(Element& el) const override;
            };

            class AddAppliedStereotypeFunctor : public AbstractPostProcessFunctor {
                protected:
                    Element& m_stereotypedEl;
                public:
                    AddAppliedStereotypeFunctor(Element* el, YAML::Node node, Element& stereotypedEl) : m_stereotypedEl(stereotypedEl), AbstractPostProcessFunctor(el, node) {};
                    void operator()(Element& el) const override;
            };
            class AddClientFunctor : public AbstractPostProcessFunctor {
                public:
                    AddClientFunctor(Element* el, YAML::Node node) : AbstractPostProcessFunctor(el, node) {};
                    void operator()(Element& el) const override;
            };
            class AddSupplierFunctor : public AbstractPostProcessFunctor {
                public:
                    AddSupplierFunctor(Element* el, YAML::Node node) : AbstractPostProcessFunctor(el, node) {};
                    void operator()(Element& el) const override;
            };
            class AddDeployedArtifactFunctor : public AbstractPostProcessFunctor {
                public:
                    AddDeployedArtifactFunctor(Element* el, YAML::Node node) : AbstractPostProcessFunctor(el, node) {};
                    void operator()(Element& el) const override;
            };
            void emitModel(YAML::Emitter& emitter, Model& model, EmitterMetaData& data);
            void parseElement(YAML::Node node, Element& el, ParserMetaData& data);
            void emitElement(YAML::Emitter& emitter, Element& el, EmitterMetaData& data);
            void parseNamedElement(YAML::Node node, NamedElement& el, ParserMetaData& data);
            void emitNamedElement(YAML::Emitter& emitter, NamedElement& el, EmitterMetaData& data);
            void parseTypedElement(YAML::Node node, TypedElement& el, ParserMetaData& data);
            void emitTypedElement(YAML::Emitter& emitter, TypedElement& el, EmitterMetaData& data);
            ValueSpecification& determineAndParseValueSpecification(YAML::Node node, ParserMetaData& data);
            void parseProperty(YAML::Node node, Property& prop, ParserMetaData& data);
            void emitProperty(YAML::Emitter& emitter, Property& prop, EmitterMetaData& data);
            void parseParameter(YAML::Node node, Parameter& el, ParserMetaData& data);
            void emitParameter(YAML::Emitter& emitter, Parameter& el, EmitterMetaData& data);
            void parsePackage(YAML::Node node, Package& pckg, ParserMetaData& data);
            void emitPackage(YAML::Emitter& emitter, Package& pckg, EmitterMetaData& data);
            void parseClassifier(YAML::Node node, Classifier& clazz, ParserMetaData& data);
            void emitClassifier(YAML::Emitter& emitter, Classifier& clazz, EmitterMetaData& data);
            void parseGeneralization(YAML::Node node, Generalization& general, ParserMetaData& data);
            void emitGeneralization(YAML::Emitter& emitter, Generalization& generalization, EmitterMetaData& data);
            void parseInstanceSpecification(YAML::Node node, InstanceSpecification& inst, ParserMetaData& data);
            void emitInstanceSpecification(YAML::Emitter& emitter, InstanceSpecification& inst, EmitterMetaData& data);
            void parseSlot(YAML::Node node, Slot& slot, ParserMetaData& data);
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
            void parseInstanceValue(YAML::Node node, InstanceValue& val, ParserMetaData& data);
            void emitInstanceValue(YAML::Emitter& emitter, InstanceValue& val, EmitterMetaData& data);
            void parsePackageMerge(YAML::Node node, PackageMerge& merge, ParserMetaData& data);
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
            void parseTemplateableElement(YAML::Node node, TemplateableElement& el, ParserMetaData& data);
            void emitTemplateableElement(YAML::Emitter& emitter, TemplateableElement& el, EmitterMetaData& data);
            void parseTemplateSignature(YAML::Node node, TemplateSignature& signature, ParserMetaData& data);
            void emitTemplateSignature(YAML::Emitter& node, TemplateSignature& signautre, EmitterMetaData& data);
            ParameterableElement& determinAndParseParameterableElement(YAML::Node node, ParserMetaData& data);
            void parseTemplateParameter(YAML::Node node, TemplateParameter& parameter, ParserMetaData& data);
            void emitTemplateParameter(YAML::Emitter& emitter, TemplateParameter& parameter, EmitterMetaData& data);
            void parseTemplateBinding(YAML::Node node, TemplateBinding& binding, ParserMetaData& data);
            void emitTemplateBinding(YAML::Emitter& emitter, TemplateBinding& binding, EmitterMetaData& data);
            void parseTemplateParameterSubstitution(YAML::Node node, TemplateParameterSubstitution& sub, ParserMetaData& data);
            void emitTemplateParameterSubstitution(YAML::Emitter& emitter, TemplateParameterSubstitution& sub, EmitterMetaData& data);
            void parseAssociation(YAML::Node node, Association& association, ParserMetaData& data);
            void emitAssociation(YAML::Emitter& emitter, Association& association, EmitterMetaData& data);
            void parseExtension(YAML::Node node, Extension& extension, ParserMetaData& data);
            void emitExtension(YAML::Emitter& emitter, Extension& extension, EmitterMetaData& data);
            ElementType elementTypeFromString(std::string eType);
            void parseProfileApplication(YAML::Node node, ProfileApplication& application, ParserMetaData& data);
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
        }
    }
}

#endif