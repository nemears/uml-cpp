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

void emitElementBody(YAML::Emitter& emitter, Element& el, EmitterData& data) {
    switch (el.getElementType()) {
        case ElementType::ABSTRACTION : {

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
    }
}

void emitElement(YAML::Emitter& emitter, Element& el, EmitterData& data) {
    emitter << "id" << YAML::Value << el.getID().string();
    emitOwnedSet(emitter, el, data, "appliedStereotypes", &Element::getAppliedStereotypes, determineAndEmitInstanceSpecification);
}

void emitInstanceSpecificationFeatures(YAML::Emitter& emitter, InstanceSpecification& specification, EmitterData& data) {

}

void emitNamedElement(YAML::Emitter& emitter, NamedElement& el, EmitterData& data) {
    if (!el.getName().empty()) {
        emitter << "name" << YAML::Value << el.getName();
    }
}

void emitEnumerationLiteral(YAML::Emitter& emitter, EnumerationLiteral& literal, EmitterData& data) {
    emitElementTypeAndData(emitter, literal, data, "enumerationLiteral", 
                emitElement, 
                emitNamedElement, 
                emitInstanceSpecificationFeatures);
}

void emitInstanceSpecification(YAML::Emitter& emitter, InstanceSpecification& inst, EmitterData& data) {
    emitElementTypeAndData(emitter, inst, data, "instanceSpecification", 
                emitElement, 
                emitNamedElement, 
                emitInstanceSpecificationFeatures);
}

void determineAndEmitInstanceSpecification(YAML::Emitter& emitter, InstanceSpecification& inst, EmitterData& data) {
    switch (inst.getElementType()) {
        case ElementType::ENUMERATION_LITERAL : {
            return emitEnumerationLiteral(emitter, inst.as<EnumerationLiteral>(), data);
        }
        case ElementType::INSTANCE_SPECIFICATION : {
            return emitInstanceSpecification(emitter, inst, data);
        }
        default : {
            throw SerializationError("cannot serialize instance specification, element type cannot be used: " + Element::elementTypeToString(inst.getElementType()));
        }
    }
}
    
}