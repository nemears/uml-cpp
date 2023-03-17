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
void emitOwnedSet(YAML::Emitter& emitter, U& el, EmitterData& data, string key, S& (U::*signature)(), void (*emitterFunc)(YAML::Emitter&, T&, EmitterData&)) {
    if (!(el.*signature)().empty()) {
        emitter << key << YAML::Value << YAML::BeginSeq;
        if (data.mode == SerializationMode::WHOLE) {
            for (auto& child : (el.*signature)()) {
                (*emitterFunc)(emitter, child, data);
            }
        } else {
            for (ID id : (el.*signature)().ids()) {
                emitter << id.string();
            }
        }
        emitter << YAML::EndSeq;
    }
}

void emitElementData(YAML::Emitter& emitter, Element& el, EmitterData& data) {
    switch (el.getElementType()) {
        case ElementType::ABSTRACTION : {

            break;
        }
        case ElementType::DATA_TYPE : {
            emitDataType(emitter, el.as<DataType>(), data);
            break;
        }
        // TODO
        case ElementType::ENUMERATION_LITERAL : {
            emitEnumerationLiteral(emitter, el.as<EnumerationLiteral>(), data);
            break;
        }
        // TODO
        case ElementType::INSTANCE_SPECIFICATION : {
            emitInstanceSpecification(emitter, el.as<InstanceSpecification>(), data);
            break;
        }
        case ElementType::PACKAGE : {
            emitPackage(emitter, el.as<Package>(), data);
            break;
        }
    }
}

void emitDataTypeFeatures(YAML::Emitter& emitter, DataType& dataType, EmitterData& data) {
    // TODO emit owned attributes
}

void emitElementFeatures(YAML::Emitter& emitter, Element& el, EmitterData& data) {
    emitter << "id" << YAML::Value << el.getID().string();
    emitOwnedSet(emitter, el, data, "appliedStereotypes", &Element::getAppliedStereotypes, determineAndEmitInstanceSpecification);
}

void emitInstanceSpecificationFeatures(YAML::Emitter& emitter, InstanceSpecification& specification, EmitterData& data) {

}

void emitNamedElementFeatures(YAML::Emitter& emitter, NamedElement& el, EmitterData& data) {
    if (!el.getName().empty()) {
        emitter << "name" << YAML::Value << el.getName();
    }
}

void emitPackageFeatures(YAML::Emitter& emitter, Package& package, EmitterData& data) {
    emitOwnedSet(emitter, package, data, "packagedElements", &Package::getPackagedElements, determineAndEmitPackageableElements);
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

void emitDataType(YAML::Emitter& emitter, DataType& dataType, EmitterData& data) {
    emitScope(emitter, dataType, data, emitPackageableElementScope, emitElementScope);
    emitElementTypeAndData(emitter, dataType, data, "dataType",
                emitElementFeatures,
                emitNamedElementFeatures,
                emitDataTypeFeatures);
}

void emitEnumerationLiteral(YAML::Emitter& emitter, EnumerationLiteral& literal, EmitterData& data) {
    emitScope(emitter, literal, data, emitEnumerationLiteralScope, emitPackageableElementScope, emitElementScope);
    emitElementTypeAndData(emitter, literal, data, "enumerationLiteral", 
                emitElementFeatures, 
                emitNamedElementFeatures, 
                emitInstanceSpecificationFeatures);
}

void emitInstanceSpecification(YAML::Emitter& emitter, InstanceSpecification& inst, EmitterData& data) {
    emitScope(emitter, inst, data, emitPackageableElementScope, emitElementScope);
    emitElementTypeAndData(emitter, inst, data, "instanceSpecification", 
                emitElementFeatures, 
                emitNamedElementFeatures, 
                emitInstanceSpecificationFeatures);
}

void emitPackage(YAML::Emitter& emitter, Package& package, EmitterData& data) {
    emitScope(emitter, package, data, emitPackageableElementScope, emitElementScope);
    emitElementTypeAndData(emitter, package, data, "package",
                emitElementFeatures,
                emitNamedElementFeatures,
                emitPackageFeatures);
}

void determineAndEmitInstanceSpecification(YAML::Emitter& emitter, InstanceSpecification& inst, EmitterData& data) {
    switch (inst.getElementType()) {
        case ElementType::ENUMERATION_LITERAL : {
            emitEnumerationLiteral(emitter, inst.as<EnumerationLiteral>(), data);
            break;
        }
        case ElementType::INSTANCE_SPECIFICATION : {
            emitInstanceSpecification(emitter, inst, data);
            break;
        }
        default : {
            throw SerializationError("cannot serialize instance specification, element type cannot be used: " + Element::elementTypeToString(inst.getElementType()));
        }
    }
}

void determineAndEmitPackageableElements(YAML::Emitter& emitter, PackageableElement& el, EmitterData& data) {
    switch (el.getElementType()) {
        case ElementType::ENUMERATION_LITERAL : {
            emitEnumerationLiteral(emitter, el.as<EnumerationLiteral>(), data);
            break;
        }
        case ElementType::INSTANCE_SPECIFICATION : {
            emitInstanceSpecification(emitter, el.as<InstanceSpecification>(), data);
            break;
        }
        case ElementType::PACKAGE : {
            emitPackage(emitter, el.as<Package>(), data);
            break;
        }
        default : {
            throw SerializationError("cannot serialize packageableElement, element type cannot be used: " + Element::elementTypeToString(el.getElementType()));
        }
    }
}
    
}