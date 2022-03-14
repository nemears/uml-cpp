#ifndef UML_UML_STABLE_H
#define UML_UML_STABLE_H

/**
 * Comprehensive include of all stable non-abstract parts of the api
 **/

#include "uml/abstraction.h"
#include "uml/activity.h"
#include "uml/activityEdge.h"
#include "uml/activityNode.h"
#include "uml/behavior.h"
#include "uml/class.h"
#include "uml/comment.h"
#include "uml/artifact.h"
#include "uml/association.h"
#include "uml/connector.h"
#include "uml/connectorEnd.h"
#include "uml/dataType.h"
#include "uml/dependency.h"
#include "uml/deployment.h"
#include "uml/enumeration.h"
#include "uml/enumerationLiteral.h"
#include "uml/expression.h"
#include "uml/extension.h"
#include "uml/extensionEnd.h"
#include "uml/generalization.h"
#include "uml/generalizationSet.h"
#include "uml/instanceSpecification.h"
#include "uml/instanceValue.h"
#include "uml/interface.h"
#include "uml/interfaceRealization.h"
#include "uml/literalBool.h"
#include "uml/literalInt.h"
#include "uml/literalNull.h"
#include "uml/literalReal.h"
#include "uml/literalString.h"
#include "uml/literalUnlimitedNatural.h"
#include "uml/manifestation.h"
#include "uml/model.h"
#include "uml/opaqueBehavior.h"
#include "uml/operation.h"
#include "uml/package.h"
#include "uml/packageMerge.h"
#include "uml/parameter.h"
#include "uml/port.h"
#include "uml/primitiveType.h"
#include "uml/profile.h"
#include "uml/profileApplication.h"
#include "uml/property.h"
#include "uml/realization.h"
#include "uml/reception.h"
#include "uml/signal.h"
#include "uml/slot.h"
#include "uml/stereotype.h"
#include "uml/templateBinding.h"
#include "uml/templateParameter.h"
#include "uml/templateParameterSubstitution.h"
#include "uml/templateSignature.h"
#include "uml/umlManager.h"
#include "uml/umlPtr.h"
#include "uml/usage.h"

#include "uml/managers/umlClient.h"

namespace UML {
    typedef UmlPtr<PrimitiveType> PrimitiveTypePtr;
}

#endif