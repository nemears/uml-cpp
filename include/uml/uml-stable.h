#ifndef _UML_UML_STABLE_H_
#define _UML_UML_STABLE_H_

/**
 * Comprehensive include of all stable non-abstract parts of the api
 **/

#include "uml/types/abstraction.h"
#include "uml/types/action.h"
#include "uml/types/actionInputPin.h"
#include "uml/types/activity.h"
#include "uml/types/activityEdge.h"
#include "uml/types/activityFinalNode.h"
#include "uml/types/activityNode.h"
#include "uml/types/activityParameterNode.h"
#include "uml/types/activityPartition.h"
#include "uml/types/behavior.h"
#include "uml/types/callBehaviorAction.h"
#include "uml/types/centralBufferNode.h"
#include "uml/types/class.h"
#include "uml/types/classifierTemplateParameter.h"
#include "uml/types/comment.h"
#include "uml/types/controlFlow.h"
#include "uml/types/artifact.h"
#include "uml/types/association.h"
#include "uml/types/connector.h"
#include "uml/types/connectorEnd.h"
#include "uml/types/dataStoreNode.h"
#include "uml/types/dataType.h"
#include "uml/types/decisionNode.h"
#include "uml/types/dependency.h"
#include "uml/types/deployment.h"
#include "uml/types/elementImport.h"
#include "uml/types/enumeration.h"
#include "uml/types/enumerationLiteral.h"
#include "uml/types/exceptionHandler.h"
#include "uml/types/executableNode.h"
#include "uml/types/expression.h"
#include "uml/types/extension.h"
#include "uml/types/extensionEnd.h"
#include "uml/types/flowFinalNode.h"
#include "uml/types/forkNode.h"
#include "uml/types/generalization.h"
#include "uml/types/generalizationSet.h"
#include "uml/types/initialNode.h"
#include "uml/types/instanceSpecification.h"
#include "uml/types/instanceValue.h"
#include "uml/types/interface.h"
#include "uml/types/interfaceRealization.h"
#include "uml/types/interruptibleActivityRegion.h"
#include "uml/types/joinNode.h"
#include "uml/types/literalBool.h"
#include "uml/types/literalInt.h"
#include "uml/types/literalNull.h"
#include "uml/types/literalReal.h"
#include "uml/types/literalString.h"
#include "uml/types/literalUnlimitedNatural.h"
#include "uml/types/manifestation.h"
#include "uml/types/mergeNode.h"
#include "uml/types/model.h"
#include "uml/types/objectFlow.h"
#include "uml/types/opaqueAction.h"
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
#include "uml/types/property.h"
#include "uml/types/realization.h"
#include "uml/types/reception.h"
#include "uml/types/redefinableTemplateSignature.h"
#include "uml/types/signal.h"
#include "uml/types/slot.h"
#include "uml/types/stereotype.h"
#include "uml/types/templateBinding.h"
#include "uml/types/templateParameter.h"
#include "uml/types/templateParameterSubstitution.h"
#include "uml/types/templateSignature.h"
#include "uml/umlPtr.h"
#include "uml/types/usage.h"
#include "uml/types/valuePin.h"

#include "uml/managers/manager.h"
#include "uml/managers/protocol/umlClient.h"

namespace UML {
    typedef UmlPtr<PrimitiveType> PrimitiveTypePtr;
    typedef UmlPtr<LiteralBool> LiteralBoolPtr;
    typedef UmlPtr<ExceptionHandler> ExceptionHandlerPtr;
    typedef UmlPtr<OpaqueAction> OpaqueActionPtr;
    typedef UmlPtr<ElementImport> ElementImportPtr;
    typedef UmlPtr<PackageImport> PackageImportPtr;
    typedef UmlPtr<EnumerationLiteral> EnumerationLiteralPtr;
}

#endif