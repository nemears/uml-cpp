#include "uml/package.h"
#include "uml/umlManager.h"

namespace UML {
    namespace CPP {
        // WARN: must load cppProfile.yml beforehand 
        // TODO: swith to Artifact not Package when implemented
        Package* parseHeader(std::string path, UmlManager& manager);
    }
}