#include "uml/cpp/cppParser.h"
#include "uml/cpp/clang.h"
#include "uml/class.h"
#include "uml/operation.h"
#include "uml/instanceSpecification.h"
#include "uml/stereotype.h"
#include "uml/literalInt.h"
#include "uml/literalReal.h"
#include "uml/primitiveType.h"
#include "uml/property.h"
#include "uml/parameter.h"

using namespace std;

namespace UML {
namespace CPP {

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
        case CXCursor_FieldDecl : {
            CXType type = clang_getCursorType(c);
            switch (type.kind) {
                case CXTypeKind::CXType_Char_S : {
                    Property& charProp = data.manager.create<Property>();
                    charProp.setName(clang_getCString(clang_getCursorSpelling(c)));
                    charProp.setType(&data.manager.get<PrimitiveType>(ID::fromString("C_char_bvN6xdQ&&LaR7MU_F_9uR")));
                    charProp.setVisibility(data.visibilty);
                    switch (data.owningElementType) {
                        case ElementType::CLASS : {
                            data.owningElement.as<Class>().getOwnedAttributes().add(charProp);
                            break;
                        }
                        default : {
                            cerr << "unknown owner for field decl! element type: " << Element::elementTypeToString(data.owningElementType) << endl; 
                        }
                    }
                    break;
                }
                case CXTypeKind::CXType_Int : {
                    Property& intProp = data.manager.create<Property>();
                    intProp.setName(clang_getCString(clang_getCursorSpelling(c)));
                    intProp.setType(&data.manager.get<PrimitiveType>(ID::fromString("C_int_ZvgWKuxGtKtjRQPMNTXjic")));
                    intProp.setVisibility(data.visibilty);
                    switch (data.owningElementType) {
                        case ElementType::CLASS : {
                            data.owningElement.as<Class>().getOwnedAttributes().add(intProp);
                            break;
                        }
                        default : {
                            cerr << "unknown owner for field decl! element type: " << Element::elementTypeToString(data.owningElementType) << endl; 
                        }
                    }
                    break;
                }
                case CXTypeKind::CXType_Float : {
                    Property& floatProp = data.manager.create<Property>();
                    floatProp.setName(clang_getCString(clang_getCursorSpelling(c)));
                    floatProp.setType(&data.manager.get<PrimitiveType>(ID::fromString("C_float_FRQyo8d1KEQQLOnnPPn6")));
                    floatProp.setVisibility(data.visibilty);
                    switch (data.owningElementType) {
                        case ElementType::CLASS : {
                            data.owningElement.as<Class>().getOwnedAttributes().add(floatProp);
                            break;
                        }
                        default : {
                            cerr << "unknown owner for field decl! element type: " << Element::elementTypeToString(data.owningElementType) << endl; 
                        }
                    }
                    break;
                }
                case CXTypeKind::CXType_Double : {
                    Property& doubleProp = data.manager.create<Property>();
                    doubleProp.setName(clang_getCString(clang_getCursorSpelling(c)));
                    doubleProp.setType(&data.manager.get<PrimitiveType>(ID::fromString("C_double_HM2asoTiFmoWEK8ZuAE")));
                    doubleProp.setVisibility(data.visibilty);
                    switch (data.owningElementType) {
                        case ElementType::CLASS : {
                            data.owningElement.as<Class>().getOwnedAttributes().add(doubleProp);
                            break;
                        }
                        default : {
                            cerr << "unknown owner for field decl! element type: " << Element::elementTypeToString(data.owningElementType) << endl; 
                        }
                    }
                    break;
                }
                default : {
                    cerr << "unhandled type for class field (property)! cursor type: " << clang_getCString(clang_getTypeSpelling(clang_getCursorType(c))) << endl;
                }
            }
            break;
        }
        case CXCursor_CXXMethod : {
            Operation& method = data.manager.create<Operation>();
            method.setName(clang_getCString(clang_getCursorSpelling(c)));
            method.setVisibility(data.visibilty);
            CXType type = clang_getCursorType(c);

            // return parameter
            CXType return_type = clang_getResultType(type);
            switch (return_type.kind) {
                case CXTypeKind::CXType_Void : {
                    break;
                }
                default : {
                    Parameter& returnParam = data.manager.create<Parameter>();
                    returnParam.setName("return");
                    returnParam.setDirection(ParameterDirectionKind::RETURN);
                    switch (return_type.kind) {
                        case CXTypeKind::CXType_Char_S : {
                            returnParam.setType(&data.manager.get<PrimitiveType>(ID::fromString("C_char_bvN6xdQ&&LaR7MU_F_9uR")));
                            break;
                        }
                        case CXTypeKind::CXType_Int : {
                            returnParam.setType(&data.manager.get<PrimitiveType>(ID::fromString("C_int_ZvgWKuxGtKtjRQPMNTXjic")));
                            break;
                        }
                        case CXTypeKind::CXType_Float : {
                            returnParam.setType(&data.manager.get<PrimitiveType>(ID::fromString("C_float_FRQyo8d1KEQQLOnnPPn6")));
                            break;
                        }
                        case CXTypeKind::CXType_Double : {
                            returnParam.setType(&data.manager.get<PrimitiveType>(ID::fromString("C_double_HM2asoTiFmoWEK8ZuAE")));
                            break;
                        }
                        default : {
                            cerr << "unhandled parameter for method " << method.getName() << " of type '" << clang_getCString(clang_getTypeSpelling(return_type)) << "'" <<endl;
                            break;
                        }
                    }
                    method.getOwnedParameters().add(returnParam);
                    break;
                }
            }

            // signature parameters           
            switch (type.kind) {
                case CXType_FunctionProto : {
                    for (size_t i = 0; i < clang_Cursor_getNumArguments(c); i++) {
                        CXCursor param_c = clang_Cursor_getArgument(c, i);
                        switch (clang_getCursorKind(param_c)) {
                            case CXCursor_ParmDecl : {
                                Parameter& param = data.manager.create<Parameter>();
                                param.setName(clang_getCString(clang_getCursorSpelling(param_c)));
                                param.setDirection(ParameterDirectionKind::IN);
                                CXType param_type = clang_getCursorType(param_c);
                                switch (param_type.kind) {
                                    case CXTypeKind::CXType_Char_S : {
                                        param.setType(&data.manager.get<PrimitiveType>(ID::fromString("C_char_bvN6xdQ&&LaR7MU_F_9uR")));
                                        break;
                                    }
                                    case CXTypeKind::CXType_Int : {
                                        param.setType(&data.manager.get<PrimitiveType>(ID::fromString("C_int_ZvgWKuxGtKtjRQPMNTXjic")));
                                        break;
                                    }
                                    case CXTypeKind::CXType_Float : {
                                        param.setType(&data.manager.get<PrimitiveType>(ID::fromString("C_float_FRQyo8d1KEQQLOnnPPn6")));
                                        break;
                                    }
                                    case CXTypeKind::CXType_Double : {
                                        param.setType(&data.manager.get<PrimitiveType>(ID::fromString("C_double_HM2asoTiFmoWEK8ZuAE")));
                                        break;
                                    }
                                    default : {
                                        cerr << "unhandled parameter for method " << method.getName() << " of type '" << clang_getCString(clang_getTypeSpelling(clang_getCursorType(param_c))) << "'" <<endl;
                                        break;
                                    }
                                }
                                method.getOwnedParameters().add(param);
                                break;
                            }
                            default : {
                                cerr << "method contains cursor '" << clang_getCString(clang_getCursorSpelling(param_c)) << "' of kind '" << clang_getCString(clang_getCursorKindSpelling(clang_getCursorKind(param_c))) << "', but no mapping is set" << endl;
                            }
                        }
                    }
                    break;
                }
                default : {
                    cerr << "unhandled argument for method " << method.getName() << " , type " << clang_getCString(clang_getCursorSpelling(c)) << endl;
                }
            }
            switch (data.owningElementType) {
                case ElementType::CLASS : {
                    data.owningElement.as<Class>().getOperations().add(method);
                    break;
                }
                default : {
                    cerr << "unhandled owning element " << Element::elementTypeToString(data.owningElementType) << endl;
                    break; 
                }
            }
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
        case CXCursor_ClassDecl : {
            Class& cppClass = data->manager.create<Class>();
            cppClass.setName(clang_getCString(clang_getCursorSpelling(c)));
            setOwnerHelper(cppClass, data->owningElement);
            CppParserMetaData classData = {data->manager, cppClass, cppClass.getElementType()};
            clang_visitChildren(c, *classVisit, &classData);
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