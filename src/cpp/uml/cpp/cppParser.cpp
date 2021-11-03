#include "uml/cpp/cppParser.h"
#include "uml/cpp/clang.h"
#include "uml/uml-stable.h"

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
                    dynamic_cast<Class&>(owner).getOwnedOperations().add(dynamic_cast<Operation&>(ownee));
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

void setC_RecordType(CXType type, CppParserMetaData& data, CXCursor c, Property& prop) {
    CXString recordString = clang_getTypeSpelling(type);
    if (data.owningElement.getOwner()) {
        if (data.owningElement.getOwner()->isSubClassOf(ElementType::NAMESPACE)) {
            NamedElement& record = data.owningElement.getOwner()->as<Namespace>().getMembers().get(clang_getCString(recordString));
            if (record.isSubClassOf(ElementType::TYPE)) {
                prop.setType(&record.as<Type>());
            }
        } else {
            throw UmlCppParserException("could not find predefined type " + string(clang_getCString(recordString)) + fileNameAndLineNumber(c));
        }
    } else {
        throw UmlCppParserException("could not find owner of class " + data.owningElement.as<NamedElement>().getName() + fileNameAndLineNumber(c));
    }
    clang_disposeString(recordString);
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
        case CXTypeKind::CXType_Record : {
            setC_RecordType(type, data, c, prop);
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
            switch (data.owningElement.getElementType()) {
                case ElementType::CLASS : {
                    data.owningElement.as<Class>().getOwnedAttributes().add(prop);
                    break;
                }
                default : {
                    throw UmlCppParserException("unknown owner for field decl! element type: " + Element::elementTypeToString(data.owningElement.getElementType()) + fileNameAndLineNumber(c)); 
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
                    CppParserMetaData arrayData = {data.manager, data.unit, prop, VisibilityKind::PUBLIC};
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
                case CXTypeKind::CXType_Record : {
                    setC_RecordType(type, data, c, prop);
                    break;
                }
                default : {
                    CXString spelling = clang_getCursorSpelling(c);
                    CXString typeSpelling = clang_getTypeSpelling(type);
                    throw UmlCppParserException("unhandled type for class field (property)! name: " + string(clang_getCString(spelling)) + " of type " + string(clang_getCString(typeSpelling)) + fileNameAndLineNumber(c));
                    clang_disposeString(spelling);
                    clang_disposeString(typeSpelling);
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
            switch (data.owningElement.getElementType()) {
                case ElementType::CLASS : {
                    data.owningElement.as<Class>().getOwnedOperations().add(method);
                    break;
                }
                default : {
                    throw UmlCppParserException("unhandled owning element " + Element::elementTypeToString(data.owningElement.getElementType()) + fileNameAndLineNumber(c));
                    break; 
                }
            }
            break;
        }
        case CXCursor_CXXBaseSpecifier : {
            CXType type = clang_getCursorType(c);
            CXString typeSpelling = clang_getTypeSpelling(type);
            std::string generalName(clang_getCString(typeSpelling));
            Classifier& general = data.owningElement.getOwnerRef().as<Namespace>().getMembers().get(generalName).as<Classifier>();
            Generalization& generalization = data.manager.create<Generalization>();
            generalization.setGeneral(general);
            data.owningElement.as<Class>().getGeneralizations().add(generalization);
            // TODO handle virtual inheritance with generalization sets & access specifiers with stereotype, or generalization set
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
    switch (data.owningElement.getElementType()) {
        case ElementType::INSTANCE_SPECIFICATION : {
            switch (clang_getCursorKind(c)) {
                case CXCursor_IntegerLiteral : {
                    Slot& sizeSlot = data.manager.create<Slot>();
                    data.owningElement.as<InstanceSpecification>().getAppliedStereotypes().get("C++ Array").getSlots().add(sizeSlot);
                    sizeSlot.setDefiningFeature(&data.owningElement.getAppliedStereotypes().get("C++ Array").getClassifier()->getAttributes().get("size"));
                    LiteralInt& sizeValue = data.manager.create<LiteralInt>();
                    CXSourceRange range = clang_getCursorExtent(c);
                    CXToken *tokens = 0;
                    unsigned int nTokens = 0;
                    clang_tokenize(data.unit, range, &tokens, &nTokens);
                    for (unsigned int i = 0; i < nTokens; i++)
                    {
                        CXString spelling = clang_getTokenSpelling(data.unit, tokens[i]);
                        sizeValue.setValue(atoi(clang_getCString(spelling)));
                        sizeSlot.getValues().add(sizeValue);
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
            break;
        }
        case ElementType::PROPERTY : {
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
            break;
        }
        default : {
            /** TODO: throw error **/
        }
    }
    return CXChildVisit_Continue;
}

void setC_VariableType(CXType type, InstanceSpecification& variable, CppParserMetaData& data, CXCursor c) {
    switch (type.kind) {
        case CXTypeKind::CXType_Bool : {
            variable.setClassifier(&data.manager.get<PrimitiveType>(ID::fromString("C_bool_sWBeSxCp5A7Ns9OJ4tBdG")));
            break;
        }
        case CXTypeKind::CXType_Char_S : {
            variable.setClassifier(&data.manager.get<PrimitiveType>(ID::fromString("C_char_bvN6xdQ&&LaR7MU_F_9uR")));
            break;
        }
        case CXTypeKind::CXType_Int : {
            variable.setClassifier(&data.manager.get<PrimitiveType>(ID::fromString("C_int_ZvgWKuxGtKtjRQPMNTXjic")));
            break;
        }
        case CXTypeKind::CXType_Float : {
            variable.setClassifier(&data.manager.get<PrimitiveType>(ID::fromString("C_float_FRQyo8d1KEQQLOnnPPn6")));
            break;
        }
        case CXTypeKind::CXType_Double : {
            variable.setClassifier(&data.manager.get<PrimitiveType>(ID::fromString("C_double_HM2asoTiFmoWEK8ZuAE")));
            break;
        }
        case CXTypeKind::CXType_Record : {
            CXString recordTypeSpelling = clang_getTypeSpelling(type);
            string recordName = string(clang_getCString(recordTypeSpelling));
            variable.setClassifier(&data.owningElement.as<Package>().getPackagedElements().get(recordName.substr(recordName.find_last_of("::") + 1)).as<Classifier>());
            clang_disposeString(recordTypeSpelling);
            break;
        }
        default : {
            CXString spelling = clang_getCursorSpelling(c);
            CXString typeSpelling = clang_getTypeKindSpelling(type.kind);
            throw UmlCppParserException("Cannot currently handle clang type " + string(clang_getCString(typeSpelling)) + " for array " + string(clang_getCString(spelling)) + " " + fileNameAndLineNumber(c));
            clang_disposeString(spelling);
            clang_disposeString(typeSpelling);
            break;
        }
    }
}

void c_evalPrimitiveInst(CXCursor c, LiteralSpecification& val) {
    CXEvalResult evalResult = clang_Cursor_Evaluate(c);
    switch (clang_EvalResult_getKind(evalResult)) {
        case CXEvalResultKind::CXEval_Int : {
            switch (val.getElementType()) {
                case ElementType::LITERAL_BOOL : {
                    val.as<LiteralBool>().setValue(clang_EvalResult_getAsInt(evalResult));
                    break;
                }
                case ElementType::LITERAL_INT : {
                    val.as<LiteralInt>().setValue(clang_EvalResult_getAsInt(evalResult));
                    break;
                }
            }
            break;
        }
        case CXEvalResultKind::CXEval_Float : {
            switch (val.getElementType()) {
                case ElementType::LITERAL_REAL : {
                    val.as<LiteralReal>().setValue(clang_EvalResult_getAsDouble(evalResult));
                    break;
                }
            }
            break;
        }
        default : {
            throw UmlCppParserException("Could not evaluate literal boolean!" + fileNameAndLineNumber(c));
        }
    }
    clang_EvalResult_dispose(evalResult);
}

CXChildVisitResult primitiveVisit(CXCursor c, CXCursor parent, CXClientData client_data) {
    CppParserMetaData& data = *static_cast<CppParserMetaData*>(client_data);
    Slot& valSlot = data.manager.create<Slot>();
    valSlot.setDefiningFeature(&data.owningElement.as<InstanceSpecification>().getClassifier()->getAttributes().get("value"));
    valSlot.setOwningInstance(&data.owningElement.as<InstanceSpecification>());
    switch (clang_getCursorKind(c)) {
        case CXCursor_CXXBoolLiteralExpr : {
            LiteralBool& boolVal = data.manager.create<LiteralBool>();
            valSlot.getValues().add(boolVal);
            c_evalPrimitiveInst(c, boolVal);
            break;
        }
        case CXCursor_CharacterLiteral : {
            LiteralInt& charVal = data.manager.create<LiteralInt>();
            valSlot.getValues().add(charVal);
            c_evalPrimitiveInst(c, charVal);
            break;
        }
        case CXCursor_IntegerLiteral : {
            LiteralInt& intVal = data.manager.create<LiteralInt>();
            valSlot.getValues().add(intVal);
            c_evalPrimitiveInst(c, intVal);
            break;
        }
        case CXCursor_UnexposedExpr : {
            LiteralReal& realVal = data.manager.create<LiteralReal>();
            valSlot.getValues().add(realVal);
            c_evalPrimitiveInst(c, realVal);
            break;
        }
        case CXCursor_FloatingLiteral : {
            LiteralReal& realVal = data.manager.create<LiteralReal>();
            valSlot.getValues().add(realVal);
            c_evalPrimitiveInst(c, realVal);
            break;
        }
        default : {
            CXString spelling = clang_getCursorKindSpelling(c.kind);
            throw UmlCppParserException("No mapping for character value for cursor " + string(clang_getCString(spelling)) + " " + fileNameAndLineNumber(c));
            clang_disposeString(spelling);
        }
    }

    return CXChildVisit_Continue;
}

void parseNonClassVariable(CXCursor c, CppParserMetaData& data) {
    InstanceSpecification& variable = data.manager.create<InstanceSpecification>();
    CXString spelling = clang_getCursorSpelling(c);
    variable.setName(clang_getCString(spelling));
    clang_disposeString(spelling);
    variable.setOwningPackage(&data.owningElement.as<Package>());
    CXType type = clang_getCursorType(c);
    switch (type.kind) {
        case CXTypeKind::CXType_Bool : {
            variable.setClassifier(&data.manager.get<PrimitiveType>(ID::fromString("C_bool_sWBeSxCp5A7Ns9OJ4tBdG")));
            CppParserMetaData boolInstData = {data.manager, data.unit, variable};
            clang_visitChildren(c, &primitiveVisit, &boolInstData);
            break;
        }
        case CXTypeKind::CXType_Char_S : {
            variable.setClassifier(&data.manager.get<PrimitiveType>(ID::fromString("C_char_bvN6xdQ&&LaR7MU_F_9uR")));
            CppParserMetaData charInstData = {data.manager, data.unit, variable};
            clang_visitChildren(c, &primitiveVisit, &charInstData);
            break;
        }
        case CXTypeKind::CXType_Int : {
            variable.setClassifier(&data.manager.get<PrimitiveType>(ID::fromString("C_int_ZvgWKuxGtKtjRQPMNTXjic")));
            CppParserMetaData intInstData = {data.manager, data.unit, variable};
            clang_visitChildren(c, &primitiveVisit, &intInstData);
            break;
        }
        case CXTypeKind::CXType_Float : {
            variable.setClassifier(&data.manager.get<PrimitiveType>(ID::fromString("C_float_FRQyo8d1KEQQLOnnPPn6")));
            CppParserMetaData floatInstData = {data.manager, data.unit, variable};
            clang_visitChildren(c, &primitiveVisit, &floatInstData);
            break;
        }
        case CXTypeKind::CXType_Double : {
            variable.setClassifier(&data.manager.get<PrimitiveType>(ID::fromString("C_double_HM2asoTiFmoWEK8ZuAE")));
            CppParserMetaData doubleInstData = {data.manager, data.unit, variable};
            clang_visitChildren(c, &primitiveVisit, &doubleInstData);
            break;
        }
        case CXTypeKind::CXType_ConstantArray : {
            InstanceSpecification& arrayStereotypeInst = data.manager.create<InstanceSpecification>();
            arrayStereotypeInst.setClassifier(&data.manager.get<Stereotype>(Profile::cppConstArrayID));
            arrayStereotypeInst.setName(arrayStereotypeInst.getClassifier()->getName());
            variable.getAppliedStereotypes().add(arrayStereotypeInst);
            CXType arrayType = clang_getElementType(type);
            setC_VariableType(arrayType, variable, data, c);
            CppParserMetaData arrayData = {data.manager, data.unit, variable, VisibilityKind::PUBLIC};
            clang_visitChildren(c, &arrayVisit, &arrayData);
            /** TODO: set value **/
            break;
        }
        case CXTypeKind::CXType_Pointer : {
            InstanceSpecification& pointerStereotypeInst = data.manager.create<InstanceSpecification>();
            pointerStereotypeInst.setClassifier(&data.manager.get<Stereotype>(Profile::cppPointerID));
            pointerStereotypeInst.setName(pointerStereotypeInst.getClassifier()->getName());
            variable.getAppliedStereotypes().add(pointerStereotypeInst);
            setC_VariableType(clang_getPointeeType(type), variable, data, c);
            /** TODO: set value **/
            break;
        }
        case CXTypeKind::CXType_LValueReference : {
            InstanceSpecification& referenceStereotypeInst = data.manager.create<InstanceSpecification>();
            referenceStereotypeInst.setClassifier(&data.manager.get<Stereotype>(Profile::cppReferenceID));
            referenceStereotypeInst.setName(referenceStereotypeInst.getClassifier()->getName());
            variable.getAppliedStereotypes().add(referenceStereotypeInst);
            setC_VariableType(clang_getPointeeType(type), variable, data, c);
            /** TODO: set value **/
            break;
        }
        case CXTypeKind::CXType_Void : {
            // TODO
        }
        case CXTypeKind::CXType_Record : {
            CXString recordTypeSpelling = clang_getTypeSpelling(type);
            string recordName = string(clang_getCString(recordTypeSpelling));
            variable.setClassifier(&data.owningElement.as<Package>().getPackagedElements().get(recordName.substr(recordName.find_last_of("::") + 1)).as<Classifier>());
            clang_disposeString(recordTypeSpelling);
            break;
        }
        default : {
            CXString spelling = clang_getCursorSpelling(c);
            CXString typeSpelling = clang_getTypeKindSpelling(type.kind);
            throw UmlCppParserException("Unahandled type for variable " + string(clang_getCString(spelling)) + " of clang type " + string(clang_getCString(typeSpelling)) + fileNameAndLineNumber(c));
            clang_disposeString(spelling);
            clang_disposeString(typeSpelling);
            break;
        }
    }
}

CXChildVisitResult namespaceVisit(CXCursor c, CXCursor parent, CXClientData client_data) {
    CppParserMetaData& data = *static_cast<CppParserMetaData*>(client_data);
    switch (clang_getCursorKind(c)) {
        case CXCursor_VarDecl : {
            parseNonClassVariable(c, data);
            return CXChildVisit_Continue;
        }
        case CXCursor_ClassDecl : {
            Class& cppClass = data.manager.create<Class>();
            CXString spelling = clang_getCursorSpelling(c);
            cppClass.setName(clang_getCString(spelling));
            clang_disposeString(spelling);
            setOwnerHelper(cppClass, data.owningElement);
            CppParserMetaData classData = {data.manager, data.unit, cppClass};
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
            stereotypeInst.setClassifier(&data->manager.get<Stereotype>(Profile::cppNamespaceID));
            namespacePckg.getAppliedStereotypes().add(stereotypeInst);
            setOwnerHelper(namespacePckg, data->owningElement);
            CppParserMetaData namespaceData = {data->manager, data->unit, namespacePckg};
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
            CppParserMetaData classData = {data->manager, data->unit, cppClass};
            clang_visitChildren(c, *classVisit, &classData);
            break;
        }
        case CXCursor_VarDecl : {
            parseNonClassVariable(c, *data);
            return CXChildVisit_Continue;
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
    CXIndex index = clang_createIndex(0, 0);
    const char* const clangArgs[1] = {"-xc++"};
    CXTranslationUnit unit = clang_parseTranslationUnit(index, path.c_str(), clangArgs, 1, 0, 0, CXTranslationUnit_None);
    if (unit == 0) {
        return 0;
    }

    filesystem::path headerPath = path;

    Package& containingPackage = manager.create<Package>();
    containingPackage.setName(headerPath.filename().string());

    Artifact& header = manager.create<Artifact>();
    header.setName(headerPath.filename().string());
    containingPackage.getPackagedElements().add(header);

    // get cursor
    CXCursor cursor = clang_getTranslationUnitCursor(unit);

    // set up client data
    Element* umlParent = &containingPackage;
    CppParserMetaData data = {manager, unit, *umlParent};

    // start going through the AST
    clang_visitChildren(cursor,*headerVisit, &data);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);

    return &containingPackage;
}

// TODO valid implementation
void parseCpp(std::string name, std::vector<std::string> headers, std::vector<std::string> sourceFiles, UmlManager& manager) {
    Package& deploymentPackage = manager.create<Package>();
    deploymentPackage.setName(name + "_deployment");
    Package& cppPackage = manager.create<Package>();
    cppPackage.setName(name);
    if (manager.getRoot() != 0) {
        manager.getRoot()->as<Package>().getPackagedElements().add(deploymentPackage, cppPackage);
        if (!manager.getRoot()->as<Package>().getPackagedElements().count(ID::fromString("CPP_4zpFq9s6YilFjqZAPguiluqk"))) {
            throw ManagerStateException("Did not have C++ profile owned by root package!");
        }
    }
    Deployment& deployment = manager.create<Deployment>();
    deployment.setName(name);
    deploymentPackage.getPackagedElements().add(deployment);
    for (string header : headers) {
        Artifact& headerArtifact = manager.create<Artifact>();
        headerArtifact.setName(header);
        parseHeader(header, manager); // TODO change to Artifact
        deployment.getDeployedArtifact().add(headerArtifact);
        deploymentPackage.getPackagedElements().add(headerArtifact);
    }

    // TODO source files
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