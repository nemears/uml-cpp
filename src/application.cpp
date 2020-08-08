#include "application.h"

using namespace std;

int main(int argc, char** argv) {
    string fileName = "tiny.yml";
    Model damn;
    damn.setName("damn");
    cout << damn.getName() << '\n';
    switch (argc)
    {
        case 1: {
            cout << "Application running!\n";
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
            return 0;
        }
        default: {
            cerr << "Improper number of arguments\n";
            return 1;
        }
    }
}