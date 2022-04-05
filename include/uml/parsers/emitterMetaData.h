#ifndef _UML_PARSERS_EMITTER_META_DATA_H_
#define _UML_PARSERS_EMITTER_META_DATA_H_

#include <filesystem>
#include "uml/id.h"

namespace UML {

    class UmlManager;

    namespace Parsers {

        enum class EmitterStrategy {
            WHOLE, 
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