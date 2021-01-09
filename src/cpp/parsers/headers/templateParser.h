#ifndef TEMPLATEPARSER
#define TEMPLATEPARSER
#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <boost/uuid/uuid.hpp>
#include "yaml-cpp/yaml.h"
#include "inputParser.h"
#include "../../uml/headers/element.h"
using namespace std;

class TemplateParser {
    protected:
        virtual bool parseTag(InputParser* inputParser, Element* el) = 0;
        virtual Element* createElement() = 0; // interface

        // new function interface for yaml-cpp parser to get details of uml element type from file
        virtual bool parseFeatures(YAML::Node node, Element* el) = 0;

    public:
        string keyword;
        map<boost::uuids::uuid, Element*>* elements;

        // old top level parse function
        bool parse(InputParser* inputParser);

        // new top level parse function
        bool parse(YAML::Node node);

        virtual ~TemplateParser() {
            //delete elements;
        };

        // Old function using inputParser parser
        Element* parseElement(InputParser* inputParser);

        // New function using yaml-cpp parser
        Element* parseElement(YAML::Node node);
        
        /**
         * Constructor
         * @param std::map<boost::uuids:uuid, Element*> elements is a map of key :id, value : element 
         *      containing all elements in the parsed model
         */
        TemplateParser(map<boost::uuids::uuid, Element*>* elements) {
            this->elements = elements;
        };
};
#endif