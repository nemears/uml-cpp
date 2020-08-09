#ifndef MODELPARSER
#define MODELPARSER
#include "templateParser.h"
#include "namespaceParser.h"
#include "../uml/model.h"
using namespace std;

class ModelParser : public NamespaceParser {
    protected:
        Element* createElement();
        bool parseTag(InputParser* input, Element * el);

    public:
        ModelParser() {
            this->keyword = "model";
        }
};
#endif