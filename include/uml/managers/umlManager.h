#pragma once

#include "uml/managers/templateTypeList.h"
#include "uml/managers/manager.h"
#include "uml/types/packageableElement/impl.h"
#include "uml/types/package/impl.h"
#include "uml/types/element/impl.h"
#include "uml/types/namedElement/impl.h"
#include "uml/types/namespace/impl.h"

namespace UML {

    using UmlTypes = TemplateTypeList<Element, NamedElement, Namespace, Package, PackageableElement>;

    typedef Manager<UmlTypes> UmlManager;
}
