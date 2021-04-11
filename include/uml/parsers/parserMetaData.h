#ifndef PARSER_METADATA_H
#define PARSER_METADATA_H

#include "uml/sequence.h"

namespace UML{
    namespace Parsers {
        class ParserMetaData {
            public:
                Sequence<> elements;
        };
    }
}

#endif