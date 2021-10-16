#ifndef UML_PARSERS_SINGLETON_FUNCTORS_H
#define UML_PARSERS_SINGLETON_FUNCTORS_H
#include "parser.h"

namespace UML {
    namespace Parsers {

        template <class T = Element, class U = Element> class parseAndSetSingletonFunctor {
            protected:
                Singleton<T, U> U::* m_signature = 0;
            public:
                void operator()(YAML::Node node, ParserMetaData& data, U& el) const {
                    if (data.m_strategy == ParserStrategy::WHOLE) {
                        Element* packagedEl = parseExternalAddToManager(data, node.as<std::string>());
                        if (packagedEl == 0) {
                            throw UmlParserException("Could not identify YAML node for packaged elements", data.m_path.string(), node);
                        }
                        (el.*m_signature).set(packagedEl->as<T>());
                    }
                    else {
                        std::string path = node.as<std::string>();
                        std::string idStr = path.substr(path.find_last_of("/") + 1, path.find_last_of("/") + 29);
                        if (isValidID(idStr)) {
                            ID id = ID::fromString(idStr);
                            (el.*m_signature).setByID(id);
                        }
                        else {
                            throw UmlParserException("Invalid id for path, was the data specified as individual, that can only work on a mount!", data.m_path.string(), node);
                        }
                    }
                };
        };

        class SetDefaultValue : public parseAndSetSingletonFunctor<ValueSpecification, Property> {
            public:
                SetDefaultValue();
        };

        class SetSpecific : public parseAndSetSingletonFunctor<Classifier, Generalization> {
            public:
                SetSpecific();
        };

        class SetNestingClass : public parseAndSetSingletonFunctor<Class, Classifier> {
            public:
                SetNestingClass();
        };

        class OperationSetClass : public parseAndSetSingletonFunctor<Class, Operation> {
            public:
                OperationSetClass();
        };

        class PropertySetClass : public parseAndSetSingletonFunctor<Class, Property> {
            public:
                PropertySetClass();
        };

        class SetOwningPackage : public parseAndSetSingletonFunctor<Package, PackageableElement> {
            public:
                SetOwningPackage();
        };

        class PropertySetDataType : public parseAndSetSingletonFunctor<DataType, Property> {
            public:
                PropertySetDataType();
        };

        class OperationSetDataType : public parseAndSetSingletonFunctor<DataType, Operation> {
            public:
                OperationSetDataType();
        };

        class SetOwningElement : public parseAndSetSingletonFunctor<Element, Comment> {
            public:
                SetOwningElement();
        };

        class PropertySetArtifact : public parseAndSetSingletonFunctor<Artifact, Property> {
            public:
                PropertySetArtifact();
        };

        class OperationSetArtifact : public parseAndSetSingletonFunctor<Artifact, Operation> {
            public:
                OperationSetArtifact();
        };

        class ArtifactSetArtifact : public parseAndSetSingletonFunctor<Artifact, Artifact> {
            public:
                ArtifactSetArtifact();
        };

        class ManifestationSetArtifact : public parseAndSetSingletonFunctor<Artifact, Manifestation> {
            public:
                ManifestationSetArtifact();
        };

        class InstanceSpecificationSetClassifier : public parseAndSetSingletonFunctor<Classifier, InstanceSpecification> {
            public:
                InstanceSpecificationSetClassifier();
        };

        class SetOwningInstance : public parseAndSetSingletonFunctor<InstanceSpecification, Slot> {
            public:
                SetOwningInstance();
        };

        class SetDefiningFeature : public parseAndSetSingletonFunctor<StructuralFeature, Slot> {
            public:
                SetDefiningFeature();
        };

        class SetOwningSlot : public parseAndSetSingletonFunctor<Slot, ValueSpecification> {
            public:
                SetOwningSlot();
        };

        class SetSpecification : public parseAndSetSingletonFunctor<ValueSpecification, InstanceSpecification> {
            public:
                SetSpecification();
        };

        class SetInstance : public parseAndSetSingletonFunctor<InstanceSpecification, InstanceValue> {
            public:
                SetInstance();
        };

        class SetOwningInstanceSpec : public parseAndSetSingletonFunctor<InstanceSpecification, ValueSpecification> {
            public:
                SetOwningInstanceSpec();
        };

        class SetOwningAssociation : public parseAndSetSingletonFunctor<Association, Property> {
            public:
                SetOwningAssociation();
        };

        class SetAssociation : public parseAndSetSingletonFunctor<Association, Property> {
            public:
                SetAssociation();
        };

        class SetType : public parseAndSetSingletonFunctor<Type, TypedElement> {
            public:
                SetType();
        };

        class SetOperation : public parseAndSetSingletonFunctor<Operation, Parameter> {
            public:
                SetOperation();
        };

        class BehaviorSetSpecification : public parseAndSetSingletonFunctor<BehavioralFeature, Behavior> {
            public:
                BehaviorSetSpecification();
        };

        class SetBehavioredClassifier : public parseAndSetSingletonFunctor<BehavioredClassifier, Behavior> {
            public:
                SetBehavioredClassifier();
        };

        class SetBehavior : public parseAndSetSingletonFunctor<Behavior, Parameter> {
            public:
                SetBehavior();
        };

        class SetEnumeration : public parseAndSetSingletonFunctor<Enumeration, EnumerationLiteral> {
            public:
                SetEnumeration();
        };

        class SetExpression : public parseAndSetSingletonFunctor<Expression, ValueSpecification> {
            public:
                SetExpression();
        };

        class SetClassifierBehavior : public parseAndSetSingletonFunctor<Behavior, BehavioredClassifier> {
            public:
                SetClassifierBehavior();
        };

        class SetTemplate : public parseAndSetSingletonFunctor<TemplateableElement, TemplateSignature> {
            public:
                SetTemplate();
        };

        class SetOwnedTemplateSignature : public parseAndSetSingletonFunctor<TemplateSignature, TemplateableElement> {
            public:
                SetOwnedTemplateSignature();
        };

        class SetSignature : public parseAndSetSingletonFunctor<TemplateSignature, TemplateParameter> {
            public:
                SetSignature();
        };

        class SetOwnedParameteredElement : public parseAndSetSingletonFunctor<ParameterableElement, TemplateParameter> {
            public:
                SetOwnedParameteredElement();
        };

        class SetOwningTemplateParameter : public parseAndSetSingletonFunctor<TemplateParameter, ParameterableElement> {
            public:
                SetOwningTemplateParameter();
        };

        class SetTemplateParameter : public parseAndSetSingletonFunctor<TemplateParameter, ParameterableElement> {
            public:
                SetTemplateParameter();
        };
    }
}

#endif