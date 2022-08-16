#ifndef _UML_PARSERS_PARSER_METADATA_H_
#define _UML_PARSERS_PARSER_METADATA_H_

#include "yaml-cpp/yaml.h"
#include "uml/id.h"
#include <unordered_map>
#include <filesystem>

namespace UML{

    class UmlManager;
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
                ParserMetaData(UmlManager* manager); // delete
                ParserMetaData(){};
                UmlManager* m_manager = 0; // delete
                AbstractManager* m_manager2 = 0;
                ParserStrategy m_strategy = ParserStrategy::WHOLE;
                void* m_additionalData = 0;
        };
    }
}

#endif