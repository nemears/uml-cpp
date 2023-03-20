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
        case ElementType::COMMENT : {
            Comment& comment = el.as<Comment>();
            emitScope(emitter, comment, data, emitElementScope);
            emitElementTypeAndData(emitter, comment, data, "comment", emitElementFeatures, emitCommentFeatures);
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
        // TODO
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
                        emitTypedElementFeatures,
                        emitParameterableElementFeatures);
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
    }
}

void emitClassFeatures(YAML::Emitter& emitter, Class& clazz, EmitterData& data) {
    emitOwnedSet<Classifier>(emitter, clazz, data, "nestedClassifiers", &Class::getNestedClassifiers);
    emitOwnedSet<Property>(emitter, clazz, data, "ownedAttributes", &Class::getOwnedAttributes);
    emitOwnedSet<Operation>(emitter, clazz, data, "ownedOperations", &Class::getOwnedOperations);
    emitOwnedSet<Reception>(emitter, clazz, data, "ownedReceptions", &Class::getOwnedReceptions);
}

void emitClassifierFeatures(YAML::Emitter& emitter, Classifier& classifier, EmitterData& data) {
    emitOwnedSet<Generalization>(emitter, classifier, data, "generalizations", &Classifier::getGeneralizations);
}

void emitCommentFeatures(YAML::Emitter& emitter, Comment& comment, EmitterData& data) {
    emitSet<Element>(emitter, comment, "annotatedElements", &Comment::getAnnotatedElements);
    if (!comment.getBody().empty()) {
        emitter << "body" << YAML::Value << comment.getBody();
    }
}

void emitDataTypeFeatures(YAML::Emitter& emitter, DataType& dataType, EmitterData& data) {
    emitOwnedSet<Property>(emitter, dataType, data, "ownedAttributes", &DataType::getOwnedAttributes);
    emitOwnedSet<Operation>(emitter, dataType, data, "ownedOperations", &DataType::getOwnedOperations);
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

void emitInstanceSpecificationFeatures(YAML::Emitter& emitter, InstanceSpecification& specification, EmitterData& data) {

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

void emitParameterableElementFeatures(YAML::Emitter& emitter, ParameterableElement& parameterableElement, EmitterData& data) {
    if (!parameterableElement.getOwningTemplateParameter()) {
        emitSingleton<TemplateParameter>(emitter, parameterableElement, "templateParameter", &ParameterableElement::getTemplateParameter);
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

bool emitElementScope(YAML::Emitter& emitter, Element& el, EmitterData& data) {
    return emitSingleton<Element>(emitter, el, "owner", &Element::getOwner);
}

bool emitElementImportScope(YAML::Emitter& emitter, ElementImport& elementImport, EmitterData& data) {
    return emitSingleton<Namespace>(emitter, elementImport, "importingNamespace", &ElementImport::getImportingNamespace);
}

bool emitEnumerationLiteralScope(YAML::Emitter& emitter, EnumerationLiteral& literal, EmitterData& data) {
    return emitSingleton<Enumeration>(emitter, literal, "enumeration", &EnumerationLiteral::getEnumeration);
}

bool emitNamedElementScope(YAML::Emitter& emitter, NamedElement& namedElement, EmitterData& data) {
    return emitSingleton<Namespace>(emitter, namedElement, "namespace", &NamedElement::getNamespace);
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
    }
    return false;
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