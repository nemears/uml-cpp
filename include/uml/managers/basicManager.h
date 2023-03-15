#ifndef _UML_MANAGERS_BASIC_MANAGER_H_
#define _UML_MANAGERS_BASIC_MANAGER_H_

#include "manager.h"
#include "simpleAccessPolicy.h"
#include "serialization/open_uml/openUmlSerializationPolicy.h"

namespace UML {
    typedef Manager<SimpleAccessPolicy, OpenUmlSerializationPolicy> BasicManager;
}

#endif