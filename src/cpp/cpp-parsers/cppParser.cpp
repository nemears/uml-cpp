#include "cpp-parsers/cppParser.h"
#include <clang-c/Index.h>

using namespace std;

namespace UML {
namespace CPP {

struct CppParserMetaData {
    UmlManager& manager;
    Element& owningElement;
    ElementType owningElementType;
};

CXChildVisitResult namespaceVisit(CXCursor c, CXCursor parent, CXClientData client_data) {
    cout << "Cpp namespace contains Cursor '" << clang_getCString(clang_getCursorSpelling(c)) << "' of kind '" << clang_getCString(clang_getCursorKindSpelling(clang_getCursorKind(c))) << endl;
    return CXChildVisit_Recurse;
}

CXChildVisitResult headerVisit(CXCursor c, CXCursor parent, CXClientData client_data) {
    CppParserMetaData* data = static_cast<CppParserMetaData*>(client_data);
    switch (clang_getCursorKind(c)) {
        case CXCursor_Namespace : {
            Package& namespacePckg = data->manager.create<Package>();
            namespacePckg.setName(clang_getCString(clang_getCursorSpelling(c)));
            // TODO apply cpp namespace stereotype?
            switch (data->owningElementType) {
                case ElementType::PACKAGE : {
                    dynamic_cast<Package&>(data->owningElement).getPackagedElements().add(namespacePckg);
                    break;
                }
                default : {
                    cerr << "No execution mapped for cpp namespace with parent type " << data->owningElement.getElementTypeString();
                }
            }
            CppParserMetaData namespaceData = {data->manager, namespacePckg, namespacePckg.getElementType()};
            clang_visitChildren(c, *namespaceVisit, &namespaceData);
            break;
        }
        default : {
            cerr << "Cursor '" << clang_getCString(clang_getCursorSpelling(c)) << "' of kind '" << clang_getCString(clang_getCursorKindSpelling(clang_getCursorKind(c))) << " does not have parsing mapped to it yet!" << endl;
        }
    }
    return CXChildVisit_Continue;
}

Package* parseHeader(string path, UmlManager& manager) {
    Package* ret = 0;
    CXIndex index = clang_createIndex(0, 0);
    const char* const clangArgs[1] = {"-xc++"};
    CXTranslationUnit unit = clang_parseTranslationUnit(index, path.c_str(), clangArgs, 1, 0, 0, CXTranslationUnit_None);
    if (unit == 0) {
        return 0;
    }

    ret = &manager.create<Package>();
    // TODO apply <<file>> stereotype?

    // get cursor
    CXCursor cursor = clang_getTranslationUnitCursor(unit);

    // set up client data
    Element* umlParent = ret;
    ElementType umlParentType = umlParent->getElementType();
    CppParserMetaData data = {manager, *umlParent, umlParentType};

    // start going through the AST
    clang_visitChildren(cursor,*headerVisit, &data);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);

    return ret;
}

}
}