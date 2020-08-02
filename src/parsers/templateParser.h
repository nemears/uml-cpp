#ifndef TEMPLATEPARSER
#define TEMPLATEPARSER

#define NULL Element

#include <fstream>
#include <string>
#include "inputParser.h"
#include "../uml/element.h"
#include <iostream>
using namespace std;

class TemplateParser {

    protected:
    string keyword;
    Element element;
    virtual Element parseElement(InputParser* inputParser) {return Element();};
    virtual bool parseTag(InputParser* inputParser, Element el) {return false;};

    public:
    bool parse(InputParser* inputParser);
};
#endif