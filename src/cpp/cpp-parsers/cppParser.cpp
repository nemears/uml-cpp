#include "cpp-parsers/cppParser.h"
#include <clang-c/Index.h>
#include "uml/class.h"
#include "uml/operation.h"
#include "uml/instanceSpecification.h"
#include "uml/stereotype.h"
#include "uml/literalInt.h"
#include "uml/literalReal.h"

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
    CppParserMetaData* data = static_cast<CppParserMetaData*>(client_data);
    switch (clang_getCursorKind(c)) {
        case CXCursor_VarDecl : {
            CXType type = clang_getCursorType(c);
            switch (type.kind) {
                case CXTypeKind::CXType_Char_S : {
                    LiteralInt& cChar = data->manager.create<LiteralInt>();
                    cChar.setName(clang_getCString(clang_getCursorSpelling(c)));
                    cChar.setType(&data->manager.get<PrimitiveType>(ID::fromString("C_char_bvN6xdQ&&LaR7MU_F_9uR")));
                    if (data->owningElement.getElementType() == ElementType::PACKAGE) {
                        dynamic_cast<Package&>(data->owningElement).getPackagedElements().add(cChar);
                    }
                    break;
                }
                case CXTypeKind::CXType_Int : {
                    LiteralInt& cInt = data->manager.create<LiteralInt>();
                    cInt.setName(clang_getCString(clang_getCursorSpelling(c)));
                    cInt.setType(&data->manager.get<PrimitiveType>(ID::fromString("C_int_ZvgWKuxGtKtjRQPMNTXjic")));
                    if (data->owningElement.getElementType() == ElementType::PACKAGE) {
                        dynamic_cast<Package&>(data->owningElement).getPackagedElements().add(cInt);
                    }
                    break;
                }
                case CXTypeKind::CXType_Float : {
                    LiteralReal& cFloat = data->manager.create<LiteralReal>();
                    cFloat.setName(clang_getCString(clang_getCursorSpelling(c)));
                    cFloat.setType(&data->manager.get<PrimitiveType>(ID::fromString("C_float_FRQyo8d1KEQQLOnnPPn6")));
                    if (data->owningElementType == ElementType::PACKAGE) {
                        dynamic_cast<Package&>(data->owningElement).getPackagedElements().add(cFloat);
                    }
                    break;
                }
                case CXTypeKind::CXType_Double : {
                    LiteralReal& cDouble = data->manager.create<LiteralReal>();
                    cDouble.setName(clang_getCString(clang_getCursorSpelling(c)));
                    cDouble.setType(&data->manager.get<PrimitiveType>(ID::fromString("C_double_HM2asoTiFmoWEK8ZuAE")));
                    if (data->owningElementType == ElementType::PACKAGE) {
                        dynamic_cast<Package&>(data->owningElement).getPackagedElements().add(cDouble);
                    }
                    break;
                }
                default : {
                    cerr << "Unahandled type for variable type " << clang_getCString(clang_getTypeSpelling(clang_getCursorType(c))) << endl;
                    break;
                }
            }
            break;
        }
        default : {
            cerr << "Cpp namespace contains Cursor '" << clang_getCString(clang_getCursorSpelling(c)) << "' of kind '" << clang_getCString(clang_getCursorKindSpelling(clang_getCursorKind(c))) << ", but no mapping has been defined yet!" << endl;
            break;
        }
    }
    return CXChildVisit_Recurse;
}

CXChildVisitResult headerVisit(CXCursor c, CXCursor parent, CXClientData client_data) {
    CppParserMetaData* data = static_cast<CppParserMetaData*>(client_data);
    if (data->manager.count(ID::fromString("CPP_4zpFq9s6YilFjqZAPguiluqk")) == 0) {
        // TODO throw error
        cerr << "ERROR: Did not load C++ Profile before parsing header!" << endl;
    }
    switch (clang_getCursorKind(c)) {
        case CXCursor_Namespace : {
            Package& namespacePckg = data->manager.create<Package>();
            namespacePckg.setName(clang_getCString(clang_getCursorSpelling(c)));
            InstanceSpecification& stereotypeInst = data->manager.create<InstanceSpecification>();
            stereotypeInst.setClassifier(&data->manager.get<Stereotype>(ID::fromString("Cpp_NAMESPACE_3FloKgLhiH2P0t")));
            namespacePckg.getAppliedStereotypes().add(stereotypeInst);
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