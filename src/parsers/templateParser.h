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

    private:
    Element parseElement(InputParser* inputParser);

    protected:
    string keyword;
    Element element;
    virtual bool parseTag(InputParser* inputParser, Element el) {return false;};
    virtual Element createElement() = 0; // interface

    public:
    bool parse(InputParser* inputParser);
};
#endif