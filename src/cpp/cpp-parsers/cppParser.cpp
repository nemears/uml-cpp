#include "cpp-parsers/cppParser.h"
#include <clang-c/Index.h>

using namespace std;

namespace UML {
namespace CPP {

Package* parseHeader(string path, UmlManager& manager) {
    Package* ret = 0;
    CXIndex index = clang_createIndex(0, 0);
    const char* const clangArgs[1] = {"-xc++"};
    CXTranslationUnit unit = clang_parseTranslationUnit(index, path.c_str(), clangArgs, 1, nullptr, 0, CXTranslationUnit_None);
    if (unit == 0) {
        return 0;
    }

    ret = &manager.create<Package>();
    // TODO apply <<file>> stereotype?

    CXCursor cursor = clang_getTranslationUnitCursor(unit);
    // cout << clang_getCursorKind(cursor) << std::endl;

    clang_visitChildren(cursor,
    [](CXCursor c, CXCursor parent, CXClientData client_data)
    {
        cout << "Cursor '" << clang_getCString(clang_getCursorSpelling(c)) << "' of kind '" << clang_getCString(clang_getCursorKindSpelling(clang_getCursorKind(c))) << endl;
        return CXChildVisit_Recurse;
    },
    nullptr);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);

    return ret;
}

}
}