#ifndef TEMPLATEPARSER
#define TEMPLATEPARSER
#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <boost/uuid/uuid.hpp>
#include "inputParser.h"
#include "../../uml/headers/element.h"
using namespace std;

class TemplateParser {
    protected:
        virtual bool parseTag(InputParser* inputParser, Element* el) = 0;
        virtual Element* createElement() = 0; // interface

    public:
        string keyword;
        map<boost::uuids::uuid, Element*>* elements;

        bool parse(InputParser* inputParser);
        virtual ~TemplateParser() {
            delete elements;
        };
        Element* parseElement(InputParser* inputParser);
        
        // TODO add map pointer of all elements to constructor of each parser to be shared
        TemplateParser(map<boost::uuids::uuid, Element*>* elements) {
            this->elements = elements;
        };
};
#endif