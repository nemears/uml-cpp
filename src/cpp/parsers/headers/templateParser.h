#ifndef TEMPLATEPARSER
#define TEMPLATEPARSER
#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <boost/uuid/uuid.hpp>
#include "yaml-cpp/yaml.h"
#include "../../uml/headers/element.h"
using namespace std;

class TemplateParser {
    protected:
        virtual UML::Element* createElement() = 0; // interface
        virtual bool parseFeatures(YAML::Node node, UML::Element* el) = 0;

    public:
        string keyword;
        map<boost::uuids::uuid, UML::Element*>* elements;
        bool parse(YAML::Node node);

        virtual ~TemplateParser() {
            //delete elements;
        };

        UML::Element* parseElement(YAML::Node node);
        
        /**
         * Constructor
         * @param std::map<boost::uuids:uuid, Element*> elements is a map of key :id, value : element 
         *      containing all elements in the parsed model
         */
        TemplateParser(map<boost::uuids::uuid, UML::Element*>* elements) {
            this->elements = elements;
        };
};
#endif