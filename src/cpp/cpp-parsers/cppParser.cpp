#include "cpp-parsers/cppParser.h"
#include <clang-c/Index.h>
#include "uml/class.h"
#include "uml/operation.h"

using namespace std;

namespace UML {
namespace CPP {

struct CppParserMetaData {
    UmlManager& manager;
    Element& owningElement;
    ElementType owningElementType;
    VisibilityKind visibilty;
};

void setOwnerHelper(Element& ownee, Element& owner) {
    switch (owner.getElementType()) {
        case ElementType::PACKAGE : {
            dynamic_cast<Package&>(owner).getPackagedElements().add(dynamic_cast<PackageableElement&>(ownee));
            break;
        }
        case ElementType::CLASS : {
            switch(ownee.getElementType()) {
                case ElementType::OPERATION : {
                    dynamic_cast<Class&>(owner).getOperations().add(dynamic_cast<Operation&>(ownee));
                    break;
                }
            }
            break;
        }
        default : {
            cerr << "No execution mapped for cpp namespace with parent type " << owner.getElementTypeString();
            // TODO throw error
        }
    }
}

CXChildVisitResult classVisit(CXCursor c, CXCursor parent, CXClientData client_data) { 
    CppParserMetaData& data = *static_cast<CppParserMetaData*>(client_data); 
    switch (clang_getCursorKind(c)) {
        case CXCursor_CXXAccessSpecifier : {
            switch (clang_getCXXAccessSpecifier(c)) {
                case CX_CXXPrivate : {
                    data.visibilty = VisibilityKind::PRIVATE;
                    break;
                }
                case CX_CXXProtected : {
                    data.visibilty = VisibilityKind::PROTECTED;
                    break;
                }
                case CX_CXXPublic : {
                    data.visibilty = VisibilityKind::PUBLIC;
                    break;
                }
                case CX_CXXInvalidAccessSpecifier :
                default : {
                    cerr << "Cpp class contains access specifier with unset mappings" << endl;
                }
            }
            break;
        }
        case CXCursor_Constructor : {
            Operation& constructor = data.manager.create<Operation>();
            constructor.setName(clang_getCString(clang_getCursorSpelling(c)));
            setOwnerHelper(constructor, data.owningElement);
            break;
        }
        default : {
            cerr << "Cpp class contains Cursor '" << clang_getCString(clang_getCursorSpelling(c)) << "' of kind '" << clang_getCString(clang_getCursorKindSpelling(clang_getCursorKind(c))) << "', but no mapping is set" << endl;
            return CXChildVisit_Recurse;
        }
    }
    return CXChildVisit_Continue;
}

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
            setOwnerHelper(namespacePckg, data->owningElement);
            CppParserMetaData namespaceData = {data->manager, namespacePckg, namespacePckg.getElementType()};
            clang_visitChildren(c, *namespaceVisit, &namespaceData);
            break;
        }
        case CXCursor_ClassDecl : {
            Class& cppClass = data->manager.create<Class>();
            cppClass.setName(clang_getCString(clang_getCursorSpelling(c)));
            setOwnerHelper(cppClass, data->owningElement);
            CppParserMetaData classData = {data->manager, cppClass, cppClass.getElementType()};
            clang_visitChildren(c, *classVisit, &classData);
            break;
        }
        default : {
            cerr << "Cursor '" << clang_getCString(clang_getCursorSpelling(c)) << "' of kind '" << clang_getCString(clang_getCursorKindSpelling(clang_getCursorKind(c))) << "' does not have parsing mapped to it yet!" << endl;
            // TODO throw error
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