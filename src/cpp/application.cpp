#include "application.h"

int main(int argc, char** argv) {

    InputParser input("class.yml");
    ClassParser ppp(new map<boost::uuids::uuid, Element*>);
    input.readNextLine();
    ppp.parse(&input);
    cout << "done";
    Element* el1 = (*ppp.elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")];
    Element* el2 = (*ppp.elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")];

    switch (argc)
    {
        case 1: {
            cout << "Application running!\n";
            // TODO GUI
        }
        case 2: {
            string fileName = argv[1];
            cout << "parsing file " << fileName << "\n";
            InputParser input(fileName);
            ModelParser pp(new map<boost::uuids::uuid, Element*>);
            input.readNextLine();
            if (!pp.parse(&input)) {
                cerr << "could not parse " << fileName << "\n";
                return -1;
            }

            // TODO print model
            

            return 0;
        }
        default: {
            cerr << "Improper number of arguments\n";
            return 1;
        }
    }
}