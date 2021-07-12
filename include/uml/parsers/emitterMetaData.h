#ifndef EMITTER_META_DATA_H
#define EMITTER_META_DATA_H

#include <filesystem>

namespace UML {
    namespace Parsers {

        enum class EmitterStrategy {
            WHOLE,
            COMPOSITE
        };

        struct EmitterMetaData {
            public:
                std::filesystem::path m_path;
                EmitterStrategy m_strategy;
                std::string m_fileName;
                UmlManager* m_manager;
        };
    }
}

#endif