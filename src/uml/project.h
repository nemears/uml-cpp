#ifndef PROJECT
#define PROJECT

#include "element.h"
#include <string>
using namespace std;

class Project : public Element {
    private:
    string name;

    public:
    string getName(){return name;};
    void setName(string name){this->name = name;};
};
#endif