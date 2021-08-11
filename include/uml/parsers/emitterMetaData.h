#ifndef EMITTER_META_DATA_H
#define EMITTER_META_DATA_H

#include <filesystem>
#include "uml/id.h"

namespace UML {

    class UmlManager;

    namespace Parsers {

        enum class EmitterStrategy {
            WHOLE,
            COMPOSITE, 
            INDIVIDUAL
        };

        struct EmitterMetaData {
            public:
                std::filesystem::path m_path;
                EmitterStrategy m_strategy;
                std::string m_fileName;
                UmlManager* m_manager;
                std::filesystem::path getMountPath(ID id);
                std::filesystem::path getPath(ID id);
        };
    }
}

#endif