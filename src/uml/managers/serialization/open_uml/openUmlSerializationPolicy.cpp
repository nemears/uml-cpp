#include "uml/managers/serialization/open_uml/openUmlSerializationPolicy.h"
#include "uml/managers/serialization/open_uml/emit.h"
#include "uml/managers/serialization/open_uml/parse.h"
#include "yaml-cpp/yaml.h"

using namespace std;

namespace UML {

std::string OpenUmlSerializationPolicy::extensionName() {
    return ".yml"; // ".uml.yml"
}

ElementPtr OpenUmlSerializationPolicy::parseIndividual(std::string data, AbstractManager& manager) {
    ParserData metaData;
    metaData.manager = &manager;
    return parse(data, metaData);
}

ElementPtr OpenUmlSerializationPolicy::parseWhole(std::string data, AbstractManager& manager) {
    ParserData metaData;
    metaData.mode = SerializationMode::WHOLE;
    metaData.manager = &manager;
    return parse(data, metaData);
}

std::string OpenUmlSerializationPolicy::emitIndividual(Element& el, AbstractManager& manager) {
    EmitterData data;
    data.emitReferences = true;
    return emit(el, data);
}

std::string OpenUmlSerializationPolicy::emitWhole(Element& el, AbstractManager& manager) {
    EmitterData data;
    data.mode = SerializationMode::WHOLE;
    return emit(el, data);
}

std::string OpenUmlJsonSerializationPolicy::extensionName() {
    return ".json"; // ".uml.json"
}

ElementPtr OpenUmlJsonSerializationPolicy::parseIndividual(std::string data, AbstractManager& manager) {
    ParserData metaData;
    metaData.manager = &manager;
    ElementPtr ret = parse(data, metaData);

    return ret;
}

ElementPtr OpenUmlJsonSerializationPolicy::parseWhole(std::string data, AbstractManager& manager) {
    ParserData metaData;
    metaData.mode = SerializationMode::WHOLE;
    metaData.manager = &manager;
    return parse(data, metaData);
}

std::string OpenUmlJsonSerializationPolicy::emitIndividual(Element& el, AbstractManager& manager) {
    EmitterData data;
    data.isJSON = true;
    return emit(el, data);
}

std::string OpenUmlJsonSerializationPolicy::emitWhole(Element& el, AbstractManager& manager) {
    EmitterData data;
    data.mode = SerializationMode::WHOLE;
    data.isJSON = true;
    return emit(el, data);
}

std::string emit(Element& el, EmitterData& data) {
    YAML::Emitter emitter;
    if (data.isJSON) {
        emitter << YAML::DoubleQuoted << YAML::Flow;
        if (data.emitReferences) {
            emitter << YAML::BeginMap;
        }
    } else if (data.emitReferences) {
        emitter << YAML::BeginDoc;
    }
    emitter << YAML::BeginMap;
    emitElementData(emitter, el, data);
    emitter << YAML::EndMap;
    if (data.emitReferences) {
        if (data.isJSON) {
            emitter << YAML::EndMap << YAML::BeginMap;
        } else {
            emitter << YAML::EndDoc << YAML::BeginDoc;
        }
        emitter << YAML::BeginSeq;
        for (auto reference : el.m_node->m_references) {
            emitter << reference.first.string();
        }
        emitter << YAML::EndSeq;
        if (data.isJSON) {
            emitter << YAML::EndMap;
        } else {
            emitter << YAML::EndDoc;
        }
    }
    return emitter.c_str();
}

ElementPtr parse(std::string data, ParserData& metaData) {
    std::vector<YAML::Node> rootNodes = YAML::LoadAll(data);
    ElementPtr ret;
    if (rootNodes.empty()) {
        throw SerializationError("could not parse data supplied to manager! Is it JSON or YAML?");
    }
    if (metaData.mode == SerializationMode::INDIVIDUAL) {
        ret = parseNode(rootNodes[0], metaData);
        // TODO parseScope
        if (rootNodes.size() == 2) {
            YAML::Node references = rootNodes[1];
            if (!references.IsSequence()) {
                throw SerializationError("reference list is not a list!" + getLineNumber(references));
            }
            for (size_t i = 0; i < references.size(); i++) {
                ID referenceID = ID::fromString(references[i].as<string>());
                if (ret->m_node->m_references.find(referenceID) == ret->m_node->m_references.end()) {
                    ret->m_node->setReference(referenceID);
                }



                // if (!metaData.manager->loaded(referenceID)) {
                // //     ElementPtr referencedEl = metaData.manager->get(referenceID);
                // //     ret->m_node->setReference(*referencedEl);
                // //     referencedEl->m_node->setReference(*ret);
                // // } else {
                //     ret->m_node->setReference(referenceID);
                // }
            }
        }
    } else {
        ret = parseNode(rootNodes[0], metaData);
    }
    return ret;
}

}