#pragma once

#include "egm/egm-basic.h"
#include "uml/types/abstraction.h"
#include "uml/types/comment.h"
#include "uml/types/dependency.h"
#include "uml/types/directedRelationship.h"
#include "uml/types/element.h"
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
#include "uml/types/package.h"
#include "uml/types/packageableElement.h"
#include "uml/types/realization.h"
#include "uml/types/relationship.h"
#include "uml/types/type.h"
#include "uml/types/typedElement.h"
#include "uml/types/usage.h"
#include "uml/types/valueSpecification.h"

namespace UML {
    using UmlTypes = EGM::TemplateTypeList<
        Abstraction,
        Comment,
        Dependency,
        DirectedRelationship,
        Element,
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
        Package,
        PackageableElement,
        Realization,
        Type,
        TypedElement,
        Relationship,
        Usage,
        ValueSpecification
    >;

    using UmlManager = EGM::Manager<UmlTypes>;
}
