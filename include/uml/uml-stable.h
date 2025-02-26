#pragma once

#include "egm/egm.h"
#include "uml/types/abstraction.h"
#include "uml/types/association.h"
#include "uml/types/behavioralFeature.h"
#include "uml/types/class.h"
#include "uml/types/classifier.h"
#include "uml/types/comment.h"
#include "uml/types/connectableElement.h"
#include "uml/types/dataType.h"
#include "uml/types/dependency.h"
#include "uml/types/directedRelationship.h"
#include "uml/types/element.h"
#include "uml/types/encapsulatedClassifier.h"
#include "uml/types/enumeration.h"
#include "uml/types/enumerationLiteral.h"
#include "uml/types/feature.h"
#include "uml/types/generalization.h"
#include "uml/types/instanceSpecification.h"
#include "uml/types/instanceValue.h"
#include "uml/types/literalBoolean.h"
#include "uml/types/literalInteger.h"
#include "uml/types/literalNull.h"
#include "uml/types/literalReal.h"
#include "uml/types/literalSpecification.h"
#include "uml/types/literalString.h"
#include "uml/types/literalUnlimitedNatural.h"
#include "uml/types/multiplicityElement.h"
#include "uml/types/namedElement.h"
#include "uml/types/namespace.h"
#include "uml/types/operation.h"
#include "uml/types/package.h"
#include "uml/types/packageableElement.h"
#include "uml/types/parameter.h"
#include "uml/types/primitiveType.h"
#include "uml/types/property.h"
#include "uml/types/realization.h"
#include "uml/types/reception.h"
#include "uml/types/redefinableElement.h"
#include "uml/types/relationship.h"
#include "uml/types/signal.h"
#include "uml/types/slot.h"
#include "uml/types/structuralFeature.h"
#include "uml/types/structuredClassifier.h"
#include "uml/types/type.h"
#include "uml/types/typedElement.h"
#include "uml/types/usage.h"
#include "uml/types/valueSpecification.h"

namespace UML {
    using UmlTypes = EGM::TemplateTypeList<
        Abstraction,
        Association,
        BehavioralFeature,
        Class,
        Classifier,
        Comment,
        ConnectableElement,
        DataType,
        Dependency,
        DirectedRelationship,
        Element,
        EncapsulatedClassifier,
        Enumeration,
        EnumerationLiteral,
        Feature,
        Generalization,
        InstanceSpecification,
        InstanceValue,
        LiteralBoolean,
        LiteralInteger,
        LiteralNull,
        LiteralReal,
        LiteralSpecification,
        LiteralString,
        LiteralUnlimitedNatural,
        MultiplicityElement,
        NamedElement,
        Namespace,
        Operation,
        Package,
        PackageableElement,
        PrimitiveType,
        Property,
        Realization,
        Reception,
        RedefinableElement,
        Relationship,
        Signal,
        Slot,
        StructuralFeature,
        StructuredClassifier,
        Type,
        TypedElement,
        Usage,
        ValueSpecification
    >;

    using UmlManager = EGM::Manager<UmlTypes>;
}
