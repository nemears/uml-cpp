#ifndef TEMPLATEPARSER
#define TEMPLATEPARSER
#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <boost/uuid/uuid.hpp>
#include "yaml-cpp/yaml.h"
#include "uml/element.h"

using namespace std;
using namespace UML;

typedef struct {
    boost::uuids::uuid otherEl;
    list<void (*)(Element*, Element*)> applyOnEl;
} PostParser;

/**
 * This class sets up the variables and methods for all of the yuml-parsers
 * based off of yml configuration files
 **/

class TemplateParser {
    protected:

        /**
         * Creates the element of scope to this parser if the parser reads that it needs to create an element
         * @return the Element type corrsponding to this parser
         **/
        virtual Element* createElement() = 0; // interface

        /**
         * Parses features relevant to the uml feature in the yaml config file
         * @param node  - the Yaml node for parsing
         * @param el - the uml element of current scope to fill out from the config file
         * @return flag whether a fatal error was found
         **/
        virtual bool parseFeatures(YAML::Node node, Element* el) = 0;

    public:
        /**
         * The keyword to parse for relevant to this parser note: will only appear if it can be defined,
         * e.g. Classifier is never a valid keyword
         **/
        string keyword;

        /**
         * The map of elements parsed within the document so far, key is uuid of element, value is the element of interest
         **/
        map<boost::uuids::uuid, Element*>* elements;

        /**
         * This is a map that just holds postParsing info to be called on element after instantiation
         **/
        map<boost::uuids::uuid, PostParser*>* postProcessFlag;

        /**
         * The public method to begin parsing a document. The node should be the root node of the document,
         * it can work intermediately however
         * @param node - the yaml-cpp node of the document to be parsed
         * @return whether a fatal error occured
         **/
        bool parse(YAML::Node node);

        /**
         * Destructor, unused for now
         */
        virtual ~TemplateParser() {
            //delete elements;
        };

        /**
         * method that parses current node and returns the uml Element corresponding to that node parsed
         * @param node - the yaml-cpp node to parse
         * @return the element parsed
         **/
        Element* parseElement(YAML::Node node);
        
        /**
         * Constructor
         * @param std::map<boost::uuids:uuid, Element*> elements is a map of key :id, value : element 
         *      containing all elements in the parsed model
         */
        TemplateParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) {
            this->elements = elements;
            this->postProcessFlag = postParsers;
        };

        virtual bool emit(YAML::Emitter& emitter, Element* el) = 0;

        string emitDocument(Element* rootEl);

        Element* theEl;
};
#endif