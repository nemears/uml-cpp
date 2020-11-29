#include "application.h"

int main(int argc, char** argv) {
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