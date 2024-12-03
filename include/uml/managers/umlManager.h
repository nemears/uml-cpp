#pragma once

#include "uml/managers/templateTypeList.h"
#include "uml/managers/manager.h"
#include "uml/types/packageableElement/impl.h"
#include "uml/types/package/impl.h"

namespace UML {

    using UmlTypes = TemplateTypeList<Package, PackageableElement>;

    typedef Manager<UmlTypes> UmlManager;
}
