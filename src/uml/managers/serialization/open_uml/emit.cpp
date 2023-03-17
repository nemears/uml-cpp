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

template<class T, class U, class S>
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

template <class T, class U>
void emitSingleton(YAML::Emitter& emitter, U& el, string key, UmlPtr<T> (U::*acessor)() const) {
    if ((el.*acessor)()) {
        emitter << key << YAML::Value << (el.*acessor)().id().string();
    }
}

void emitElementData(YAML::Emitter& emitter, Element& el, EmitterData& data) {
    switch (el.getElementType()) {
        case ElementType::ABSTRACTION : {

            break;
        }
        case ElementType::DATA_TYPE : {
            DataType& dataType = el.as<DataType>();
            emitScope(emitter, dataType, data, emitPackageableElementScope, emitElementScope);
            emitElementTypeAndData(emitter, dataType, data, "dataType",
                emitElementFeatures,
                emitNamedElementFeatures,
                emitDataTypeFeatures);
            break;
        }
        // TODO
        case ElementType::ENUMERATION_LITERAL : {
            EnumerationLiteral& literal = el.as<EnumerationLiteral>();
            emitScope(emitter, literal, data, emitEnumerationLiteralScope, emitPackageableElementScope, emitElementScope);
            emitElementTypeAndData(emitter, literal, data, "enumerationLiteral", 
                        emitElementFeatures, 
                        emitNamedElementFeatures, 
                        emitInstanceSpecificationFeatures);
            break;
        }
        // TODO
        case ElementType::INSTANCE_SPECIFICATION : {
            InstanceSpecification& inst = el.as<InstanceSpecification>();
            emitScope(emitter, inst, data, emitPackageableElementScope, emitElementScope);
            emitElementTypeAndData(emitter, inst, data, "instanceSpecification", 
                        emitElementFeatures, 
                        emitNamedElementFeatures, 
                        emitInstanceSpecificationFeatures);
            break;
        }
        case ElementType::PACKAGE : {
            Package& package = el.as<Package>();
            emitScope(emitter, package, data, emitPackageableElementScope, emitElementScope);
            emitElementTypeAndData(emitter, package, data, "package",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitPackageFeatures);
            break;
        }
        case ElementType::PROPERTY : {
            Property& property = el.as<Property>();
            emitScope(emitter, property, data, emitPropertyScope, emitElementScope);
            emitElementTypeAndData(emitter, property, data, "property",
                        emitElementFeatures,
                        emitNamedElementFeatures,
                        emitTypedElementFeatures);
            break;
        }
    }
}

void emitDataTypeFeatures(YAML::Emitter& emitter, DataType& dataType, EmitterData& data) {
    emitOwnedSet<Property>(emitter, dataType, data, "ownedAttributes", &DataType::getOwnedAttributes);
}

void emitElementFeatures(YAML::Emitter& emitter, Element& el, EmitterData& data) {
    emitter << "id" << YAML::Value << el.getID().string();
    emitOwnedSet<InstanceSpecification>(emitter, el, data, "appliedStereotypes", &Element::getAppliedStereotypes);
}

void emitInstanceSpecificationFeatures(YAML::Emitter& emitter, InstanceSpecification& specification, EmitterData& data) {

}

void emitNamedElementFeatures(YAML::Emitter& emitter, NamedElement& el, EmitterData& data) {
    if (!el.getName().empty()) {
        emitter << "name" << YAML::Value << el.getName();
    }
}

void emitPackageFeatures(YAML::Emitter& emitter, Package& package, EmitterData& data) {
    emitOwnedSet<PackageableElement>(emitter, package, data, "packagedElements", &Package::getPackagedElements);
}

void emitTypedElementFeatures(YAML::Emitter& emitter, TypedElement& typedElement, EmitterData& data) {
    emitSingleton<Type>(emitter, typedElement, "type", &TypedElement::getType);
}

bool emitElementScope(YAML::Emitter& emitter, Element& el, EmitterData& data) {
    if (el.getOwner()) {
        emitter << "owner" << YAML::Value << el.getOwner().id().string();
        return true;
    }
    return false;
}

bool emitEnumerationLiteralScope(YAML::Emitter& emitter, EnumerationLiteral& literal, EmitterData& data) {
    if (literal.getEnumeration()) {
        emitter << "enumeration" << YAML::Value << literal.getEnumeration().id().string();
        return true;
    }
    return false;
}

bool emitPackageableElementScope(YAML::Emitter& emitter, PackageableElement& el, EmitterData& data) {
    if (el.getOwningPackage()) {
        emitter << "owningPackage" << YAML::Value << el.getOwningPackage().id().string();
        return true;
    }
    return false;
}

bool emitPropertyScope(YAML::Emitter& emitter, Property& property, EmitterData& data) {
    if (property.getClass()) {
        emitter << "class" << YAML::Value << property.getClass().id().string();
        return true;
    } else if (property.getDataType()) {
        emitter << "dataType" << YAML::Value << property.getDataType().id().string();
        return true;
    } else if (property.getInterface()) {
        emitter << "interface" << YAML::Value << property.getInterface().id().string();
        return true;
    }
    return false;
}
    
}