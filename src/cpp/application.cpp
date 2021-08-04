#include "application.h"
#include "iostream"
#include "uml/cpp/cppParser.h"

using namespace std;
using namespace UML;

int main(int argc, char** argv) {

    
    switch (argc)
    {
        case 1: {
            cout << "Printing random IDs!" << endl;
            for (char i = 0; i < 20; i++) {
                cout << ID::randomID().string() << endl;
            }
            // TODO GUI
        }
        case 2: {
            string fileName = argv[1];
            cout << "parsing file " << fileName << "\n";
            Package* header;
            UmlManager m;
            try {
                m.parse("../src/yml/uml/cpp/cppProfile.yml");
                header = CPP::parseHeader(fileName, m);
            } catch (exception& e) {
                cerr << "[FATAL ERROR] " << e.what() << endl;
            }

            // TODO print model
            // YAML::Emitter emitter;
            // try {
            //     cout << pp.emitDocument(pp.theEl) << '\n';
            // } catch (exception& e) {
            //     cerr << "[FATAL ERROR] " << e.what() << endl;
            // }
            cout << UML::Parsers::emit(*header) << endl; 

            return 0;
        }
        // case 3: {
        //     // this means that there is an extra option
        //     cout << "application received argument " << argv[1] << endl;
        //     if (strcmp(argv[1], "cppParse") == 0) {
        //         cout << "parsing cpp" << endl;
        //         CppClassParser cppClassParser;
        //         string tmp(argv[2]);
        //         cppClassParser.parse(tmp);
        //     }

        //     return 0;
        // }
        default: {
            cerr << "Improper number of arguments\n";
            return 1;
        }
    }
}