#ifndef NAMESPACE
#define NAMESPACE

#include <string>
#include <iostream>
#include "element.h"

using namespace std;

class Namespace : public Element {
    private:

    public:
        string name;
        string getName();
        void setName(const string &name);
};
#endif