#include "application.h"

int main(int argc, char** argv) {

    
    switch (argc)
    {
        case 1: {
            cout << "Application running!\n";
            // TODO GUI
        }
        case 2: {
            YAML::Node node = YAML::LoadFile(argv[1]);
            string fileName = argv[1];
            cout << "parsing file " << fileName << "\n";
            ModelParser pp(new map<boost::uuids::uuid, UML::Element*>);
            try {
                if(!pp.parse(node)) {
                    cerr << "could not parse " << fileName << endl;
                    return -1;
                }
            } catch (exception& e) {
                cerr << e.what() << endl;
            }

            // TODO print model
            YAML::Emitter emitter;
            try {
                cout << pp.emitDocument(pp.theEl) << '\n';
            } catch (exception& e) {
                cerr << e.what() << endl;
            }

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