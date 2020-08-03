#ifndef NAMESPACE
#define NAMESPACE

#include <string>
#include "element.h"

using namespace std;

class Namespace : public Element {
    private:
        string name;

    public:
        string getName();
        void setName(string name);
};
#endif