#include <string>
#include <iostream>
#include <clang-c/Index.h>

class CppClassParser {
    public:
        bool parse(std::string file);
};