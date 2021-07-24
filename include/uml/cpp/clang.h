#ifndef UML_CPP_CLANG_H
#define UML_CPP_CLANG_H

#include <clang-c/Index.h>
#include "uml/umlManager.h"

namespace UML {
    namespace CPP {

        struct CppParserMetaData {
            UmlManager& manager;
            CXTranslationUnit unit;
            Element& owningElement;
            ElementType owningElementType;
            VisibilityKind visibilty;
        };

        CXChildVisitResult headerVisit(CXCursor c, CXCursor parent, CXClientData client_data);
        CXChildVisitResult namespaceVisit(CXCursor c, CXCursor parent, CXClientData client_data);
        CXChildVisitResult classVisit(CXCursor c, CXCursor parent, CXClientData client_data);
        CXChildVisitResult arrayVisit(CXCursor c, CXCursor parent, CXClientData client_data);
        unsigned lineNumber(CXCursor c);
    }
}

#endif