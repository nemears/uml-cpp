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
        virtual Element* createElement() = 0; // interface
        virtual bool parseFeatures(YAML::Node node, Element* el) = 0;

    public:
        string keyword;
        map<boost::uuids::uuid, Element*>* elements;
        bool parse(YAML::Node node);

        virtual ~TemplateParser() {
            //delete elements;
        };
        
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