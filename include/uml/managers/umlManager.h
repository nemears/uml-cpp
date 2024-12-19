#pragma once

#include "uml/managers/templateTypeList.h"
#include "uml/managers/manager.h"
#include "uml/types/element.h"
#include "uml/types/namedElement.h"
#include "uml/types/namespace.h"
#include "uml/types/packageableElement.h"
#include "uml/types/package.h"

namespace UML {

    using UmlTypes = TemplateTypeList<Element, NamedElement, Namespace, PackageableElement, Package>;

    typedef Manager<UmlTypes> UmlManager;
}
