#ifndef PARSER_METADATA_H
#define PARSER_METADATA_H

#include "yaml-cpp/yaml.h"
#include "uml/id.h"
#include <unordered_map>
#include <filesystem>

namespace UML{

    class UmlManager;
    class Element;

    namespace Parsers {

        enum class ParserStrategy {
            WHOLE,
            INDIVIDUAL
        };

        class ParserMetaData {
            public:
                std::filesystem::path m_path;
                ParserMetaData(UmlManager* manager);
                UmlManager* m_manager;
                ParserStrategy m_strategy = ParserStrategy::WHOLE;
        };
    }
}

#endif