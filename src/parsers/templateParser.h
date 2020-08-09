#ifndef TEMPLATEPARSER
#define TEMPLATEPARSER
#include <fstream>
#include <string>
#include "inputParser.h"
#include "../uml/element.h"
#include <iostream>
using namespace std;

class TemplateParser {
    protected:
        virtual bool parseTag(InputParser* inputParser, Element* el) = 0;
        virtual Element* createElement() = 0; // interface

    public:
        string keyword;
        bool parse(InputParser* inputParser);
        virtual ~TemplateParser() {};
        Element* parseElement(InputParser* inputParser);
};
#endif