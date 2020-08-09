#ifndef TEMPLATEPARSER
#define TEMPLATEPARSER

#include <fstream>
#include <string>
#include "inputParser.h"
#include "../uml/element.h"
#include <iostream>
using namespace std;

class TemplateParser {

    private:
    Element* parseElement(InputParser* inputParser);

    protected:
    string keyword;
    virtual bool parseTag(InputParser* inputParser, Element* el) = 0;
    virtual Element* createElement() = 0; // interface

    public:
    bool parse(InputParser* inputParser);
    virtual ~TemplateParser() {};
};
#endif