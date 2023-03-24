#include "uml/managers/serialization/open_uml/emit.h"

using namespace std;

namespace UML {

template <class T, class V = T>
void emitFeatures(YAML::Emitter& emitter, T& el, EmitterData& data, void (*emitterFunc)(YAML::Emitter&, V&, EmitterData&)) {
    emitterFunc(emitter, el, data);
}

template <class T, class V = T, class ... Funcs>
void emitFeatures(YAML::Emitter& emitter, T& el, EmitterData& data, void (*emitterFunc)(YAML::Emitter&, V&, EmitterData&), Funcs... funcs) {
    emitFeatures(emitter, el, data, emitterFunc);
    emitFeatures(emitter, el, data, funcs...);
}

template <class T, class ... Funcs>
void emitElementTypeAndData(YAML::Emitter& emitter, T& el, EmitterData& data, string key, Funcs... funcs) {
    emitter << key << YAML::BeginMap;
    emitFeatures(emitter, el, data, funcs...);
    emitter << YAML::EndMap;
}

template <class T, class V = T>
void emitScope(YAML::Emitter& emitter, T& el, EmitterData& data, bool (*emitterFunc)(YAML::Emitter&, V&, EmitterData&)) {
    emitterFunc(emitter, el, data);
}

template <class T, class V = T, class ... Funcs>
void emitScope(YAML::Emitter& emitter, T& el, EmitterData& data, bool (*emitterFunc)(YAML::Emitter&, V&, EmitterData&), Funcs... funcs) {
    if (!emitterFunc(emitter, el, data)) {
        emitScope(emitter, el, data, funcs...);
    }
}

template <class T, class U, class S>
void emitSet(YAML::Emitter& emitter, U& el, string key, S& (U::*signature)()) {
    if (!(el.*signature)().empty()) {
        emitter << key << YAML::Value << YAML::BeginSeq;
        for (ID id : (el.*signature)().ids()) {
            emitter << id.string();
        }
        emitter << YAML::EndSeq;
    }
}

template <class T, class U, class S>
void emitOwnedSet(YAML::Emitter& emitter, U& el, EmitterData& data, string key, S& (U::*signature)()) {
    if (!(el.*signature)().empty()) {
        emitter << key << YAML::Value << YAML::BeginSeq;
        if (data.mode == SerializationMode::WHOLE) {
            for (auto& child : (el.*signature)()) {
                emitElementData(emitter, child, data);
            }
        } else {
            for (ID id : (el.*signature)().ids()) {
                emitter << id.string();
            }
        }
        emitter << YAML::EndSeq;
    }
}

template <class T, class U, class S>
bool subsetContains(U& el, T& addedEl, S& (U::*signature)()) {
    return (el.*signature)().contains(addedEl);
}

template <class T, class U, class S, class ... Subsets>
bool subsetContains(U& el, T& addedEl, S& (U::*signature)(), Subsets ... subsets) {
    if (!(el.*signature)().contains(addedEl)) {
        return subsetContains(el, addedEl, subsets...);
    }
    return true;
}

template <class T, class U, class S, class ... SubSets>
void emitOwnedSet(YAML::Emitter& emitter, U& el, EmitterData& data, string key, S& (U::*signature)(), SubSets ... subSets) {
    if (!(el.*signature)().empty()) {
        bool elementsAllInSubsets = true;
        for (auto& child : (el.*signature)()) {
            if (!subsetContains(el, child, subSets...)) {
                elementsAllInSubsets = false;
            }
        }
        if (elementsAllInSubsets) {
            return;
        }
        emitter << key << YAML::Value << YAML::BeginSeq;
        if (data.mode == SerializationMode::WHOLE) {
            for (auto& child : (el.*signature)()) {
                emitElementData(emitter, child, data);
            }
        } else {
            for (ID id : (el.*signature)().ids()) {
                emitter << id.string();
            }
        }
        emitter << YAML::EndSeq;
    }
}

template <class T, class U>
bool emitSingleton(YAML::Emitter& emitter, U& el, string key, UmlPtr<T> (U::*acessor)() const) {
    if ((el.*acessor)()) {
        emitter << key << YAML::Value << (el.*acessor)().id().string();
        return true;
    }
    return false;
}

template <class T, class U>
void emitOwnedSingleton(YAML::Emitter& emitter, U& el, EmitterData& data, string key, UmlPtr<T> (U::*acessor)() const) {
    if ((el.*acessor)()) {
        if (data.mode == SerializationMode::WHOLE) {
            emitter << key << YAML::Value;
            emitElementData(emitter, *(el.*acessor)(), data); 
        } else {
            emitter << key << YAML::Value << (el.*acessor)().id().string();
        }
    }
}

void emitElementData(YAML::Emitter& emitter, Element& el, EmitterData& data) {
    switch (el.getElementType()) {
        case ElementType::ABSTRACTION : {
            Abstraction& abstraction = el.as<Abstraction>();
            emitScope(emitter, abstraction, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, abstraction, data, "abstraction",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitDependencyFeatures);
                        // TODO emitAbstractionFeatures;
            break;
        }
        case ElementType::CLASS : {
            Class& clazz = el.as<Class>();
            emitScope(emitter, clazz, data, 
                        emitPackageableElementScope, 
                        emitParameterableElementScope,
                        emitNamedElementScope, 
                        emitElementScope);
            emitElementTypeAndData(emitter, clazz, data, "class",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitNamespaceFeatures,
                        emitTemplateableElementFeatures,
                        emitParameterableElementFeatures,
                        emitClassifierFeatures,
                        emitStructuredClassifierFeatures,
                        emitClassFeatures);
            break;
        }
        case ElementType::CLASSIFIER_TEMPLATE_PARAMETER : {
            ClassifierTemplateParameter& classifierTemplateParameter = el.as<ClassifierTemplateParameter>();
            emitScope(emitter, classifierTemplateParameter, data,
                        emitTemplateParameterScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, classifierTemplateParameter, data, "classifierTemplateParameter", 
                        emitElementFeatures,
                        emitTemplateParameterFeatures,
                        emitClassifierTemplateParameter);
            break;
        }
        case ElementType::COMMENT : {
            Comment& comment = el.as<Comment>();
            emitScope(emitter, comment, data, emitElementScope);
            emitElementTypeAndData(emitter, comment, data, "comment", emitElementFeatures, emitCommentFeatures);
            break;
        }
        case ElementType::CONSTRAINT : {
            Constraint& constraint = el.as<Constraint>();
            emitScope(emitter, constraint, data, 
                        emitConstraintScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, constraint, data, "constraint",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitConstraintFeatures);
            break;
        }
        case ElementType::DATA_TYPE : {
            DataType& dataType = el.as<DataType>();
            emitScope(emitter, dataType, data, 
                        emitPackageableElementScope, 
                        emitParameterableElementScope,
                        emitNamedElementScope, 
                        emitElementScope);
            emitElementTypeAndData(emitter, dataType, data, "dataType",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitNamespaceFeatures,
                        emitTemplateableElementFeatures,
                        emitParameterableElementFeatures,
                        emitClassifierFeatures,
                        emitDataTypeFeatures);
            break;
        }
        case ElementType::DEPENDENCY : {
            Dependency& dependency = el.as<Dependency>();
            emitScope(emitter, dependency, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, dependency, data, "dependency",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitDependencyFeatures);
            break;
        }
        case ElementType::ELEMENT_IMPORT : {
            ElementImport& elementImport = el.as<ElementImport>();
            emitScope(emitter, elementImport, data,
                        emitElementImportScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, elementImport, data, "elementImport",
                        emitElementFeatures,
                        emitElementImportFeatures);
            break;
        }
        case ElementType::ENUMERATION : {
            Enumeration& enumeration = el.as<Enumeration>();
            emitScope(emitter, enumeration, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, enumeration, data, "enumeration", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitNamespaceFeatures,
                        emitTemplateableElementFeatures,
                        emitParameterableElementFeatures,
                        emitClassifierFeatures,
                        emitDataTypeFeatures,
                        emitEnumerationFeatures);
            break;
        }
        case ElementType::ENUMERATION_LITERAL : {
            EnumerationLiteral& literal = el.as<EnumerationLiteral>();
            emitScope(emitter, literal, data,
                        emitEnumerationLiteralScope, 
                        emitPackageableElementScope, 
                        emitParameterableElementScope,
                        emitNamedElementScope, 
                        emitElementScope);
            emitElementTypeAndData(emitter, literal, data, "enumerationLiteral", 
                        emitElementFeatures, 
                        emitNamedElementFeatures, 
                        emitParameterableElementFeatures,
                        emitInstanceSpecificationFeatures);
            break;
        }
        case ElementType::EXPRESSION : {
            Expression& expression = el.as<Expression>();
            emitScope(emitter, expression, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, expression, data, "expression",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitTypedElementFeatures,
                        emitParameterableElementFeatures,
                        emitExpressionFeatures);
            break;
        }
        case ElementType::GENERALIZATION : {
            Generalization& generalization = el.as<Generalization>();
            emitScope(emitter, generalization, data,
                        emitGeneralizationScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, generalization, data, "generalization",
                        emitElementFeatures,
                        emitGeneralizationFeatures);
            break;
        }
        case ElementType::GENERALIZATION_SET : {
            GeneralizationSet& generalizationSet = el.as<GeneralizationSet>();
            emitScope(emitter, generalizationSet, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, generalizationSet, data, "generalizationSet",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitGeneralizationSetFeatures);
            break;
        }
        case ElementType::INSTANCE_SPECIFICATION : {
            InstanceSpecification& inst = el.as<InstanceSpecification>();
            emitScope(emitter, inst, data, 
                        emitPackageableElementScope, 
                        emitParameterableElementScope,
                        emitNamedElementScope, 
                        emitElementScope);
            emitElementTypeAndData(emitter, inst, data, "instanceSpecification", 
                        emitElementFeatures, 
                        emitNamedElementFeatures, 
                        emitParameterableElementFeatures,
                        emitInstanceSpecificationFeatures);
            break;
        }
        case ElementType::INSTANCE_VALUE : {
            InstanceValue& instanceValue = el.as<InstanceValue>();
            emitScope(emitter, instanceValue, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, instanceValue, data, "instanceValue",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitTypedElementFeatures,
                        emitParameterableElementFeatures,
                        emitInstanceValueFeatures);
            break;
        }
        case ElementType::LITERAL_BOOL : {
            LiteralBool& literalBool = el.as<LiteralBool>();
            emitScope(emitter, literalBool, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, literalBool, data, "literalBool",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitTypedElementFeatures,
                        emitLiteralBoolFeatures);
            break;
        }
        case ElementType::LITERAL_INT : {
            LiteralInt& literalInt = el.as<LiteralInt>();
            emitScope(emitter, literalInt, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, literalInt, data, "literalInt",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitTypedElementFeatures,
                        emitLiteralIntFeatures);
            break;
        }
        case ElementType::LITERAL_REAL : {
            LiteralReal& literalReal = el.as<LiteralReal>();
            emitScope(emitter, literalReal, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, literalReal, data, "literalReal",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitTypedElementFeatures,
                        emitLiteralRealFeatures);
            break;
        }
        case ElementType::LITERAL_STRING : {
            LiteralString& literalString = el.as<LiteralString>();
            emitScope(emitter, literalString, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, literalString, data, "literalString",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitTypedElementFeatures,
                        emitLiteralStringFeatures);
            break;
        }
        case ElementType::LITERAL_UNLIMITED_NATURAL : {
            LiteralUnlimitedNatural& literalUnlimitedNatural = el.as<LiteralUnlimitedNatural>();
            emitScope(emitter, literalUnlimitedNatural, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, literalUnlimitedNatural, data, "literalUnlimitedNatural",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitTypedElementFeatures,
                        emitLiteralUnlimitedNaturalFeatures);
            break;
        }
        case ElementType::OPERATION : {
            Operation& operation = el.as<Operation>();
            emitScope(emitter, operation, data,
                        emitOperationScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, operation, data, "operation", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitTemplateableElementFeatures,
                        emitFeatureFeatures,
                        emitBehavioralFeatureFeatures);
            break;
        }
        case ElementType::PACKAGE : {
            Package& package = el.as<Package>();
            emitScope(emitter, package, data, 
                        emitPackageableElementScope, 
                        emitParameterableElementScope, 
                        emitNamedElementScope, 
                        emitElementScope);
            emitElementTypeAndData(emitter, package, data, "package",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitNamespaceFeatures,
                        emitTemplateableElementFeatures,
                        emitPackageFeatures);
            break;
        }
        case ElementType::PACKAGE_IMPORT : {
            PackageImport& packageImport = el.as<PackageImport>();
            emitScope(emitter, packageImport, data,
                        emitPackageImportScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, packageImport, data, "packageImport",
                        emitPackageImportFeatures,
                        emitElementFeatures);
            break;
        }
        case ElementType::PARAMETER : {
            Parameter& parameter = el.as<Parameter>();
            emitScope(emitter, parameter, data,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, parameter, data, "paramter", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitTypedElementFeatures,
                        emitMultiplicityElementFeatures,
                        emitParameterFeatures);
            break;
        }
        case ElementType::PARAMETER_SET : {
            ParameterSet& parameterSet = el.as<ParameterSet>();
            emitScope(emitter, parameterSet, data, 
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, parameterSet, data, "parameterSet",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterSetFeatures);
            break;
        }
        case ElementType::PRIMITIVE_TYPE : {
            PrimitiveType& primitiveType = el.as<PrimitiveType>();
            emitScope(emitter, primitiveType, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, primitiveType, data, "primitiveType", 
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitNamespaceFeatures,
                        emitTemplateableElementFeatures,
                        emitParameterableElementFeatures,
                        emitClassifierFeatures,
                        emitDataTypeFeatures);
            break;
        }
        case ElementType::PROPERTY : {
            Property& property = el.as<Property>();
            emitScope(emitter, property, data, 
                        emitPropertyScope, 
                        emitParameterableElementScope,
                        emitNamedElementScope, 
                        emitElementScope);
            emitElementTypeAndData(emitter, property, data, "property",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitFeatureFeatures,
                        emitTypedElementFeatures,
                        emitStructuralFeatureFeatures,
                        emitMultiplicityElementFeatures,
                        emitParameterableElementFeatures,
                        emitPropertyFeatures);
            break;
        }
        case ElementType::REDEFINABLE_TEMPLATE_SIGNATURE : {
            RedefinableTemplateSignature& redefinableTemplateSignature = el.as<RedefinableTemplateSignature>();
            emitScope(emitter, redefinableTemplateSignature, data,
                        emitRedefinableTemplateSignatureScope,
                        emitTemplateSignatureScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, redefinableTemplateSignature, data, "redefinableTemplateSignature",
                        emitElementFeatures,
                        emitTemplateSignatureFeatures,
                        emitRedefinableTemplateSignatureFeatures);
            break;
        }
        case ElementType::REALIZATION : {
            Realization& realization = el.as<Realization>();
            emitScope(emitter, realization, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, realization, data, "realization",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitDependencyFeatures);
                        // TODO emitAbstractionFeatures;
            break;
        }
        case ElementType::SLOT : {
            Slot& slot = el.as<Slot>();
            emitScope(emitter, slot, data,
                        emitSlotScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, slot, data, "slot", 
                        emitElementFeatures,
                        emitSlotFeatures);
            break;
        }
        case ElementType::TEMPLATE_BINDING : {
            TemplateBinding& templateBinding = el.as<TemplateBinding>();
            emitScope(emitter, templateBinding, data,
                        emitTemplateBindingScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, templateBinding, data, "templateBinding",
                        emitElementFeatures,
                        emitTemplateBindingFeatures);
            break;
        }
        case ElementType::TEMPLATE_PARAMETER : {
            TemplateParameter& templateParameter = el.as<TemplateParameter>();
            emitScope(emitter, templateParameter, data, 
                        emitTemplateParameterScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, templateParameter, data, "templateParameter",
                        emitTemplateParameterFeatures,
                        emitElementFeatures);
            break;
        }
        case ElementType::TEMPLATE_PARAMETER_SUBSTITUTION : {
            TemplateParameterSubstitution& templateParameterSubstitution = el.as<TemplateParameterSubstitution>();
            emitScope(emitter, templateParameterSubstitution, data,
                        emitTemplateParameterSubstitutionScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, templateParameterSubstitution, data, "templateParameterSubstitution", 
                        emitTemplateParameterSubstitutionFeatures,
                        emitElementFeatures);
            break;
        }
        case ElementType::TEMPLATE_SIGNATURE : {
            TemplateSignature& templateSignature = el.as<TemplateSignature>();
            emitScope(emitter, templateSignature, data, emitTemplateSignatureScope, emitElementScope);
            emitElementTypeAndData(emitter, templateSignature, data, "templateSignature",
                        emitElementFeatures,
                        emitTemplateSignatureFeatures);
            break;
        }
        case ElementType::USAGE : {
            Usage& usage = el.as<Usage>();
            emitScope(emitter, usage, data,
                        emitPackageableElementScope,
                        emitParameterableElementScope,
                        emitNamedElementScope,
                        emitElementScope);
            emitElementTypeAndData(emitter, usage, data, "usage",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitParameterableElementFeatures,
                        emitDependencyFeatures);
            break;
        }
        default : {
            throw SerializationError("No strategy to serialize element of type " + Element::elementTypeToString(el.getElementType()));
        }
    }
}

void emitBehavioralFeatureFeatures(YAML::Emitter& emitter, BehavioralFeature& behavioralFeature, EmitterData& data) {
    if (behavioralFeature.getConcurrency() != CallConcurrencyKind::Sequential) {
        emitter << "concurrency" << YAML::Value;
        switch (behavioralFeature.getConcurrency()) {
            case CallConcurrencyKind::Concurrent : {
                emitter << "concurrent";
                break;
            }
            case CallConcurrencyKind::Guarded : {
                emitter << "guarded";
                break;
            }
        }
    }
    emitSet<Type>(emitter, behavioralFeature, "raisedExceptions", &BehavioralFeature::getRaisedExceptions);
    emitSet<Behavior>(emitter, behavioralFeature, "methods", &BehavioralFeature::getMethods);
    emitOwnedSet<Parameter>(emitter, behavioralFeature, data, "ownedParameters", &BehavioralFeature::getOwnedParameters);
    emitOwnedSet<ParameterSet>(emitter, behavioralFeature, data, "ownedParameterSets", &BehavioralFeature::getOwnedParameterSets);
}

void emitClassFeatures(YAML::Emitter& emitter, Class& clazz, EmitterData& data) {
    emitOwnedSet<Classifier>(emitter, clazz, data, "nestedClassifiers", &Class::getNestedClassifiers);
    emitOwnedSet<Property>(emitter, clazz, data, "ownedAttributes", &Class::getOwnedAttributes);
    emitOwnedSet<Operation>(emitter, clazz, data, "ownedOperations", &Class::getOwnedOperations);
    emitOwnedSet<Reception>(emitter, clazz, data, "ownedReceptions", &Class::getOwnedReceptions);
}

void emitClassifierFeatures(YAML::Emitter& emitter, Classifier& classifier, EmitterData& data) {
    emitOwnedSet<Generalization>(emitter, classifier, data, "generalizations", &Classifier::getGeneralizations);
    emitSet<GeneralizationSet>(emitter, classifier, "powertypeExtent", &Classifier::getPowerTypeExtent);
}

void emitClassifierTemplateParameter(YAML::Emitter& emitter, ClassifierTemplateParameter& classifierTemplateParameter, EmitterData& data) {
    emitSet<Classifier>(emitter, classifierTemplateParameter, "constrainingClassifiers", &ClassifierTemplateParameter::getConstrainingClassifiers);
}

void emitCommentFeatures(YAML::Emitter& emitter, Comment& comment, EmitterData& data) {
    emitSet<Element>(emitter, comment, "annotatedElements", &Comment::getAnnotatedElements);
    if (!comment.getBody().empty()) {
        emitter << "body" << YAML::Value << comment.getBody();
    }
}

void emitConstraintFeatures(YAML::Emitter& emitter, Constraint& constraint, EmitterData& data) {
    emitSet<Element>(emitter, constraint, "constrainedElements", &Constraint::getConstrainedElements);
    emitOwnedSingleton<ValueSpecification>(emitter, constraint, data, "specification", &Constraint::getSpecification);
}

void emitDataTypeFeatures(YAML::Emitter& emitter, DataType& dataType, EmitterData& data) {
    emitOwnedSet<Property>(emitter, dataType, data, "ownedAttributes", &DataType::getOwnedAttributes);
    emitOwnedSet<Operation>(emitter, dataType, data, "ownedOperations", &DataType::getOwnedOperations);
}

void emitDependencyFeatures(YAML::Emitter& emitter, Dependency& dependency, EmitterData& data) {
    emitSet<NamedElement>(emitter, dependency, "suppliers", &Dependency::getSuppliers);
    emitSet<NamedElement>(emitter, dependency, "clients", &Dependency::getClients);
}

void emitElementFeatures(YAML::Emitter& emitter, Element& el, EmitterData& data) {
    emitter << "id" << YAML::Value << el.getID().string();
    emitOwnedSet<InstanceSpecification>(emitter, el, data, "appliedStereotypes", &Element::getAppliedStereotypes);
}

template <class T>
void emitVisibilityKind(YAML::Emitter& emitter, T& elWithVisibility) {
    if (elWithVisibility.getVisibility() != VisibilityKind::PUBLIC) {
        emitter << "visibility" << YAML::Value;
        switch (elWithVisibility.getVisibility()) {
            case VisibilityKind::PACKAGE : {
                emitter << "package";
                break;
            }
            case VisibilityKind::PRIVATE : {
                emitter << "private";
                break;
            }
            case VisibilityKind::PROTECTED : {
                emitter << "protected";
                break;
            }
        }
    }
}

void emitElementImportFeatures(YAML::Emitter& emitter, ElementImport& elementImport, EmitterData& data) {
    emitSingleton<PackageableElement>(emitter, elementImport, "importedElement", &ElementImport::getImportedElement);
    if (!elementImport.getAlias().empty()) {
        emitter << "alias" << YAML::Value << elementImport.getAlias();
    }
    emitVisibilityKind(emitter, elementImport);
}

void emitEnumerationFeatures(YAML::Emitter& emitter, Enumeration& enumeration, EmitterData& data) {
    emitOwnedSet<EnumerationLiteral>(emitter, enumeration, data, "ownedLiterals", &Enumeration::getOwnedLiterals);
}

void emitExpressionFeatures(YAML::Emitter& emitter, Expression& expression, EmitterData& data) {
    if (!expression.getSymbol().empty()) {
        emitter << expression.getSymbol();
    }
    emitOwnedSet<ValueSpecification>(emitter, expression, data, "operands", &Expression::getOperands);
}

void emitFeatureFeatures(YAML::Emitter& emitter, Feature& feature, EmitterData& data) {
    if (feature.isStatic()) {
        emitter << "isStatic" << YAML::Value << true;
    }
}

void emitGeneralizationFeatures(YAML::Emitter& emitter, Generalization& generalization, EmitterData& data) {
    emitSingleton<Classifier>(emitter, generalization, "general", &Generalization::getGeneral);
    emitSet<GeneralizationSet>(emitter, generalization, "generalizationSets", &Generalization::getGeneralizationSets);
}

void emitGeneralizationSetFeatures(YAML::Emitter& emitter, GeneralizationSet& generalizationSet, EmitterData& data) {
    emitter << "isCovering" << YAML::Value << generalizationSet.isCovering();
    emitter << "isDisjoint" << YAML::Value << generalizationSet.isDisjoint();
    emitSingleton<Classifier>(emitter, generalizationSet, "powertype", &GeneralizationSet::getPowerType);
    emitSet<Generalization>(emitter, generalizationSet, "generalizations", &GeneralizationSet::getGeneralizations);
}

void emitInstanceSpecificationFeatures(YAML::Emitter& emitter, InstanceSpecification& specification, EmitterData& data) {
    emitOwnedSet<Slot>(emitter, specification, data, "slots", &InstanceSpecification::getSlots);
    emitOwnedSingleton<ValueSpecification>(emitter, specification, data, "specification", &InstanceSpecification::getSpecification);
    emitSet<Classifier>(emitter, specification, "classifiers", &InstanceSpecification::getClassifiers);
}

void emitInstanceValueFeatures(YAML::Emitter& emitter, InstanceValue& instanceValue, EmitterData& data) {
    emitSingleton<InstanceSpecification>(emitter, instanceValue, "instance", &InstanceValue::getInstance);
}

void emitLiteralBoolFeatures(YAML::Emitter& emitter, LiteralBool& literalBool, EmitterData& data) {
    emitter << "value" << YAML::Value << literalBool.getValue();
}

void emitLiteralIntFeatures(YAML::Emitter& emitter, LiteralInt& literalInt, EmitterData& data) {
    emitter << "value" << YAML::Value << literalInt.getValue();
}

void emitLiteralRealFeatures(YAML::Emitter& emitter, LiteralReal& literalReal, EmitterData& data) {
    emitter << "value" << YAML::Value << literalReal.getValue();
}

void emitLiteralStringFeatures(YAML::Emitter& emitter, LiteralString& literalString, EmitterData& data) {
    if (!literalString.getValue().empty()) {
        emitter << "value" << YAML::Value << literalString.getValue();
    }
}

void emitLiteralUnlimitedNaturalFeatures(YAML::Emitter& emitter, LiteralUnlimitedNatural& literalUnlimitedNatural, EmitterData& data) {
    emitter << "value" << YAML::Value;
    if (literalUnlimitedNatural.isInfinite()) {
        emitter << "*";
    } else {
        emitter << literalUnlimitedNatural.getNumberValue();
    }
}

void emitMultiplicityElementFeatures(YAML::Emitter& emitter, MultiplicityElement& multiplicityElement, EmitterData& data) {
    if (multiplicityElement.isOrdered()) {
        emitter << "isOrdered" << YAML::Value << true;
    }
    if (multiplicityElement.isUnique()) {
        emitter << "isUnique" << YAML::Value << true;
    }
    if (!emitSingleton<ValueSpecification>(emitter, multiplicityElement, "lowerValue", &MultiplicityElement::getLowerValue)) {
        if (multiplicityElement.getLower() != -1) {
            emitter << "lower" << YAML::Value << multiplicityElement.getLower();
        }
    }
    if (!emitSingleton<ValueSpecification>(emitter, multiplicityElement, "upperValue", &MultiplicityElement::getUpperValue)) {
        if (multiplicityElement.getUpper() != -1) {
            emitter << "lower" << YAML::Value << multiplicityElement.getUpper();
        }
    }
}

void emitNamedElementFeatures(YAML::Emitter& emitter, NamedElement& el, EmitterData& data) {
    if (!el.getName().empty()) {
        emitter << "name" << YAML::Value << el.getName();
    }
    emitVisibilityKind(emitter, el);
}

void emitNamespaceFeatures(YAML::Emitter& emitter, Namespace& nmspc, EmitterData& data) {
    emitOwnedSet<ElementImport>(emitter, nmspc, data, "elementImports", &Namespace::getElementImports);
    emitOwnedSet<PackageImport>(emitter, nmspc, data, "packageImports", &Namespace::getPackageImports);
    emitOwnedSet<Constraint>(emitter, nmspc, data, "ownedRules", &Namespace::getOwnedRules);
}

void emitPackageFeatures(YAML::Emitter& emitter, Package& package, EmitterData& data) {
    emitOwnedSet<PackageableElement>(emitter, package, data, "packagedElements", &Package::getPackagedElements);
}

void emitPackageImportFeatures(YAML::Emitter& emitter, PackageImport& packageImport, EmitterData& data) {
    emitSingleton<Package>(emitter, packageImport, "importedPackage", &PackageImport::getImportedPackage);
    emitVisibilityKind(emitter, packageImport);
}

void emitParameterFeatures(YAML::Emitter& emitter, Parameter& parameter, EmitterData& data) {
    if (parameter.getDirection() != ParameterDirectionKind::NONE) {
        emitter << "direction" << YAML::Value;
        switch (parameter.getDirection()) {
            case ParameterDirectionKind::IN_UML : {
                emitter << "in";
                break;
            }
            case ParameterDirectionKind::INOUT : {
                emitter << "inout";
                break;
            }
            case ParameterDirectionKind::OUT_UML : {
                emitter << "out";
                break;
            }
            case ParameterDirectionKind::RETURN : {
                emitter << "return";
            }
            default : {
                throw SerializationError(" bad direction");
            }
        }
    }
    if (parameter.isException()) {
        emitter << "isException" << YAML::Value << true;
    }
    if (parameter.isStream()) {
        emitter << "isStream" << YAML::Value << true;
    }
    emitOwnedSingleton<ValueSpecification>(emitter, parameter, data, "defaultValue", &Parameter::getDefaultValue);
    emitSet<ParameterSet>(emitter, parameter, "parameterSets", &Parameter::getParameterSets);
}

void emitParameterableElementFeatures(YAML::Emitter& emitter, ParameterableElement& parameterableElement, EmitterData& data) {
    if (!parameterableElement.getOwningTemplateParameter()) {
        emitSingleton<TemplateParameter>(emitter, parameterableElement, "templateParameter", &ParameterableElement::getTemplateParameter);
    }
}

void emitParameterSetFeatures(YAML::Emitter& emitter, ParameterSet& parameterSet, EmitterData& data) {
    emitSet<Parameter>(emitter, parameterSet, "parameters", &ParameterSet::getParameters);
    emitOwnedSet<Constraint>(emitter, parameterSet, data, "conditions", &ParameterSet::getConditions);
}

void emitPropertyFeatures(YAML::Emitter& emitter, Property& property, EmitterData& data) {
    if (property.getAggregation() != AggregationKind::NONE) {
        emitter << "aggregation" << YAML::Value;
        switch (property.getAggregation()) {
            case AggregationKind::COMPOSITE : {
                emitter << "composite";
                break;
            }
            case AggregationKind::SHARED : {
                emitter << "shared";
                break;
            }
        }
    }
    if (property.isComposite()) {
        emitter << "isComposite" << YAML::Value << true;
    }
    emitOwnedSingleton<ValueSpecification>(emitter, property, data, "defaultValue", &Property::getDefaultValue);
    emitSingleton<Association>(emitter, property, "association", &Property::getAssociation);
    emitSet<Property>(emitter, property, "redefinedProperties", &Property::getRedefinedProperties);
}

void emitRedefinableTemplateSignatureFeatures(YAML::Emitter& emitter, RedefinableTemplateSignature& redefinableTemplateSignature, EmitterData& data) {
    emitSet<RedefinableTemplateSignature>(emitter, redefinableTemplateSignature, "extendedSignatures", &RedefinableTemplateSignature::getExtendedSignatures);
}

void emitSlotFeatures(YAML::Emitter& emitter, Slot& slot, EmitterData& data) {
    emitOwnedSet<ValueSpecification>(emitter, slot, data, "values", &Slot::getValues);
    emitSingleton<StructuralFeature>(emitter, slot, "definingFeature", &Slot::getDefiningFeature);
}

void emitStructuralFeatureFeatures(YAML::Emitter& emitter, StructuralFeature& structuralFeature, EmitterData& data) {
    if (structuralFeature.isReadOnly()) {
        emitter << "isReadOnly" << YAML::Value << true;
    }
}

void emitStructuredClassifierFeatures(YAML::Emitter& emitter, StructuredClassifier& structuredClassifier, EmitterData& data) {
    emitOwnedSet<Connector>(emitter, structuredClassifier, data, "ownedConnectors", &StructuredClassifier::getOwnedConnectors);
}

void emitTemplateableElementFeatures(YAML::Emitter& emitter, TemplateableElement& templateableElement, EmitterData& data) {
    emitOwnedSingleton<TemplateSignature>(emitter, templateableElement, data, "ownedTemplateSignature", &TemplateableElement::getOwnedTemplateSignature);
    emitOwnedSet<TemplateBinding>(emitter, templateableElement, data, "templateBindings", &TemplateableElement::getTemplateBindings);
}

void emitTemplateBindingFeatures(YAML::Emitter& emitter, TemplateBinding& templateBinding, EmitterData& data) {
    emitOwnedSet<TemplateParameterSubstitution>(emitter, templateBinding, data, "parameterSubstitutions", &TemplateBinding::getParameterSubstitutions);
    emitSingleton<TemplateSignature>(emitter, templateBinding, "signature", &TemplateBinding::getSignature);
}

void emitTemplateParameterFeatures(YAML::Emitter& emitter, TemplateParameter& templateParameter, EmitterData& data) {
    if (!emitSingleton<ParameterableElement>(emitter, templateParameter, "ownedParameteredElement", &TemplateParameter::getOwnedParameteredElement)) {
        emitSingleton<ParameterableElement>(emitter, templateParameter, "parameteredElement", &TemplateParameter::getParameteredElement);
    }
    if (!emitSingleton<ParameterableElement>(emitter, templateParameter, "ownedDefault", &TemplateParameter::getOwnedDefault)) {
        emitSingleton<ParameterableElement>(emitter, templateParameter, "default", &TemplateParameter::getDefault);
    }
}

void emitTemplateParameterSubstitutionFeatures(YAML::Emitter& emitter, TemplateParameterSubstitution& templateParameterSubstitution, EmitterData& data) {
    emitSingleton<TemplateParameter>(emitter, templateParameterSubstitution, "formal", &TemplateParameterSubstitution::getFormal);
    if (!emitSingleton<ParameterableElement>(emitter, templateParameterSubstitution, "ownedActual", &TemplateParameterSubstitution::getOwnedActual)) {
        emitSingleton<ParameterableElement>(emitter, templateParameterSubstitution, "actual", &TemplateParameterSubstitution::getActual);
    }
}

void emitTemplateSignatureFeatures(YAML::Emitter& emitter, TemplateSignature& signature, EmitterData& data) {
    emitSet<TemplateParameter>(emitter, signature, "parameters", &TemplateSignature::getParameters);
    emitOwnedSet<TemplateParameter>(emitter, signature, data, "ownedParameters", &TemplateSignature::getOwnedParameters, &TemplateSignature::getParameters);
}

void emitTypedElementFeatures(YAML::Emitter& emitter, TypedElement& typedElement, EmitterData& data) {
    emitSingleton<Type>(emitter, typedElement, "type", &TypedElement::getType);
}

bool emitConstraintScope(YAML::Emitter& emitter, Constraint& constraint, EmitterData& data) {
    return emitSingleton<Namespace>(emitter, constraint, "context", &Constraint::getContext);
}

bool emitElementScope(YAML::Emitter& emitter, Element& el, EmitterData& data) {
    return emitSingleton<Element>(emitter, el, "owner", &Element::getOwner);
}

bool emitElementImportScope(YAML::Emitter& emitter, ElementImport& elementImport, EmitterData& data) {
    return emitSingleton<Namespace>(emitter, elementImport, "importingNamespace", &ElementImport::getImportingNamespace);
}

bool emitEnumerationLiteralScope(YAML::Emitter& emitter, EnumerationLiteral& literal, EmitterData& data) {
    return emitSingleton<Enumeration>(emitter, literal, "enumeration", &EnumerationLiteral::getEnumeration);
}

bool emitGeneralizationScope(YAML::Emitter& emitter, Generalization& generalization, EmitterData& data) {
    return emitSingleton<Classifier>(emitter, generalization, "specific", &Generalization::getSpecific);
}

bool emitNamedElementScope(YAML::Emitter& emitter, NamedElement& namedElement, EmitterData& data) {
    return emitSingleton<Namespace>(emitter, namedElement, "namespace", &NamedElement::getNamespace);
}

bool emitOperationScope(YAML::Emitter& emitter, Operation& operation, EmitterData& data) {
    if (emitSingleton<Class>(emitter, operation, "class", &Operation::getClass)) {
        return true;
    } else if (emitSingleton<DataType>(emitter, operation, "dataType", &Operation::getDataType)) {
        return true;
    } else if (emitSingleton<Interface>(emitter, operation, "interface", &Operation::getInterface)) {
        return true;
    }
    return false;
}

bool emitPackageableElementScope(YAML::Emitter& emitter, PackageableElement& el, EmitterData& data) {
    return emitSingleton<Package>(emitter, el, "owningPackage", &PackageableElement::getOwningPackage);
}

bool emitPackageImportScope(YAML::Emitter& emitter, PackageImport& packageImport, EmitterData& data) {
    return emitSingleton<Namespace>(emitter, packageImport, "importingNamespace", &PackageImport::getImportingNamespace);
}

bool emitParameterableElementScope(YAML::Emitter& emitter, ParameterableElement& el, EmitterData& data) {
    return emitSingleton<TemplateParameter>(emitter, el, "owningTemplateParameter", &ParameterableElement::getOwningTemplateParameter);
}

bool emitPropertyScope(YAML::Emitter& emitter, Property& property, EmitterData& data) {
    if (emitSingleton<Class>(emitter, property, "class", &Property::getClass)) {
        return true;
    } else if (emitSingleton<DataType>(emitter, property, "dataType", &Property::getDataType)) {
        return true;
    } else if (emitSingleton<Interface>(emitter, property, "interface", &Property::getInterface)) {
        return true;
    } else if (emitSingleton<Association>(emitter, property, "owningAssociation", &Property::getOwningAssociation)) {
        return true;
    }
    return false;
}

bool emitRedefinableTemplateSignatureScope(YAML::Emitter& emitter, RedefinableTemplateSignature& redefinableTemplateSignature, EmitterData& data) {
    return emitSingleton<Classifier>(emitter, redefinableTemplateSignature, "classifier", &RedefinableTemplateSignature::getClassifier);
}

bool emitSlotScope(YAML::Emitter& emitter, Slot& slot, EmitterData& data) {
    return emitSingleton<InstanceSpecification>(emitter, slot, "owningInstance", &Slot::getOwningInstance);
}

bool emitTemplateBindingScope(YAML::Emitter& emitter, TemplateBinding& templateBinding, EmitterData& data) {
    return emitSingleton<TemplateableElement>(emitter, templateBinding, "boundElement", &TemplateBinding::getBoundElement);
}

bool emitTemplateParameterScope(YAML::Emitter& emitter, TemplateParameter& templateParameter, EmitterData& data) {
    return emitSingleton<TemplateSignature>(emitter, templateParameter, "signature", &TemplateParameter::getSignature);
}

bool emitTemplateParameterSubstitutionScope(YAML::Emitter& emitter, TemplateParameterSubstitution& templateParameterSubstitution, EmitterData& data) {
    return emitSingleton<TemplateBinding>(emitter, templateParameterSubstitution, "templateBinding", &TemplateParameterSubstitution::getTemplateBinding);
}

bool emitTemplateSignatureScope(YAML::Emitter& emitter, TemplateSignature& signature, EmitterData& data) {
    return emitSingleton<TemplateableElement>(emitter, signature, "template", &TemplateSignature::getTemplate);
}
    
}