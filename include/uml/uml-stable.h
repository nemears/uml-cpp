#pragma once

#include "uml/types/classifierTemplateParameter.h"
#include "uml/types/artifact.h"
#include "uml/types/comment.h"
#include "uml/types/connector.h"
#include "uml/types/connectorEnd.h"
#include "uml/types/constraint.h"
#include "uml/types/deployedArtifact.h"
#include "uml/types/deployment.h"
#include "uml/types/elementImport.h"
#include "uml/types/enumeration.h"
#include "uml/types/enumerationLiteral.h"
#include "uml/types/expression.h"
#include "uml/types/extension.h"
#include "uml/types/extensionEnd.h"
#include "uml/types/generalization.h"
#include "uml/types/generalizationSet.h"
#include "uml/types/instanceValue.h"
#include "uml/types/interface.h"
#include "uml/types/interfaceRealization.h"
#include "uml/types/literalBool.h"
#include "uml/types/literalInt.h"
#include "uml/types/literalNull.h"
#include "uml/types/literalReal.h"
#include "uml/types/literalString.h"
#include "uml/types/literalUnlimitedNatural.h"
#include "uml/types/manifestation.h"
#include "uml/types/model.h"
#include "uml/types/opaqueBehavior.h"
#include "uml/types/operation.h"
#include "uml/types/package.h"
#include "uml/types/packageImport.h"
#include "uml/types/packageMerge.h"
#include "uml/types/parameter.h"
#include "uml/types/parameterSet.h"
#include "uml/types/port.h"
#include "uml/types/primitiveType.h"
#include "uml/types/profile.h"
#include "uml/types/profileApplication.h"
#include "uml/types/reception.h"
#include "uml/types/redefinableTemplateSignature.h"
#include "uml/types/signal.h"
#include "uml/types/slot.h"
#include "uml/types/stereotype.h"
#include "uml/types/usage.h"
#include "uml/types/templateParameter.h"
#include "uml/types/templateParameterSubstitution.h"

#include "uml/managers/umlManager.h"

namespace UML {
    typedef UmlPtr<ElementImport> ElementImportPtr;
    typedef UmlPtr<EnumerationLiteral> EnumerationLiteralPtr;
    typedef UmlPtr<Expression> ExpressionPtr;
    typedef UmlPtr<Extension> ExtensionPtr;
    typedef UmlPtr<Generalization> GeneralizationPtr;
    typedef UmlPtr<InstanceValue> InstanceValuePtr;
    typedef UmlPtr<LiteralBool> LiteralBoolPtr;
    typedef UmlPtr<LiteralInt> LiteralIntPtr;
    typedef UmlPtr<LiteralNull> LiteralNullPtr;
    typedef UmlPtr<LiteralReal> LiteralRealPtr;
    typedef UmlPtr<LiteralString> LiteralStringPtr;
    typedef UmlPtr<LiteralUnlimitedNatural> LiteralUnlimitedNaturalPtr;
    typedef UmlPtr<OpaqueBehavior> OpaqueBehaviorPtr;
    typedef UmlPtr<PackageImport> PackageImportPtr;
    typedef UmlPtr<PackageMerge> PackageMergePtr;
    typedef UmlPtr<Parameter> ParameterPtr;
    typedef UmlPtr<PrimitiveType> PrimitiveTypePtr;
    typedef UmlPtr<Slot> SlotPtr;
    typedef UmlPtr<StructuredClassifier> StructuredClassifierPtr;
}
