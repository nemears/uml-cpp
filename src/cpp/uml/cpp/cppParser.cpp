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
#include "uml/literalBool.h"
#include "uml/association.h"

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
            throw UmlCppParserException("No execution mapped for cpp namespace with parent type " + owner.getElementTypeString());
        }
    }
}

void addC_ClassAssociation(CXCursor c, CppParserMetaData& data, Association& assoc) {
    Element* el = &data.owningElement;
    while (!el->isSubClassOf(ElementType::PACKAGE) && el->getOwner() != 0) {
        el = el->getOwner();
    }
    if (el->isSubClassOf(ElementType::PACKAGE)) {
        el->as<Package>().getPackagedElements().add(assoc);
    } else {
        throw UmlCppParserException("could not find package to put association for pointer in!" + fileNameAndLineNumber(c));
    }
}

void setC_PropType(CXType type, CppParserMetaData& data, CXCursor c, Property& prop) {
    switch (type.kind) {
        case CXTypeKind::CXType_Bool : {
            prop.setType(&data.manager.get<PrimitiveType>(ID::fromString("C_bool_sWBeSxCp5A7Ns9OJ4tBdG")));
            break;
        }
        case CXTypeKind::CXType_Char_S : {
            prop.setType(&data.manager.get<PrimitiveType>(ID::fromString("C_char_bvN6xdQ&&LaR7MU_F_9uR")));
            break;
        }
        case CXTypeKind::CXType_Int : {
            prop.setType(&data.manager.get<PrimitiveType>(ID::fromString("C_int_ZvgWKuxGtKtjRQPMNTXjic")));
            break;
        }
        case CXTypeKind::CXType_Float : {
            prop.setType(&data.manager.get<PrimitiveType>(ID::fromString("C_float_FRQyo8d1KEQQLOnnPPn6")));
            break;
        }
        case CXTypeKind::CXType_Double : {
            prop.setType(&data.manager.get<PrimitiveType>(ID::fromString("C_double_HM2asoTiFmoWEK8ZuAE")));
            break;
        }
        default : {
            CXString spelling = clang_getTypeSpelling(type);
            throw UmlCppParserException("unhandled property type, '" + string(clang_getCString(spelling)) + "' " + fileNameAndLineNumber(c));
            clang_disposeString(spelling);
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
                    throw UmlCppParserException("Cpp class contains access specifier with unset mappings");
                }
            }
            break;
        }
        case CXCursor_Constructor : {
            Operation& constructor = data.manager.create<Operation>();
            CXString constructorSpelling = clang_getCursorSpelling(c);
            constructor.setName(clang_getCString(constructorSpelling));
            clang_disposeString(constructorSpelling);
            setOwnerHelper(constructor, data.owningElement);
            break;
        }
        case CXCursor_FieldDecl : {
            CXType type = clang_getCursorType(c);
            Property& prop = data.manager.create<Property>();
            CXString propSpelling = clang_getCursorSpelling(c);
            prop.setName(clang_getCString(propSpelling));
            clang_disposeString(propSpelling);
            switch (data.owningElementType) {
                case ElementType::CLASS : {
                    data.owningElement.as<Class>().getOwnedAttributes().add(prop);
                    break;
                }
                default : {
                    throw UmlCppParserException("unknown owner for field decl! element type: " + Element::elementTypeToString(data.owningElementType) + fileNameAndLineNumber(c)); 
                }
            }
            switch (type.kind) {
                case CXTypeKind::CXType_Bool : {
                    prop.setType(&data.manager.get<PrimitiveType>(ID::fromString("C_bool_sWBeSxCp5A7Ns9OJ4tBdG")));
                    prop.setVisibility(data.visibilty);
                    prop.setAggregation(AggregationKind::COMPOSITE);
                    prop.setLower(1);
                    prop.setUpper(1);
                    break;
                }
                case CXTypeKind::CXType_Char_S : {
                    prop.setType(&data.manager.get<PrimitiveType>(ID::fromString("C_char_bvN6xdQ&&LaR7MU_F_9uR")));
                    prop.setVisibility(data.visibilty);
                    prop.setAggregation(AggregationKind::COMPOSITE);
                    prop.setLower(1);
                    prop.setUpper(1);
                    break;
                }
                case CXTypeKind::CXType_Int : {
                    prop.setType(&data.manager.get<PrimitiveType>(ID::fromString("C_int_ZvgWKuxGtKtjRQPMNTXjic")));
                    prop.setVisibility(data.visibilty);
                    prop.setAggregation(AggregationKind::COMPOSITE);
                    prop.setLower(1);
                    prop.setUpper(1);
                    break;
                }
                case CXTypeKind::CXType_Float : {
                    prop.setType(&data.manager.get<PrimitiveType>(ID::fromString("C_float_FRQyo8d1KEQQLOnnPPn6")));
                    prop.setVisibility(data.visibilty);
                    prop.setAggregation(AggregationKind::COMPOSITE);
                    prop.setLower(1);
                    prop.setUpper(1);
                    break;
                }
                case CXTypeKind::CXType_Double : {
                    prop.setType(&data.manager.get<PrimitiveType>(ID::fromString("C_double_HM2asoTiFmoWEK8ZuAE")));
                    prop.setVisibility(data.visibilty);
                    prop.setAggregation(AggregationKind::COMPOSITE);
                    prop.setLower(1);
                    prop.setUpper(1);
                    break;
                }
                case CXTypeKind::CXType_ConstantArray : {
                    CXType arrayType = clang_getElementType(type);
                    prop.setAggregation(AggregationKind::COMPOSITE);
                    setC_PropType(arrayType, data, c, prop);
                    CppParserMetaData arrayData = {data.manager, data.unit, prop, ElementType::PROPERTY, VisibilityKind::PUBLIC};
                    clang_visitChildren(c, &arrayVisit, &arrayData);
                    break;
                }
                case CXTypeKind::CXType_Pointer : {
                    CXType ptrType = clang_getPointeeType(type);
                    // multiplicity unspecified without malloc or something
                    setC_PropType(ptrType, data, c, prop);
                    Association& ptrAssoc = data.manager.create<Association>();
                    ptrAssoc.getMemberEnds().add(prop);
                    Property& assocEnd = data.manager.create<Property>();
                    assocEnd.setType(&data.owningElement.as<Type>());
                    ptrAssoc.getNavigableOwnedEnds().add(assocEnd);
                    addC_ClassAssociation(c, data, ptrAssoc);
                    break;
                }
                case CXTypeKind::CXType_LValueReference : {
                    CXType refType = clang_getPointeeType(type);
                    setC_PropType(refType, data, c, prop);
                    prop.setLower(1);
                    prop.setUpper(1);
                    prop.setAggregation(AggregationKind::SHARED); // I think this is how we will specify reference vs value vs ptr

                    Association& refAssoc = data.manager.create<Association>();
                    refAssoc.getMemberEnds().add(prop);
                    Property& assocEnd = data.manager.create<Property>();
                    assocEnd.setType(&data.owningElement.as<Type>());
                    refAssoc.getNavigableOwnedEnds().add(assocEnd);
                    addC_ClassAssociation(c, data, refAssoc);

                    break;
                }
                default : {
                    CXString spelling = clang_getCursorSpelling(c);
                    CXString kindSpelling = clang_getCursorKindSpelling(clang_getCursorKind(c));
                    throw UmlCppParserException("unhandled type for class field (property)! name: " + string(clang_getCString(spelling)) + " of type " + string(clang_getCString(kindSpelling)) + fileNameAndLineNumber(c));
                    clang_disposeString(spelling);
                    clang_disposeString(kindSpelling);
                    return CXChildVisit_Recurse;
                }
            }
            break;
        }
        case CXCursor_CXXMethod : {
            Operation& method = data.manager.create<Operation>();
            CXString methodSpelling = clang_getCursorSpelling(c);
            method.setName(clang_getCString(methodSpelling));
            clang_disposeString(methodSpelling);
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
                            CXString spelling = clang_getTypeSpelling(return_type);
                            throw UmlCppParserException("unhandled parameter for method " + method.getName() + " of type '" + string(clang_getCString(spelling)) + "'" + fileNameAndLineNumber(c));
                            clang_disposeString(spelling);
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
                                CXString paramSpelling = clang_getCursorSpelling(param_c);
                                param.setName(clang_getCString(paramSpelling));
                                clang_disposeString(paramSpelling);
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
                                        CXString spelling = clang_getTypeSpelling(clang_getCursorType(param_c));
                                        throw UmlCppParserException("unhandled parameter for method " + method.getName() + " of type '" + string(clang_getCString(spelling)) + "'" + fileNameAndLineNumber(c));
                                        clang_disposeString(spelling);
                                        break;
                                    }
                                }
                                method.getOwnedParameters().add(param);
                                break;
                            }
                            default : {
                                CXString spelling = clang_getCursorSpelling(param_c);
                                CXString kindSpelling = clang_getCursorKindSpelling(clang_getCursorKind(param_c));
                                throw UmlCppParserException("method contains cursor '" + string(clang_getCString(spelling)) + "' of kind '" + string(clang_getCString(kindSpelling)) + "', but no mapping is set" + fileNameAndLineNumber(c));
                                clang_disposeString(spelling);
                                clang_disposeString(kindSpelling);
                            }
                        }
                    }
                    break;
                }
                default : {
                    CXString spelling = clang_getCursorSpelling(c);
                    throw UmlCppParserException("unhandled argument for method " + method.getName() + " , type " + string(clang_getCString(spelling)) + fileNameAndLineNumber(c));
                    clang_disposeString(spelling);
                }
            }
            switch (data.owningElementType) {
                case ElementType::CLASS : {
                    data.owningElement.as<Class>().getOperations().add(method);
                    break;
                }
                default : {
                    throw UmlCppParserException("unhandled owning element " + Element::elementTypeToString(data.owningElementType) + fileNameAndLineNumber(c));
                    break; 
                }
            }
            break;
        }
        default : {
            CXString spelling = clang_getCursorSpelling(c);
            CXString kindSpelling = clang_getCursorKindSpelling(clang_getCursorKind(c));
            throw UmlCppParserException("Cpp class contains Cursor '" + string(clang_getCString(spelling)) + "' of kind '" + string(clang_getCString(kindSpelling)) + "', but no mapping is set" + fileNameAndLineNumber(c));
            clang_disposeString(spelling);
            clang_disposeString(kindSpelling);
            return CXChildVisit_Recurse;
        }
    }
    return CXChildVisit_Continue;
}

