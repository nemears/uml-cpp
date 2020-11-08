#include "application.h"

int main(int argc, char** argv) {

    Element e1;
    Element e2;
    e1.ownedElements.push_front(&e2);
    Element* e2ptr = e1.ownedElements.front();

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
            ModelParser pp;
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