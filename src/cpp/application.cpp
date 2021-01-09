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
            // if (node.IsMap()) {
            //     for(YAML::const_iterator it=node.begin();it!=node.end();++it) {
            //         cout << it->first.as<string>() << " : " << it->second.as<string>() << '\n';
            //     }
            // }
            YamlCppParser yp(argv[1]);
            yp.readNextLine();

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