CXChildVisitResult arrayVisit(CXCursor c, CXCursor parent, CXClientData client_data) {
    CppParserMetaData& data = *static_cast<CppParserMetaData*>(client_data);
    switch (clang_getCursorKind(c)) {
        case CXCursor_IntegerLiteral : {
            data.owningElement.as<Property>().setLower(0);
            CXSourceRange range = clang_getCursorExtent(c);
            CXToken *tokens = 0;
            unsigned int nTokens = 0;
            clang_tokenize(data.unit, range, &tokens, &nTokens);
            for (unsigned int i = 0; i < nTokens; i++)
            {
                CXString spelling = clang_getTokenSpelling(data.unit, tokens[i]);
                data.owningElement.as<Property>().setUpper(atoi((clang_getCString(spelling))));
                clang_disposeString(spelling);
            }
            clang_disposeTokens(data.unit, tokens, nTokens);
            break;
        }
        default : {
            CXString spelling = clang_getCursorSpelling(c);
            CXString kindSpelling = clang_getCursorKindSpelling(clang_getCursorKind(c));
            throw UmlCppParserException("Cpp array contains Cursor '" + string(clang_getCString(spelling)) + "' of kind '" + string(clang_getCString(kindSpelling)) + "', but no mapping is set" + fileNameAndLineNumber(c));
            clang_disposeString(spelling);
            clang_disposeString(kindSpelling);
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
                case CXTypeKind::CXType_Bool : {
                    LiteralBool& cBool = data->manager.create<LiteralBool>();
                    CXString spelling = clang_getCursorSpelling(c);
                    cBool.setName(clang_getCString(spelling));
                    clang_disposeString(spelling);
                    cBool.setType(&data->manager.get<PrimitiveType>(ID::fromString("C_bool_sWBeSxCp5A7Ns9OJ4tBdG")));
                    if (data->owningElement.getElementType() == ElementType::PACKAGE) {
                        data->owningElement.as<Package>().getPackagedElements().add(cBool);
                    }
                    break;
                }
                case CXTypeKind::CXType_Char_S : {
                    LiteralInt& cChar = data->manager.create<LiteralInt>();
                    CXString spelling = clang_getCursorSpelling(c);
                    cChar.setName(clang_getCString(spelling));
                    clang_disposeString(spelling);
                    cChar.setType(&data->manager.get<PrimitiveType>(ID::fromString("C_char_bvN6xdQ&&LaR7MU_F_9uR")));
                    if (data->owningElement.getElementType() == ElementType::PACKAGE) {
                        dynamic_cast<Package&>(data->owningElement).getPackagedElements().add(cChar);
                    }
                    break;
                }
                case CXTypeKind::CXType_Int : {
                    LiteralInt& cInt = data->manager.create<LiteralInt>();
                    CXString spelling = clang_getCursorSpelling(c);
                    cInt.setName(clang_getCString(spelling));
                    clang_disposeString(spelling);
                    cInt.setType(&data->manager.get<PrimitiveType>(ID::fromString("C_int_ZvgWKuxGtKtjRQPMNTXjic")));
                    if (data->owningElement.getElementType() == ElementType::PACKAGE) {
                        dynamic_cast<Package&>(data->owningElement).getPackagedElements().add(cInt);
                    }
                    break;
                }
                case CXTypeKind::CXType_Float : {
                    LiteralReal& cFloat = data->manager.create<LiteralReal>();
                    CXString spelling = clang_getCursorSpelling(c);
                    cFloat.setName(clang_getCString(spelling));
                    clang_disposeString(spelling);
                    cFloat.setType(&data->manager.get<PrimitiveType>(ID::fromString("C_float_FRQyo8d1KEQQLOnnPPn6")));
                    if (data->owningElementType == ElementType::PACKAGE) {
                        dynamic_cast<Package&>(data->owningElement).getPackagedElements().add(cFloat);
                    }
                    break;
                }
                case CXTypeKind::CXType_Double : {
                    LiteralReal& cDouble = data->manager.create<LiteralReal>();
                    CXString spelling = clang_getCursorSpelling(c);
                    cDouble.setName(clang_getCString(spelling));
                    clang_disposeString(spelling);
                    cDouble.setType(&data->manager.get<PrimitiveType>(ID::fromString("C_double_HM2asoTiFmoWEK8ZuAE")));
                    if (data->owningElementType == ElementType::PACKAGE) {
                        dynamic_cast<Package&>(data->owningElement).getPackagedElements().add(cDouble);
                    }
                    break;
                }
                case CXTypeKind::CXType_ConstantArray : {

                }
                case CXTypeKind::CXType_Void : {
                    // TODO
                }
                default : {
                    CXString spelling = clang_getCursorSpelling(c);
                    CXString kindSpelling = clang_getCursorKindSpelling(clang_getCursorKind(c));
                    throw UmlCppParserException("Unahandled type for variable type " + string(clang_getCString(spelling)) + " of kind " + string(clang_getCString(kindSpelling)) + fileNameAndLineNumber(c));
                    clang_disposeString(spelling);
                    clang_disposeString(kindSpelling);
                    break;
                }
            }
            break;
        }
        case CXCursor_ClassDecl : {
            Class& cppClass = data->manager.create<Class>();
            CXString spelling = clang_getCursorSpelling(c);
            cppClass.setName(clang_getCString(spelling));
            clang_disposeString(spelling);
            setOwnerHelper(cppClass, data->owningElement);
            CppParserMetaData classData = {data->manager, data->unit, cppClass, cppClass.getElementType()};
            clang_visitChildren(c, *classVisit, &classData);
            break;
        }
        default : {
            CXString spelling = clang_getCursorSpelling(c);
            CXString kindSpelling = clang_getCursorKindSpelling(clang_getCursorKind(c));
            throw UmlCppParserException("Cpp namespace contains Cursor '" + string(clang_getCString(spelling)) + "' of kind '" + string(clang_getCString(kindSpelling)) + ", but no mapping has been defined yet!" + fileNameAndLineNumber(c));
            clang_disposeString(spelling);
            clang_disposeString(kindSpelling);
            break;
        }
    }
    return CXChildVisit_Recurse;
}

