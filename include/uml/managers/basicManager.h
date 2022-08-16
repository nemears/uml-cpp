#ifndef _UML_MANAGERS_BASIC_MANAGER_H_
#define _UML_MANAGERS_BASIC_MANAGER_H_

#include "managerPolicy.h"
#include "simpleAccessPolicy.h"
#include "filePersistencePolicy.h"

namespace UML {
    typedef Manager<SimpleAccessPolicy, FilePersistencePolicy> BasicManager;
}

#endif