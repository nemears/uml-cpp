#ifndef _UML_PARSERS_PARSER_METADATA_H_
#define _UML_PARSERS_PARSER_METADATA_H_

#include "yaml-cpp/yaml.h"
#include "uml/id.h"
#include <unordered_map>
#include <filesystem>

namespace UML{

    class AbstractManager;
    class Element;

    namespace Parsers {

        enum class ParserStrategy {
            WHOLE,
            INDIVIDUAL
        };

        class ParserMetaData {
            public:
                std::filesystem::path m_path;
                ParserMetaData(){};
                AbstractManager* m_manager = 0;
                ParserStrategy m_strategy = ParserStrategy::WHOLE;
                void* m_additionalData = 0;
        };
    }
}

#endif