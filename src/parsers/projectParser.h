#ifndef PROJECTPARSER
#define PROJECTPARSER
#include "templateParser.h"
#include "../uml/project.h"
using namespace std;

class ProjectParser : public TemplateParser {
    protected:
    Element parseElement(InputParser* inputParser);
    bool parseTag(InputParser* inputParser, Project el);

    public:
    ProjectParser() {
        keyword = "project";
    }
};
#endif