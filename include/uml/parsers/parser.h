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
    class Manifestation;
    class Stereotype;

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

        Element& parseString(std::string body, ParserMetaData& data);
        Element& parseYAML(YAML::Node node, ParserMetaData& data);

        EmitterMetaData getData(Element& el);
        std::string emit(Element& el);
        void emit(Element& el, YAML::Emitter& emitter);
        std::string emitIndividual(Element& el);
        void emitIndividual(Element& el, YAML::Emitter& emitter);

        void emit(EmitterMetaData& data);

        // sets up yaml emitter for emitting to a new file and emits
        void emitToFile(Element& el, EmitterMetaData& data, std::string path, std::string fileName);

        namespace {
            Element* parseExternalAddToManager(ParserMetaData& data, std::string path); 
        }

        ElementType elementTypeFromString(std::string eType);

        template <class T = Element, class U = Element> 
        void parseSingletonReference(YAML::Node node, ParserMetaData& data, std::string key, U& owner, Singleton<T,U> U::*signature) {
            if (node[key]) {
                if (node[key].IsScalar()) {
                    if (isValidID(node[key].as<std::string>())) {
                        // ID
                        ID id = ID::fromString(node[key].as<std::string>());
                        if (data.m_manager->loaded(id)) {
                            (owner.*signature).set(data.m_manager->get<T>(id));
                        } else {
                            (owner.*signature).add(id);
                        }
                    } else {
                        // Path
                        Element* parsed = parseExternalAddToManager(data, node[key].as<std::string>());
                        if (parsed) {
                            (owner.*signature).set(parsed->as<T>());
                        } else {
                            throw UmlParserException("Could not identify valid file at path " + node[key].as<std::string>(), data.m_path.string(), node[key]);
                        }
                        // throw UmlParserException("TODO, parse reference from path (seems a lil irrelevant)", data.m_path.string(), node[key]);
                    }
                } else {
                    throw UmlParserException("Invalid yaml node type for " + key + " entry, expected a scalar id", data.m_path.string(), node[key]);
                }
            }
        };

        template <class T = Element, class U = Element, class S = Set<T,U>>
        void parseSequenceReference(YAML::Node node, ParserMetaData& data, std::string key, U& owner, S& (U::*signature)()) {
            if (node[key]) {
                if (node[key].IsSequence()) {
                    for (size_t i = 0; i < node[key].size(); i++) {
                        if (isValidID(node[key][i].as<std::string>())) {
                            ID id = ID::fromString(node[key][i].as<std::string>());
                            if (data.m_manager->loaded(id)) {
                                (owner.*signature)().add(data.m_manager->get<T>(id));
                            } else {
                                (owner.*signature)().add(id);
                            }
                        }
                    }
                } else {
                    throw UmlParserException("Invalid yaml node type for " + key + " entry, expected a sequence", data.m_path.string(), node[key]);
                }
            }
        };

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

        // anonymous functions
        namespace {

            Element* parseNode(YAML::Node node, ParserMetaData& data);

            void emit(YAML::Emitter& emitter, Element& el, EmitterMetaData& data);
            void determineTypeAndEmit(YAML::Emitter& emitter, Element& el, EmitterMetaData& data);
            void emitScope(YAML::Emitter& emitter, Element& el, EmitterMetaData& data);
            void emitElementDefenition(YAML::Emitter& emitter, ElementType eType, std::string yamlName, Element& el, EmitterMetaData& data);
            void emitElementDefenitionEnd(YAML::Emitter& emitter, ElementType eType, Element& el);

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
            class SetUtilizedElementFunctor : public AbstractPostProcessFunctor {
                public:
                    SetUtilizedElementFunctor(Element* el, YAML::Node node) : AbstractPostProcessFunctor(el, node) {};
                    void operator()(Element& el) const override;
            };
            class AddClientDepencyFunctor : public AbstractPostProcessFunctor {
                public:
                    AddClientDepencyFunctor(Element* el, YAML::Node node) : AbstractPostProcessFunctor(el, node) {};
                    void operator()(Element& el) const override;
            };
            class AddSupplierDependencyFunctor : public AbstractPostProcessFunctor {
                public:
                    AddSupplierDependencyFunctor(Element* el, YAML::Node node) : AbstractPostProcessFunctor(el, node) {};
                    void operator()(Element& el) const override;
            };
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
            void parseExtension(YAML::Node node, Extension& extension, ParserMetaData& data);
            void emitExtension(YAML::Emitter& emitter, Extension& extension, EmitterMetaData& data);
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
            void parseBehavioredClassifier(YAML::Node node, BehavioredClassifier& classifier, ParserMetaData& data);
            void emitBehavioredClassifier(YAML::Emitter& emitter, BehavioredClassifier& classifier, EmitterMetaData& data);
            void parseManifestation(YAML::Node node, Manifestation& manifestation, ParserMetaData& data);
            void emitManifestation(YAML::Emitter& emitter, Manifestation& Manifestation, EmitterMetaData& data);
            void parseParameterableElement(YAML::Node node, ParameterableElement& el, ParserMetaData& data);
            void emitParameterableElement(YAML::Emitter& emitter, ParameterableElement& el, EmitterMetaData& data);
            void parseStereotype(YAML::Node node, Stereotype& stereotype, ParserMetaData& data);
            void emitStereotype(YAML::Emitter& emitter, Stereotype& stereotype, EmitterMetaData& data);
            void parseGeneralizationSet(YAML::Node node, GeneralizationSet& generalizationSet, ParserMetaData& data);
            void emitGeneralizationSet(YAML::Emitter& emitter, GeneralizationSet& generalizationSet, EmitterMetaData& data);
        }
    }
}

#endif