#ifndef UML_PARSERS_SINGLETON_FUNCTORS_H
#define UML_PARSERS_SINGLETON_FUNCTORS_H
#include "parser.h"

namespace UML {
    namespace Parsers {

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
    }
}

#endif