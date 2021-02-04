#include <string>
#include <iostream>
#include <clang-c/Index.h>

using namespace std;

class CppClassParser {
    public:
        bool parse(string file);
};