CXChildVisitResult headerVisit(CXCursor c, CXCursor parent, CXClientData client_data) {
    CppParserMetaData* data = static_cast<CppParserMetaData*>(client_data);
    if (data->manager.count(ID::fromString("CPP_4zpFq9s6YilFjqZAPguiluqk")) == 0) {
        // TODO throw error
        throw UmlCppParserException("ERROR: Did not load C++ Profile before parsing header!");
    }
    switch (clang_getCursorKind(c)) {
        case CXCursor_Namespace : {
            Package& namespacePckg = data->manager.create<Package>();
            CXString spelling = clang_getCursorSpelling(c);
            namespacePckg.setName(clang_getCString(spelling));
            clang_disposeString(spelling);
            InstanceSpecification& stereotypeInst = data->manager.create<InstanceSpecification>();
            stereotypeInst.setClassifier(&data->manager.get<Stereotype>(ID::fromString("Cpp_NAMESPACE_3FloKgLhiH2P0t")));
            namespacePckg.getAppliedStereotypes().add(stereotypeInst);
            setOwnerHelper(namespacePckg, data->owningElement);
            CppParserMetaData namespaceData = {data->manager, data->unit, namespacePckg, namespacePckg.getElementType()};
            clang_visitChildren(c, *namespaceVisit, &namespaceData);
            break;
        }
        case CXCursor_ClassDecl : {
            Class& cppClass = data->manager.create<Class>();
            CXString spelling = clang_getCursorSpelling(c);
            cppClass.setName(clang_getCString(spelling));
            clang_disposeString(spelling);
            setOwnerHelper(cppClass, data->owningElement);
            data->visibilty = VisibilityKind::PRIVATE; // access for class is default private (struct is default public)
            CppParserMetaData classData = {data->manager, data->unit, cppClass, cppClass.getElementType()};
            clang_visitChildren(c, *classVisit, &classData);
            break;
        }
        default : {
            CXString spelling = clang_getCursorSpelling(c);
            CXString kindSpelling = clang_getCursorKindSpelling(clang_getCursorKind(c));
            throw UmlCppParserException("Cursor '" + string(clang_getCString(spelling)) + "' of kind '" + string(clang_getCString(kindSpelling)) + "' does not have parsing mapped to it yet!" + fileNameAndLineNumber(c));
            clang_disposeString(spelling);
            clang_disposeString(kindSpelling);
            break;
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
    CppParserMetaData data = {manager, unit, *umlParent, umlParentType};

    // start going through the AST
    clang_visitChildren(cursor,*headerVisit, &data);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);

    return ret;
}

std::string fileNameAndLineNumber(CXCursor c) {
    CXSourceRange range = clang_getCursorExtent(c);
    CXSourceLocation rangeStart = clang_getRangeStart(range);
    unsigned line;
    CXFile file;
    clang_getFileLocation(rangeStart, &file, &line, 0, 0);
    CXString fileSpelling = clang_File_tryGetRealPathName(file);
    string ret = " file, " + string(clang_getCString(fileSpelling)) + ", line number, " + to_string(line);
    clang_disposeString(fileSpelling);
    return ret; 
}

}
}