#ifndef EMITTER_META_DATA_H
#define EMITTER_META_DATA_H

#include <filesystem>

namespace UML {
    namespace Parsers {

        enum class EmitterStrategy {
            WHOLE,
            COMPOSITE
        };

        class EmitterMetaData {
            public:
                std::filesystem::path m_path;
                EmitterStrategy m_strategy;
                std::string m_fileName;
        };
    }
}

#endif