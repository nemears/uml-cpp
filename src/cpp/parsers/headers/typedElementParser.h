#ifndef TYPEDELEMENTPARSER
#define TYPEDELEMENTPARSER
#include "namedElementParser.h"
#include "../../uml/headers/typedElement.h"

class TypedElementParser : public NamedElementParser {
    protected:
        bool parseTag(InputParser* inputParser, Element * el);
};
#endif