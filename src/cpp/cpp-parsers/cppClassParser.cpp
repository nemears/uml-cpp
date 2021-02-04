#include "cpp-parsers/cppClassParser.h"

ostream& operator<<(ostream& stream, const CXString& str)
{
  stream << clang_getCString(str);
  clang_disposeString(str);
  return stream;
}

bool CppClassParser::parse(string file) {

    // load parser
    CXIndex index = clang_createIndex(0, 0);
    CXTranslationUnit unit = clang_parseTranslationUnit(
        index,
        file.c_str(), nullptr, 0,
        nullptr, 0,
    CXTranslationUnit_None);

    // check
    if (unit == nullptr){
        cerr << "Unable to parse translation unit. Quitting." << endl;
        return false;
    }

    CXCursor cursor = clang_getTranslationUnitCursor(unit);
    clang_visitChildren(
        cursor,
        [](CXCursor c, CXCursor parent, CXClientData client_data)
        {
            cout << "Cursor '" << clang_getCursorSpelling(c) << "' of kind '"
                << clang_getCursorKindSpelling(clang_getCursorKind(c)) << "'\n";
            return CXChildVisit_Recurse;
        },
        nullptr);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);

    return true;
}